# KAOS Cipher - Python Implementation

A Python implementation of the KAOS stream cipher based on the Lorenz chaotic system.

## Features

- **256-bit key + 96-bit nonce** for strong security
- **Pure Python implementation** with no external dependencies
- **Professional API** compatible with the original C version
 
## Quick Start

```python
from kaos import KaosCipher
import os

cipher = KaosCipher()
key = os.urandom(32)    # 256-bit key
nonce = os.urandom(12)  # 96-bit nonce

plaintext = b"Secret message"
ciphertext = cipher.encrypt(plaintext, key, nonce)
decrypted = cipher.decrypt(ciphertext, key, nonce)  
```
## Implementation Files  

`kaos.py` - Core cipher implementation with encryption/decryption API  
`test_kaos.py` - Comprehensive test suite with error handling validation  
`basic_usage.py` - Simple examples demonstrating basic functionality  
`visual.py` - Lorenz attractor visualization (requires matplotlib/numpy)  

---
Part of the KAOS Cipher project - Chaotic cryptography based on Lorenz attractor
