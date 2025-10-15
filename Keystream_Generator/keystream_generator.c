/**
 * KAOS-256 Cipher - KEYSTREAM GENERATOR
 * Keystream generation for cryptographic testing
 * Author: Simón M. Guiñazú
 * Repository: https://github.com/sysphersec/kaos-cipher
 */

#include "kaos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * Generates keystream for cryptographic testing with fixed key/nonce
 * Uses reproducible parameters for consistent test results
 * 
 * @param len Length of keystream to generate in bytes
 * @return    Allocated buffer with keystream, NULL on error
 */
uint8_t* generate_test_keystream(size_t len) {
    KaosCipher cipher;
    kaos_init(&cipher);  // Uses DEFAULT parameters from kaos.h
    
    uint8_t* keystream = (uint8_t*)malloc(len);
    if (!keystream) {
        fprintf(stderr, "Memory allocation failed for %zu bytes\n", len);
        return NULL;
    }
    
    /* Fixed key/nonce for reproducibility */
    uint8_t key[KAOS_KEY_SIZE];
    uint8_t nonce[KAOS_NONCE_SIZE];
    
    memset(key, 0x42, KAOS_KEY_SIZE);      /* All 0x42 pattern */
    memset(nonce, 0x99, KAOS_NONCE_SIZE);  /* All 0x99 pattern */
    
    printf("Using fixed key: ");
    for (int i = 0; i < 8; i++) printf("%02x", key[i]);
    printf("...\n");
    
    printf("Using fixed nonce: ");
    for (int i = 0; i < 6; i++) printf("%02x", nonce[i]);
    printf("...\n");
    
    double x, y, z;
    kaos_key_to_state(key, nonce, &x, &y, &z);
    
    printf("Warmup phase: %d iterations...\n", cipher.warmup);
    
    /* Warmup phase - Critical for chaos development */
    for (int i = 0; i < cipher.warmup; i++) {
        lorenz_step(&cipher, &x, &y, &z);
        if (i % 1000 == 0 && i > 0) {
            printf("   Progress: %d/%d\n", i, cipher.warmup);
        }
    }
    
    printf("Generating keystream: %zu bytes...\n", len);
    
    /* Generate keystream bytes using KAOS library */
    for (size_t i = 0; i < len; i++) {
        lorenz_step(&cipher, &x, &y, &z);
        keystream[i] = kaos_keystream_byte(x, y, z, i);
        
        /* Progress indicator for large generations */
        if (len > 1000000 && i % (len/10) == 0 && i > 0) {
            int progress = (int)((i * 100) / len);
            printf("   Progress: %d%% (%zu/%zu bytes)\n", progress, i, len);
        }
    }
    
    printf("Keystream generation completed\n");
    return keystream;
}

/**
 * Performs quick statistical verification of generated keystream
 * Basic frequency and distribution tests for quality assurance
 * 
 * @param keystream Pointer to generated keystream data
 * @param len       Length of keystream in bytes
 */
void verify_keystream(const uint8_t* keystream, size_t len) {
    if (!keystream || len == 0) return;
    
    printf("\nQuick keystream verification:\n");
    
    /* Count ones for basic frequency test */
    int ones = 0;
    for (size_t i = 0; i < len; i++) {
        for (int bit = 0; bit < 8; bit++) {
            if (keystream[i] & (1 << bit)) ones++;
        }
    }
    
    double proportion = (double)ones / (len * 8);
    printf("   Bit balance: %.6f (ideal: 0.500000)\n", proportion);
    
    /* Byte distribution analysis */
    int byte_count[256] = {0};
    for (size_t i = 0; i < len; i++) {
        byte_count[keystream[i]]++;
    }
    
    double expected = len / 256.0;
    double chi2 = 0.0;
    for (int i = 0; i < 256; i++) {
        double diff = byte_count[i] - expected;
        chi2 += (diff * diff) / expected;
    }
    
    printf("   Chi-squared: %.2f (ideal: ~255)\n", chi2);
    printf("   Uniform distribution: %s\n", 
           (chi2 > 200 && chi2 < 300) ? "GOOD" : "CHECK");
}

/**
 * Displays usage information and command examples
 * 
 * @param program_name Name of the executable for usage examples
 */
void print_usage(const char* program_name) {
    printf("KAOS Cipher Keystream Generator\n");
    printf("Cryptographic testing tool - Uses KAOS library\n\n");
    
    printf("USAGE: %s <size_in_bytes> <output_file>\n\n", program_name);
    
    printf("EXAMPLES:\n");
    printf("  %s 1000000 kaos_1MB.bin        # 1MB for quick tests\n", program_name);
    printf("  %s 100000000 kaos_100MB.bin    # 100MB for NIST STS\n", program_name);
    printf("  %s 1000000000 kaos_1GB.bin     # 1GB for extensive testing\n\n", program_name);
    
    printf("RECOMMENDED SIZES:\n");
    printf("  NIST STS:       100MB - 1GB\n");
    printf("  Dieharder:      100MB - 1GB\n");
    printf("  TestU01:        1GB+\n");
    printf("  ENT:            Any size\n");
}

/**
 * Main function - handles command line arguments and coordinates keystream generation
 * 
 * @param argc Argument count
 * @param argv Argument vector
 * @return     Exit status (0 = success, 1 = error)
 */
int main(int argc, char* argv[]) {
    printf("===============================================\n");
    printf("         KAOS CIPHER - KEYSTREAM GENERATOR     \n");
    printf("===============================================\n\n");
    
    if (argc != 3) {
        print_usage(argv[0]);
        return 1;
    }
    
    /* Parse command line arguments */
    size_t size = atol(argv[1]);
    const char* filename = argv[2];
    
    if (size <= 0) {
        printf("Error: Invalid size '%s'\n", argv[1]);
        return 1;
    }
    
    printf("Starting keystream generation...\n");
    printf("   Size: %zu bytes (%.2f MB)\n", size, size / (1024.0 * 1024.0));
    printf("   Output: %s\n\n", filename);
    
    /* Generate keystream and measure performance */
    clock_t start_time = clock();
    uint8_t* keystream = generate_test_keystream(size);
    clock_t end_time = clock();
    
    if (!keystream) {
        printf("Keystream generation failed\n");
        return 1;
    }
    
    /* Calculate generation speed and statistics */
    double cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    double throughput = size / (1024.0 * 1024.0) / cpu_time_used;
    
    printf("\nGeneration statistics:\n");
    printf("   CPU time: %.3f seconds\n", cpu_time_used);
    printf("   Throughput: %.2f MB/s\n", throughput);
    
    /* Perform quick quality verification */
    verify_keystream(keystream, size > 10000 ? 10000 : size);
    
    /* Save keystream to file for external testing */
    printf("\nSaving to file: %s\n", filename);
    FILE* f = fopen(filename, "wb");
    if (!f) {
        printf("Error: Cannot create file '%s'\n", filename);
        free(keystream);
        return 1;
    }
    
    size_t written = fwrite(keystream, 1, size, f);
    fclose(f);
    free(keystream);
    
    if (written != size) {
        printf("Error: Only %zu/%zu bytes written\n", written, size);
        return 1;
    }
    
    printf("Success: %zu bytes written to %s\n", written, filename);
    printf("Keystream ready for cryptographic testing!\n\n");
    
    /* Provide testing instructions for common test suites */
    printf("NEXT STEPS:\n");
    printf("  NIST STS:    ./sts-2.1.2/assess 1000000\n");
    printf("  Dieharder:   dieharder -a -f %s\n", filename);
    printf("  ENT:         ent %s\n", filename);
    printf("  TestU01:     bbattery %s\n", filename);
    
    return 0;
}
