/**
 * KAOS CIPHER - Stream Cipher Based on Lorenz Chaotic System
 * Header Core Cipher API
 * RAW KEY 256-bit + 96-bit Nonce Implementation
 * Author: Simón M. Guiñazú
 * Github: https://github.com/sysphersec/kaos-cipher
 */
 
#ifndef KAOS_CIPHER_H
#define KAOS_CIPHER_H

#include <stdint.h>
#include <stddef.h>

/* Cryptographic constants */
#define KAOS_KEY_SIZE 32      // 256 bits
#define KAOS_NONCE_SIZE 12    // 96 bits  
#define KAOS_WARMUP_DEFAULT 5000

/* Cipher parameters structure */
typedef struct {
    double sigma;      // 10.0 - Prandtl number (FIXED)
    double rho;        // 28.0 - Rayleigh number (FIXED)  
    double beta;       // 8/3  - Aspect ratio (FIXED)
    double dt;         // 0.01 - Time step (FIXED)
    int warmup;        // Warmup iterations (FIXED)
} KaosCipher;

/* Core API Functions */

/**
 * Initialize cipher with SECURE DEFAULT parameters
 * Parameters optimized for cryptographic security - DO NOT MODIFY
 */
void kaos_init(KaosCipher* cipher);

/**
 * Encrypt plaintext using 256-bit key and 96-bit nonce
 * Returns allocated ciphertext buffer or NULL on error
 * Caller must free() the returned buffer
 */
uint8_t* kaos_encrypt(KaosCipher* cipher, const uint8_t* plaintext, 
                      size_t length, const uint8_t* key_256bit, 
                      const uint8_t* nonce_96bit);

/**
 * Decrypt ciphertext (identical to encryption due to XOR symmetry)
 * Returns allocated plaintext buffer or NULL on error  
 * Caller must free() the returned buffer
 */
uint8_t* kaos_decrypt(KaosCipher* cipher, const uint8_t* ciphertext, 
                      size_t length, const uint8_t* key_256bit, 
                      const uint8_t* nonce_96bit);

/**
 * Generate single keystream byte
 */
uint8_t kaos_keystream_byte(double x, double y, double z, uint64_t counter);

/**
 * Lorenz system step - core chaotic dynamics
 */
void lorenz_step(KaosCipher* cipher, double* x, double* y, double* z);

/**
 * Transform key+nonce to initial chaotic state
 */
void kaos_key_to_state(const uint8_t* key_256bit, const uint8_t* nonce_96bit, 
                       double* x, double* y, double* z);

#endif /* KAOS_CIPHER_H */
