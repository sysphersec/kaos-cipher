"""
Test script for KAOS Cipher Python implementation
"""

import os
from kaos import KaosCipher, KAOS_KEY_SIZE, KAOS_NONCE_SIZE

def test_kaos_cipher():
    """Comprehensive test of KAOS cipher functionality"""
    print("=== KAOS Cipher Python Implementation Test ===\n")
    
    # Initialize cipher
    cipher = KaosCipher()
    print("✓ KAOS Cipher initialized with secure defaults")
    
    # Generate test key and nonce
    key = os.urandom(KAOS_KEY_SIZE)
    nonce = os.urandom(KAOS_NONCE_SIZE)
    print(f"✓ Generated {KAOS_KEY_SIZE*8}-bit key and {KAOS_NONCE_SIZE*8}-bit nonce")
    
    # Test messages
    test_messages = [
        b"Hello, World!",
        b"KAOS Cipher - Chaotic Encryption",
        b"X" * 100,  # Repeated characters
        b"\x00" * 50,  # Null bytes
        os.urandom(64)  # Random data
    ]
    
    for i, plaintext in enumerate(test_messages):
        print(f"\n--- Test {i+1}: {len(plaintext)} bytes ---")
        print(f"Plaintext: {plaintext[:20]}{'...' if len(plaintext) > 20 else ''}")
        
        # Encrypt
        ciphertext = cipher.encrypt(plaintext, key, nonce)
        print(f"Ciphertext: {ciphertext[:20]}{'...' if len(ciphertext) > 20 else ''}")
        
        # Decrypt
        decrypted = cipher.decrypt(ciphertext, key, nonce)
        print(f"Decrypted: {decrypted[:20]}{'...' if len(decrypted) > 20 else ''}")
        
        # Verify
        if plaintext == decrypted:
            print("✓ Encryption/Decryption successful")
        else:
            print("✗ Encryption/Decryption failed!")
            return False
        
        # Verify ciphertext is different from plaintext
        if ciphertext != plaintext:
            print("✓ Ciphertext differs from plaintext")
        else:
            print("✗ Ciphertext should not equal plaintext!")
            return False
    
    # Test error handling
    print("\n--- Error Handling Tests ---")
    
    # Wrong key size
    try:
        wrong_key = os.urandom(16)  # 128 bits instead of 256
        cipher.encrypt(b"test", wrong_key, nonce)
        print("✗ Should have rejected wrong key size")
        return False
    except ValueError as e:
        print(f"✓ Correctly rejected wrong key size: {e}")
    
    # Wrong nonce size
    try:
        wrong_nonce = os.urandom(8)  # 64 bits instead of 96
        cipher.encrypt(b"test", key, wrong_nonce)
        print("✗ Should have rejected wrong nonce size")
        return False
    except ValueError as e:
        print(f"✓ Correctly rejected wrong nonce size: {e}")
    
    print("\n=== All tests passed! KAOS Cipher is working correctly ===")
    return True

if __name__ == "__main__":
    test_kaos_cipher()