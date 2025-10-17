/**
 * EXAMPLE 2: File Encryption/Decryption
 * Complete file protection using KAOS Cipher
 */

#include "kaos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void generate_random_key(uint8_t* key, size_t key_size) {
    for (size_t i = 0; i < key_size; i++) {
        key[i] = rand() % 256;
    }
}

int read_file(const char* filename, uint8_t** buffer, size_t* length) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("ERROR: Cannot open file '%s'\n", filename);
        return 0;
    }
    
    fseek(file, 0, SEEK_END);
    *length = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    *buffer = (uint8_t*)malloc(*length);
    if (!*buffer) {
        printf("ERROR: Memory allocation failed\n");
        fclose(file);
        return 0;
    }
    
    fread(*buffer, 1, *length, file);
    fclose(file);
    
    return 1;
}

int write_file(const char* filename, const uint8_t* data, size_t length) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        printf("ERROR: Cannot create file '%s'\n", filename);
        return 0;
    }
    
    fwrite(data, 1, length, file);
    fclose(file);
    
    return 1;
}

void save_key_to_file(const char* filename, const uint8_t* key, const uint8_t* nonce) {
    FILE* file = fopen(filename, "wb");
    if (file) {
        fwrite(key, 1, KAOS_KEY_SIZE, file);
        fwrite(nonce, 1, KAOS_NONCE_SIZE, file);
        fclose(file);
        printf("Key/Nonce saved to: %s\n", filename);
    }
}

int load_key_from_file(const char* filename, uint8_t* key, uint8_t* nonce) {
    FILE* file = fopen(filename, "rb");
    if (!file) return 0;
    
    fread(key, 1, KAOS_KEY_SIZE, file);
    fread(nonce, 1, KAOS_NONCE_SIZE, file);
    fclose(file);
    
    return 1;
}

void print_progress(size_t current, size_t total, const char* operation) {
    int percent = (int)((current * 100) / total);
    printf("\r%s: %d%% [%zu/%zu bytes]", operation, percent, current, total);
    fflush(stdout);
}

int encrypt_file(const char* input_file, const char* output_file, 
                 const char* key_file) {
    printf("ENCRYPTING FILE: %s -> %s\n", input_file, output_file);
    
    // Read input file
    uint8_t* file_data;
    size_t file_size;
    
    if (!read_file(input_file, &file_data, &file_size)) {
        return 0;
    }
    
    printf("File size: %zu bytes\n", file_size);
    
    // Initialize cipher
    KaosCipher cipher;
    kaos_init(&cipher);
    
    // Generate key and nonce
    uint8_t key[KAOS_KEY_SIZE];
    uint8_t nonce[KAOS_NONCE_SIZE];
    
    srand(time(NULL));
    generate_random_key(key, KAOS_KEY_SIZE);
    generate_random_key(nonce, KAOS_NONCE_SIZE);
    
    // Save key for decryption
    save_key_to_file(key_file, key, nonce);
    
    // Encrypt file
    printf("Encrypting...\n");
    uint8_t* encrypted_data = kaos_encrypt(&cipher, file_data, file_size, key, nonce);
    
    if (!encrypted_data) {
        printf("ERROR: Encryption failed!\n");
        free(file_data);
        return 0;
    }
    
    // Write encrypted file
    if (write_file(output_file, encrypted_data, file_size)) {
        printf("Encryption completed: %s\n", output_file);
    }
    
    // Cleanup
    free(file_data);
    free(encrypted_data);
    
    return 1;
}

int decrypt_file(const char* input_file, const char* output_file,
                 const char* key_file) {
    printf("DECRYPTING FILE: %s -> %s\n", input_file, output_file);
    
    // Read encrypted file
    uint8_t* encrypted_data;
    size_t file_size;
    
    if (!read_file(input_file, &encrypted_data, &file_size)) {
        return 0;
    }
    
    printf("File size: %zu bytes\n", file_size);
    
    // Load key and nonce
    uint8_t key[KAOS_KEY_SIZE];
    uint8_t nonce[KAOS_NONCE_SIZE];
    
    if (!load_key_from_file(key_file, key, nonce)) {
        printf("ERROR: Cannot load key file '%s'\n", key_file);
        free(encrypted_data);
        return 0;
    }
    
    // Initialize cipher
    KaosCipher cipher;
    kaos_init(&cipher);
    
    // Decrypt file
    printf("Decrypting...\n");
    uint8_t* decrypted_data = kaos_decrypt(&cipher, encrypted_data, file_size, key, nonce);
    
    if (!decrypted_data) {
        printf("ERROR: Decryption failed!\n");
        free(encrypted_data);
        return 0;
    }
    
    // Write decrypted file
    if (write_file(output_file, decrypted_data, file_size)) {
        printf("Decryption completed: %s\n", output_file);
    }
    
    // Cleanup
    free(encrypted_data);
    free(decrypted_data);
    
    return 1;
}

int main(int argc, char* argv[]) {
    printf("=== KAOS CIPHER - FILE ENCRYPTION DEMO ===\n\n");
    
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        printf("Example: %s document.pdf\n", argv[0]);
        return 1;
    }
    
    const char* input_file = argv[1];
    const char* encrypted_file = "encrypted.kaos";
    const char* decrypted_file = "decrypted_file";
    const char* key_file = "encryption_key.bin";
    
    // Step 1: Encrypt file
    printf("STEP 1: ENCRYPTION\n");
    printf("==================\n");
    if (!encrypt_file(input_file, encrypted_file, key_file)) {
        return 1;
    }
    
    printf("\n");
    
    // Step 2: Decrypt file
    printf("STEP 2: DECRYPTION\n");
    printf("==================\n");
    if (!decrypt_file(encrypted_file, decrypted_file, key_file)) {
        return 1;
    }
    
    printf("\n");
    printf("   FILE PROTECTION COMPLETED SUCCESSFULLY!\n");
    printf("   Original: %s\n", input_file);
    printf("   Encrypted: %s\n", encrypted_file);
    printf("   Decrypted: %s\n", decrypted_file);
    printf("   Key file: %s (KEEP THIS SAFE!)\n", key_file);
    
    return 0;
}
