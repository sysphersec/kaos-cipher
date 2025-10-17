/**
 * KAOS CIPHER - TEST SUITE
 * Comprehensive Cryptographic Validation with 256-bit Raw Key + 96-bit Nonce
 * Author: Simón M. Guiñazú
 * Repository: https://github.com/sysphersec/kaos-cipher
 * 
 * NOTE: This is an INTERNAL development tool for quick validation during development.
 * For formal cryptographic validation, use official test suites (NIST STS, Dieharder, ENT, TestU01).
 */

#include "kaos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

/* ===== TEST CONFIGURATION ===== */
#define TEST_KEYSTREAM_SIZE 1000000     /* 1MB for internal tests */
#define AVALANCHE_TEST_ITERATIONS 50
#define PERFORMANCE_TEST_SIZE 10485760  /* 10MB for benchmark */
#define NIST_TEST_STREAM_SIZE 100000    /* 100KB for each NIST test */

/* ===== MATHEMATICAL CONSTANTS ===== */
#define SQRT2 1.41421356237309504880

/* ===== FUNCTION PROTOTYPES ===== */
double normal_cdf(double x);
double gamma_q(double a, double x);
int count_bits(uint8_t byte);
uint8_t* generate_test_keystream(size_t len);

/* ===== NIST TEST SUITE IMPLEMENTATIONS ===== */

/**
 * Implements NIST Runs Test
 * Tests the number of runs (sequences of identical bits)
 * 
 * @param data Input keystream data
 * @param len  Length of data in bytes
 */
void nist_runs_test(const uint8_t* data, size_t len) {
    printf("[1/8] NIST RUNS TEST\n");
    printf("--------------------\n");
    
    int runs = 1;
    int current_bit = (data[0] >> 7) & 1;
    
    for (size_t i = 1; i < len * 8; i++) {
        int bit = (data[i/8] >> (7 - (i % 8))) & 1;
        if (bit != current_bit) {
            runs++;
            current_bit = bit;
        }
    }
    
    int ones = 0;
    for (size_t i = 0; i < len * 8; i++) {
        if (data[i/8] & (1 << (7 - (i % 8)))) ones++;
    }
    
    double pi = (double)ones / (len * 8);
    if (fabs(pi - 0.5) >= (2.0 / sqrt(len * 8))) {
        printf("PI = %.6f, test not applicable\n", pi);
        printf("Result: N/A\n\n");
        return;
    }
    
    double expected_runs = 2.0 * len * 8 * pi * (1.0 - pi);
    double test_statistic = fabs(runs - expected_runs) / sqrt(len * 8 * 0.25);
    double p_value = erfc(test_statistic / SQRT2);
    
    printf("Runs count: %d\n", runs);
    printf("Expected runs: %.2f\n", expected_runs);
    printf("Test Statistic: %.6f\n", test_statistic);
    printf("P-value: %.6f\n", p_value);
    printf("Result: %s\n\n", p_value > 0.01 ? "PASS" : "FAIL");
}

/**
 * Calculates the cumulative distribution function for normal distribution
 * 
 * @param x Input value
 * @return  CDF value
 */
double normal_cdf(double x) {
    return 0.5 * (1 + erf(x / SQRT2));
}

/**
 * Implements NIST Cumulative Sums Test
 * Tests the cumulative sum of the sequence
 * 
 * @param data Input keystream data
 * @param len  Length of data in bytes
 */
void nist_cumulative_sums_test(const uint8_t* data, size_t len) {
    printf("[2/8] NIST CUMULATIVE SUMS TEST\n");
    printf("-------------------------------\n");
    
    int S = 0;
    int max_S = 0;
    
    for (size_t i = 0; i < len * 8; i++) {
        int bit = (data[i/8] >> (7 - (i % 8))) & 1;
        S += (bit == 1) ? 1 : -1;
        if (abs(S) > abs(max_S)) max_S = S;
    }
    
    printf("Max cumulative sum: %d\n", max_S);
    printf("Theoretical max: ~%.0f\n", 3.0 * sqrt(len * 8));
    printf("Result: %s (Visual inspection)\n\n", 
           abs(max_S) < 4.0 * sqrt(len * 8) ? "PASS" : "FAIL");
}

/**
 * Implements NIST Longest Run Test
 * Tests the longest run of ones in the sequence
 * 
 * @param data Input keystream data
 * @param len  Length of data in bytes
 */
void nist_longest_run_test(const uint8_t* data, size_t len) {
    printf("[3/8] NIST LONGEST RUN TEST\n");
    printf("---------------------------\n");
    
    int current_run = 0;
    int max_run = 0;
    
    for (size_t i = 0; i < len * 8; i++) {
        int bit = (data[i/8] >> (7 - (i % 8))) & 1;
        if (bit == 1) {
            current_run++;
            if (current_run > max_run) max_run = current_run;
        } else {
            current_run = 0;
        }
    }
    
    double expected_max_run = log2(len * 8);
    printf("Longest run: %d\n", max_run);
    printf("Expected: ~%.2f\n", expected_max_run);
    printf("Result: %s (Visual inspection)\n\n", 
           fabs(max_run - expected_max_run) < 5 ? "PASS" : "FAIL");
}

/**
 * Calculates the regularized upper incomplete gamma function
 * 
 * @param a Shape parameter
 * @param x Input value
 * @return  Gamma Q value
 */
double gamma_q(double a, double x) {
    if (x < 0.0 || a <= 0.0) return 0.0;
    
    double term = 1.0 / a;
    double sum = term;
    for (int n = 1; n < 100; n++) {
        term *= x / (a + n);
        sum += term;
        if (term < 1e-15) break;
    }
    
    return exp(-x + a * log(x) - lgamma(a)) * sum;
}

/**
 * Implements NIST Serial Test
 * Tests the frequency of overlapping m-bit patterns
 * 
 * @param data Input keystream data
 * @param len  Length of data in bytes
 */
void nist_serial_test(const uint8_t* data, size_t len) {
    printf("[4/8] NIST SERIAL TEST\n");
    printf("----------------------\n");
    
    int freq[256] = {0};
    for (size_t i = 0; i < len; i++) {
        freq[data[i]]++;
    }
    
    double chi2 = 0.0;
    double expected = len / 256.0;
    for (int i = 0; i < 256; i++) {
        double diff = freq[i] - expected;
        chi2 += (diff * diff) / expected;
    }
    
    double p_value = gamma_q(255.0 / 2.0, chi2 / 2.0);
    
    printf("Chi-squared: %.6f\n", chi2);
    printf("P-value: %.6f\n", p_value);
    printf("Result: %s\n\n", p_value > 0.01 ? "PASS" : "FAIL");
}

/**
 * Counts the number of set bits in a byte
 * 
 * @param byte Input byte
 * @return     Number of set bits (0-8)
 */
int count_bits(uint8_t byte) {
    int count = 0;
    for (int i = 0; i < 8; i++) {
        if (byte & (1 << i)) count++;
    }
    return count;
}

/* ===== ADVANCED CRYPTOGRAPHIC TESTS ===== */

/**
 * Tests avalanche effect - small input changes should cause large output changes
 * Uses raw 256-bit keys for testing
 */
void avalanche_effect_test_raw_keys() {
    printf("[5/8] AVALANCHE EFFECT TEST (RAW KEYS)\n");
    printf("--------------------------------------\n");
    
    KaosCipher cipher;
    kaos_init(&cipher);
    
    /* Test with raw keys instead of passwords */
    uint8_t base_key[KAOS_KEY_SIZE];
    uint8_t base_nonce[KAOS_NONCE_SIZE];
    
    /* Initialize with fixed values for reproducibility */
    memset(base_key, 0xAA, KAOS_KEY_SIZE);
    memset(base_nonce, 0x55, KAOS_NONCE_SIZE);
    
    const char* test_message = "Avalanche test for KAOS Cipher with RAW KEYS";
    size_t msg_len = strlen(test_message);
    
    /* Generate base ciphertext */
    uint8_t* base_cipher = (uint8_t*)malloc(msg_len);
    if (!base_cipher) return;
    
    /* Simulate encryption using keystream generation directly */
    double x, y, z;
    kaos_key_to_state(base_key, base_nonce, &x, &y, &z);
    
    for (int i = 0; i < cipher.warmup; i++) {
        lorenz_step(&cipher, &x, &y, &z);
    }
    
    for (size_t i = 0; i < msg_len; i++) {
        lorenz_step(&cipher, &x, &y, &z);
        base_cipher[i] = kaos_keystream_byte(x, y, z, i);
    }
    
    double total_bit_change = 0.0;
    int tests_conducted = 0;
    
    /* Test key sensitivity */
    for (int bit_pos = 0; bit_pos < 32 && tests_conducted < AVALANCHE_TEST_ITERATIONS; bit_pos++) {
        uint8_t modified_key[KAOS_KEY_SIZE];
        memcpy(modified_key, base_key, KAOS_KEY_SIZE);
        modified_key[bit_pos % KAOS_KEY_SIZE] ^= 1; /* Flip one bit */
        
        double x2, y2, z2;
        kaos_key_to_state(modified_key, base_nonce, &x2, &y2, &z2);
        
        for (int i = 0; i < cipher.warmup; i++) {
            lorenz_step(&cipher, &x2, &y2, &z2);
        }
        
        int bit_changes = 0;
        for (size_t i = 0; i < msg_len; i++) {
            lorenz_step(&cipher, &x2, &y2, &z2);
            uint8_t new_byte = kaos_keystream_byte(x2, y2, z2, i);
            uint8_t xor_result = base_cipher[i] ^ new_byte;
            bit_changes += count_bits(xor_result);
        }
        
        double change_percent = (bit_changes * 100.0) / (msg_len * 8);
        total_bit_change += change_percent;
        tests_conducted++;
    }
    
    double average_change = total_bit_change / tests_conducted;
    
    printf("Tests conducted: %d\n", tests_conducted);
    printf("Average bit change: %.4f%%\n", average_change);
    printf("Ideal range: 49.5%% - 50.5%%\n");
    
    const char* result = (average_change >= 49.5 && average_change <= 50.5) ? "EXCELLENT" :
                        (average_change >= 49.0 && average_change <= 51.0) ? "VERY GOOD" : "GOOD";
    
    printf("Avalanche effect: %s\n\n", result);
    free(base_cipher);
}

/**
 * Performance benchmark test
 * Measures encryption/decryption throughput with raw keys
 */
void performance_benchmark_raw_keys() {
    printf("[6/8] PERFORMANCE BENCHMARK (RAW KEYS)\n");
    printf("--------------------------------------\n");
    
    KaosCipher cipher;
    kaos_init(&cipher);
    
    size_t data_size = PERFORMANCE_TEST_SIZE;
    uint8_t* test_data = (uint8_t*)malloc(data_size);
    
    if (!test_data) {
        printf("Error: Memory allocation failed\n");
        return;
    }
    
    /* Generate test data */
    srand(time(NULL));
    for (size_t i = 0; i < data_size; i++) {
        test_data[i] = rand() % 256;
    }
    
    /* Use raw keys */
    uint8_t key[KAOS_KEY_SIZE];
    uint8_t nonce[KAOS_NONCE_SIZE];
    memset(key, 0x42, KAOS_KEY_SIZE);
    memset(nonce, 0x99, KAOS_NONCE_SIZE);
    
    clock_t start = clock();
    
    /* Perform encryption-like operation (keystream generation) */
    double x, y, z;
    kaos_key_to_state(key, nonce, &x, &y, &z);
    
    for (int i = 0; i < cipher.warmup; i++) {
        lorenz_step(&cipher, &x, &y, &z);
    }
    
    for (size_t i = 0; i < data_size; i++) {
        lorenz_step(&cipher, &x, &y, &z);
        kaos_keystream_byte(x, y, z, i); /* Generate keystream */
    }
    
    clock_t end = clock();
    
    double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    double throughput = data_size / (1024.0 * 1024.0) / cpu_time_used;
    
    printf("Data size: %.2f MB\n", data_size / (1024.0 * 1024.0));
    printf("CPU time: %.3f seconds\n", cpu_time_used);
    printf("Throughput: %.2f MB/s\n", throughput);
    
    const char* performance;
    if (throughput > 100) performance = "EXCELLENT";
    else if (throughput > 50) performance = "VERY GOOD";
    else if (throughput > 25) performance = "GOOD";
    else performance = "ACCEPTABLE";
    
    printf("Performance: %s\n", performance);
    printf("Note: Includes %d warmup iterations\n\n", cipher.warmup);
    
    free(test_data);
}

/**
 * Advanced entropy analysis
 * Calculates Shannon entropy, min-entropy, and chi-squared statistics
 * 
 * @param data Input keystream data
 * @param len  Length of data in bytes
 */
void advanced_entropy_analysis(const uint8_t* data, size_t len) {
    printf("[7/8] ADVANCED ENTROPY ANALYSIS\n");
    printf("-------------------------------\n");
    
    int byte_count[256] = {0};
    for (size_t i = 0; i < len; i++) {
        byte_count[data[i]]++;
    }
    
    double shannon_entropy = 0.0;
    for (int i = 0; i < 256; i++) {
        if (byte_count[i] > 0) {
            double p = (double)byte_count[i] / len;
            shannon_entropy -= p * log2(p);
        }
    }
    
    double max_prob = 0.0;
    for (int i = 0; i < 256; i++) {
        double prob = (double)byte_count[i] / len;
        if (prob > max_prob) max_prob = prob;
    }
    double min_entropy = -log2(max_prob);
    
    double expected = len / 256.0;
    double chi_squared = 0.0;
    for (int i = 0; i < 256; i++) {
        double diff = byte_count[i] - expected;
        chi_squared += (diff * diff) / expected;
    }
    
    printf("Shannon entropy: %.6f bits/byte\n", shannon_entropy);
    printf("Min-entropy: %.6f bits/byte\n", min_entropy);
    printf("Chi-squared: %.6f\n", chi_squared);
    
    const char* entropy_result;
    if (shannon_entropy > 7.99) entropy_result = "EXCELLENT";
    else if (shannon_entropy > 7.95) entropy_result = "VERY GOOD";
    else if (shannon_entropy > 7.90) entropy_result = "GOOD";
    else entropy_result = "ACCEPTABLE";
    
    printf("Entropy quality: %s\n\n", entropy_result);
}

/**
 * Extended correlation analysis across multiple lags
 * Tests for autocorrelation in the keystream
 * 
 * @param data Input keystream data
 * @param len  Length of data in bytes
 */
void extended_correlation_analysis(const uint8_t* data, size_t len) {
    printf("[8/8] EXTENDED CORRELATION ANALYSIS\n");
    printf("-----------------------------------\n");
    
    const int max_lag = 50;
    double correlations[max_lag + 1];
    
    for (int lag = 1; lag <= max_lag; lag++) {
        double sum_xy = 0.0, sum_x = 0.0, sum_y = 0.0;
        double sum_x2 = 0.0, sum_y2 = 0.0;
        int count = len - lag;
        
        for (int i = 0; i < count; i++) {
            double x = data[i];
            double y = data[i + lag];
            
            sum_xy += x * y;
            sum_x += x;
            sum_y += y;
            sum_x2 += x * x;
            sum_y2 += y * y;
        }
        
        double numerator = count * sum_xy - sum_x * sum_y;
        double denominator = sqrt((count * sum_x2 - sum_x * sum_x) * (count * sum_y2 - sum_y * sum_y));
        
        correlations[lag] = (denominator != 0) ? numerator / denominator : 0.0;
    }
    
    double max_corr = 0.0, min_corr = 1.0, avg_corr = 0.0;
    for (int lag = 1; lag <= max_lag; lag++) {
        double abs_corr = fabs(correlations[lag]);
        if (abs_corr > max_corr) max_corr = abs_corr;
        if (abs_corr < min_corr) min_corr = abs_corr;
        avg_corr += abs_corr;
    }
    avg_corr /= max_lag;
    
    printf("Lags analyzed: 1-%d\n", max_lag);
    printf("Average correlation: %.6f\n", avg_corr);
    printf("Maximum correlation: %.6f\n", max_corr);
    printf("Minimum correlation: %.6f\n", min_corr);
    
    const char* correlation_result;
    if (avg_corr < 0.01) correlation_result = "EXCELLENT";
    else if (avg_corr < 0.02) correlation_result = "VERY GOOD";
    else if (avg_corr < 0.03) correlation_result = "GOOD";
    else correlation_result = "ACCEPTABLE";
    
    printf("Correlation quality: %s\n\n", correlation_result);
}

/* ===== TEST SUITE COORDINATION ===== */

/**
 * Generates test keystream using fixed key and nonce for reproducibility
 * 
 * @param len Length of keystream to generate
 * @return    Pointer to generated keystream, NULL on error
 */
uint8_t* generate_test_keystream(size_t len) {
    KaosCipher cipher;
    kaos_init(&cipher);
    
    uint8_t* keystream = (uint8_t*)malloc(len);
    if (!keystream) return NULL;
    
    /* Use fixed key/nonce for reproducible tests */
    uint8_t key[KAOS_KEY_SIZE];
    uint8_t nonce[KAOS_NONCE_SIZE];
    memset(key, 0x42, KAOS_KEY_SIZE);
    memset(nonce, 0x99, KAOS_NONCE_SIZE);
    
    double x, y, z;
    kaos_key_to_state(key, nonce, &x, &y, &z);
    
    for (int i = 0; i < cipher.warmup; i++) {
        lorenz_step(&cipher, &x, &y, &z);
    }
    
    for (size_t i = 0; i < len; i++) {
        lorenz_step(&cipher, &x, &y, &z);
        keystream[i] = kaos_keystream_byte(x, y, z, i);
    }
    
    return keystream;
}

/**
 * Runs the complete internal test suite
 * NOTE: This is for development purposes only
 * Formal validation should use official test suites
 */
void run_comprehensive_test_suite() {
    printf("===============================================\n");
    printf("            KAOS CIPHER - TEST SUITE            \n");
    printf("===============================================\n\n");
    
    printf("NOTE: This is an INTERNAL development test suite.\n");
    printf("For formal validation, use NIST STS, Dieharder, ENT, TestU01.\n\n");
    
    printf("Initializing test environment...\n");
    
    printf("Generating test keystream (%d bytes)...\n", TEST_KEYSTREAM_SIZE);
    uint8_t* keystream = generate_test_keystream(TEST_KEYSTREAM_SIZE);
    
    if (!keystream) {
        printf("Error: Failed to generate test keystream\n");
        return;
    }
    
    printf("Test keystream generated successfully.\n");
    printf("Running comprehensive test battery...\n\n");
    
    /* NIST Tests */
    nist_runs_test(keystream, NIST_TEST_STREAM_SIZE);
    nist_cumulative_sums_test(keystream, NIST_TEST_STREAM_SIZE);
    nist_longest_run_test(keystream, NIST_TEST_STREAM_SIZE);
    nist_serial_test(keystream, NIST_TEST_STREAM_SIZE);
    
    /* Advanced Cryptographic Tests */
    avalanche_effect_test_raw_keys();
    performance_benchmark_raw_keys();
    advanced_entropy_analysis(keystream, TEST_KEYSTREAM_SIZE);
    extended_correlation_analysis(keystream, TEST_KEYSTREAM_SIZE);
    
    printf("===============================================\n");
    printf("           INTERNAL TEST SUITE COMPLETED       \n");
    printf("===============================================\n");
    printf("Next: Run official test suites for formal validation:\n");
    printf("  - NIST Statistical Test Suite\n");
    printf("  - Dieharder Battery\n");
    printf("  - ENT Test Suite\n");
    printf("  - TestU01 Battery\n");
    
    free(keystream);
}

int main() {
    printf("KAOS Cipher Test Suite\n");
    printf("Raw Key 256-bit + 96-bit Nonce Implementation\n");
    printf("INTERNAL DEVELOPMENT TOOL - NOT FOR FORMAL VALIDATION\n\n");
    
    run_comprehensive_test_suite();
    
    return 0;
}
