/**
 * KAOS CIPHER - Stream Cipher Based on Lorenz Chaotic System
 * Core Implementation File
 * RAW KEY 256-bit + 96-bit Nonce Implementation
 * Author: Simón M. Guiñazú
 * Github: https://github.com/sysphersec/kaos-cipher
 * 
 * Note: Uses only raw cryptographic keys
 * Key derivation (PBKDF2/Argon2) is EXTERNAL responsibility
 */
 
#include "kaos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* 
 * LORENZ SYSTEM - Mathematical core
 * Pure chaotic dynamics implementation
 */
void lorenz_step(KaosCipher* cipher, double* x, double* y, double* z) {
    double dx = cipher->sigma * (*y - *x) * cipher->dt;
    double dy = (*x * (cipher->rho - *z) - *y) * cipher->dt;
    double dz = (*x * *y - cipher->beta * *z) * cipher->dt;
    
    *x += dx;
    *y += dy;
    *z += dz;
}

/*
 * KEY & NONCE TO INITIAL STATE
 * Transforms 256-bit key + 96-bit nonce into initial conditions
 * Uses cryptographic mixing for sensitivity
 */
void kaos_key_to_state(const uint8_t* key_256bit, const uint8_t* nonce_96bit, 
                       double* x, double* y, double* z) {
    uint64_t h1 = 0, h2 = 0, h3 = 0;
    
    // Mix key bytes (256 bits = 32 bytes)
    for (int i = 0; i < 32; i++) {
        h1 = ((h1 << 5) | (h1 >> 59)) ^ (key_256bit[i] * 0x9E3779B97F4A7C15ULL);
        h2 = ((h2 << 7) | (h2 >> 57)) ^ (key_256bit[i] * 0xBF58476D1CE4E5B9ULL);
        h3 = ((h3 << 11) | (h3 >> 53)) ^ (key_256bit[i] * 0x94D049BB133111EBULL);
    }
    
    // Mix nonce bytes (96 bits = 12 bytes)  
    for (int i = 0; i < 12; i++) {
        h1 ^= (nonce_96bit[i] << (i * 3));
        h2 += (nonce_96bit[i] * (i + 1));
        h3 = ((h3 << 13) | (h3 >> 51)) ^ nonce_96bit[i];
    }
    
    // Final mixing for sensitivity
    h1 ^= h2 ^ h3;
    h2 += h1 * 3;
    h3 ^= (h1 << 32) | (h2 >> 32);
    
    // Normalize to [0,1] range using golden ratio primes
    const uint64_t prime1 = 0x9E3779B97F4A7C15ULL; // Golden ratio prime
    const uint64_t prime2 = 0xBF58476D1CE4E5B9ULL;
    const uint64_t prime3 = 0x94D049BB133111EBULL;
    
    *x = fmod((double)(h1 ^ prime1) / 18446744073709551615.0, 1.0);
    *y = fmod((double)(h2 ^ prime2) / 18446744073709551615.0, 1.0); 
    *z = fmod((double)(h3 ^ prime3) / 18446744073709551615.0, 1.0);
    
    // Ensure non-zero initial conditions
    if (*x == 0.0) *x = 0.1234567890123456;
    if (*y == 0.0) *y = 0.9876543210987654;
    if (*z == 0.0) *z = 0.5555555555555555;
}

/*
 * KEYSTREAM GENERATION - CORE INNOVATION
 * Combines chaotic state variables with mathematical constants
 * Non-linear post-processing
 */
uint8_t kaos_keystream_byte(double x, double y, double z, uint64_t counter) {
    // Mathematical constants for mixing
    const double phi = 1.6180339887498948482;   // Golden ratio
    const double e = 2.71828182845904523536;    // Euler's number
    const double pi = 3.14159265358979323846;   // Pi
    
    // Core Lorenz combination
    double combined = (x * phi) + (y * e) + (z * pi);
    double fractional = fabs(combined) - floor(fabs(combined));
    
    // Counter-based perturbation for uniqueness
    double perturbation = counter * 0.0000001; // Small perturbation
    fractional = fmod(fractional + perturbation, 1.0);
    
    // Convert to byte
    uint8_t byte = (uint8_t)(fractional * 256.0);
    
    // Non-linear post-processing
    byte = (byte + (counter & 0xFF)) & 0xFF;           // Avalanche enhancement
    byte ^= (byte >> 4) ^ (byte << 3) ^ (counter % 97); // Non-linearity
    byte = (byte * 167 + 123) & 0xFF;                  // Random walk
    
    return byte;
}

/* 
 * ENCRYPTION - Core cipher operation
 * Uses 256-bit key + 96-bit nonce
 */
uint8_t* kaos_encrypt(KaosCipher* cipher, const uint8_t* plaintext, 
                      size_t length, const uint8_t* key_256bit, 
                      const uint8_t* nonce_96bit) {
    
    // Validate inputs
    if (!plaintext || !key_256bit || !nonce_96bit || length <= 0) {
        return NULL;
    }
    
    // Initialize chaotic system from key+nonce
    double x, y, z;
    kaos_key_to_state(key_256bit, nonce_96bit, &x, &y, &z);
    
    // Warmup phase - Critical for chaos development
    for (int i = 0; i < cipher->warmup; i++) {
        lorenz_step(cipher, &x, &y, &z);
    }
    
    // Allocate ciphertext buffer
    uint8_t* ciphertext = (uint8_t*)malloc(length);
    if (!ciphertext) {
        return NULL;
    }
    
    // Encryption - XOR with keystream
    for (size_t i = 0; i < length; i++) {
        lorenz_step(cipher, &x, &y, &z);
        uint8_t k_byte = kaos_keystream_byte(x, y, z, i);
        ciphertext[i] = plaintext[i] ^ k_byte;
    }
    
    return ciphertext;
}

/* 
 * DECRYPTION - Identical to encryption (XOR symmetry)
 * Mathematical elegance preserved
 */
uint8_t* kaos_decrypt(KaosCipher* cipher, const uint8_t* ciphertext, 
                      size_t length, const uint8_t* key_256bit, 
                      const uint8_t* nonce_96bit) {
    // XOR symmetry - identical operation
    return kaos_encrypt(cipher, ciphertext, length, key_256bit, nonce_96bit);
}

/*
 * INITIALIZATION - SECURE DEFAULTS ONLY
 * Parameters optimized and validated for cryptographic security
 * DO NOT MODIFY - Security depends on these exact values
 */
void kaos_init(KaosCipher* cipher) {
    cipher->sigma = 10.0;        // Prandtl number - VALIDATED
    cipher->rho = 28.0;          // Rayleigh number - VALIDATED  
    cipher->beta = 8.0 / 3.0;    // Aspect ratio - VALIDATED
    cipher->dt = 0.01;           // Time step - VALIDATED
    cipher->warmup = KAOS_WARMUP_DEFAULT;  // Warmup iterations - VALIDATED
}
