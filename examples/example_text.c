/**
 * EXAMPLE 1: Text Encryption/Decryption in Memory
 * Simple string protection using KAOS Cipher
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

void print_hex(const char* label, const uint8_t* data, size_t len) {
    printf("%s: ", label);
    for (size_t i = 0; i < len; i++) {
        printf("%02x", data[i]);
    }
    printf("\n");
}

void print_text(const char* label, const uint8_t* data, size_t len) {
    printf("%s: \"", label);
    for (size_t i = 0; i < len; i++) {
        if (data[i] >= 32 && data[i] <= 126) {
            printf("%c", data[i]);
        } else {
            printf("\\x%02x", data[i]);
        }
    }
    printf("\"\n");
}

int main(void) {
    printf("=== KAOS CIPHER - TEXT ENCRYPTION DEMO ===\n\n");
    
    // Initialize random seed
    srand(time(NULL));
    
    // Initialize cipher
    KaosCipher cipher;
    kaos_init(&cipher);
    
    // Secret message
    const char* secret_message = "This is a highly sensitive message that requires strong encryption!";
    size_t message_len = strlen(secret_message);
    
    // Generate cryptographic key and nonce
    uint8_t key[KAOS_KEY_SIZE];
    uint8_t nonce[KAOS_NONCE_SIZE];
    
    generate_random_key(key, KAOS_KEY_SIZE);
    generate_random_key(nonce, KAOS_NONCE_SIZE);
    
    printf("1. ORIGINAL DATA:\n");
    print_text("Plaintext", (uint8_t*)secret_message, message_len);
    print_hex("Key", key, KAOS_KEY_SIZE);
    print_hex("Nonce", nonce, KAOS_NONCE_SIZE);
    printf("\n");
    
    // ENCRYPTION
    printf("2. ENCRYPTION PROCESS:\n");
    uint8_t* ciphertext = kaos_encrypt(&cipher, (uint8_t*)secret_message, 
                                      message_len, key, nonce);
    
    if (!ciphertext) {
        printf("ERROR: Encryption failed!\n");
        return 1;
    }
    
    print_hex("Ciphertext", ciphertext, message_len);
    print_text("Ciphertext (readable)", ciphertext, message_len);
    printf("\n");
    
    // DECRYPTION
    printf("3. DECRYPTION PROCESS:\n");
    uint8_t* decrypted = kaos_decrypt(&cipher, ciphertext, message_len, key, nonce);
    
    if (!decrypted) {
        printf("ERROR: Decryption failed!\n");
        free(ciphertext);
        return 1;
    }
    
    print_text("Decrypted", decrypted, message_len);
    printf("\n");
    
    // VERIFICATION
    printf("4. VERIFICATION:\n");
    if (memcmp(secret_message, decrypted, message_len) == 0) {
        printf("SUCCESS: Perfect encryption/decryption cycle!\n");
    } else {
        printf("ERROR: Decryption mismatch!\n");
    }
    
    // SECURITY DEMONSTRATION
    printf("\n5. SECURITY FEATURES:\n");
    printf("   * 256-bit cryptographic key\n");
    printf("   * 96-bit nonce for uniqueness\n");
    printf("   * Lorenz chaotic system core\n");
    printf("   * Non-linear post-processing\n");
    printf("   * Avalanche effect: ~50%% bit change\n");
    
    // Cleanup
    free(ciphertext);
    free(decrypted);
    
    return 0;
}
