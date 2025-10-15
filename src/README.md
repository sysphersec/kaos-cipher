# KAOS Cipher - Core Library

Core implementation of the KAOS-256 chaotic stream cipher based on the Lorenz attractor. Professional cryptographic library with 256-bit keys and 96-bit nonces.

## Overview

KAOS Cipher is a novel stream cipher that leverages the mathematical properties of the Lorenz chaotic system to generate cryptographically secure pseudorandom sequences. The design provides mathematical transparency while maintaining strong security through chaotic dynamics.

## Cryptographic Specifications

- **Key Size**: 256 bits (32 bytes) - raw cryptographic keys
- **Nonce Size**: 96 bits (12 bytes) - unique per encryption
- **Security**: Based on Lorenz chaotic system with proven chaotic properties
- **Architecture**: Stream cipher with XOR operation

## Core Components

### Chaotic System
- **Lorenz Attractor** with parameters (σ=10.0, ρ=28.0, β=8/3)
- **5000 warmup iterations** for proper chaos development
- **Time step 0.01** for numerical stability

### Key Processing
- **256-bit key + 96-bit nonce** to initial state transformation
- **Cryptographic mixing** using golden ratio primes
- **Sensitivity enhancement** through multiple mixing rounds

### Keystream Generation
- **Mathematical constant combination** (φ, e, π)
- **Non-linear post-processing** for cryptographic strength
- **Counter-based perturbation** for uniqueness

## API Reference

### Initialization
```c
KaosCipher cipher;
kaos_init(&cipher);  // Uses secure default parameters
```
Encryption/Decryption
```c
// Encryption
uint8_t* ciphertext = kaos_encrypt(&cipher, plaintext, length, key, nonce);
```
// Decryption (identical operation due to XOR symmetry)
```c
uint8_t* decrypted = kaos_decrypt(&cipher, ciphertext, length, key, nonce);
```
Low-level Functions
```c
// Keystream generation
uint8_t byte = kaos_keystream_byte(x, y, z, counter);

// Chaotic system step
lorenz_step(&cipher, &x, &y, &z);

// Key to state transformation
kaos_key_to_state(key, nonce, &x, &y, &z);
```
### Security Features
- Avalanche Effect ~50% bit change sensitivity
- Chaotic Properties sensitivity to initial conditions
- Non-linear Processing resistant to linear cryptanalysis
- Mathematical Transparency based on established chaotic systems

### Important Security Notes
- Uses raw cryptographic keys - key derivation is external responsibility
- Fixed parameters - optimized and validated for security (DO NOT MODIFY)
- Unique nonce required for each encryption to prevent reuse attacks
- Professional key management recommended for production use

### Integration
The library provides a clean C API suitable for integration into various applications.  
All memory management is explicit - caller must free() returned buffers.

### Validation
Comprehensive cryptographic validation available in the Test Suite including:

### NIST statistical tests

- Entropy analysis
- Correlation testing
- Avalanche effect measurement

---
Part of the KAOS Cipher project - Chaotic cryptography core implementation
