"""
KAOS CIPHER - Stream Cipher Based on Lorenz Chaotic System
Python Implementation
RAW KEY 256-bit + 96-bit Nonce Implementation
Author: Simón M. Guiñazú
Github: https://github.com/sysphersec/kaos-cipher

Note: Uses only raw cryptographic keys
Key derivation (PBKDF2/Argon2) is EXTERNAL responsibility
"""

import math
import struct
from typing import Optional, Tuple

# Cryptographic constants
KAOS_KEY_SIZE = 32      # 256 bits
KAOS_NONCE_SIZE = 12    # 96 bits  
KAOS_WARMUP_DEFAULT = 5000

class KaosCipher:
    """
    KAOS Cipher - Chaotic Stream Cipher based on Lorenz System
    """
    
    def __init__(self):
        """Initialize cipher with SECURE DEFAULT parameters"""
        self.sigma = 10.0        # Prandtl number - VALIDATED
        self.rho = 28.0          # Rayleigh number - VALIDATED  
        self.beta = 8.0 / 3.0    # Aspect ratio - VALIDATED
        self.dt = 0.01           # Time step - VALIDATED
        self.warmup = KAOS_WARMUP_DEFAULT  # Warmup iterations - VALIDATED
    
    def lorenz_step(self, x: float, y: float, z: float) -> Tuple[float, float, float]:
        """
        LORENZ SYSTEM - Mathematical core
        Pure chaotic dynamics implementation
        """
        dx = self.sigma * (y - x) * self.dt
        dy = (x * (self.rho - z) - y) * self.dt
        dz = (x * y - self.beta * z) * self.dt
        
        x += dx
        y += dy
        z += dz
        
        return x, y, z
    
    def kaos_key_to_state(self, key_256bit: bytes, nonce_96bit: bytes) -> Tuple[float, float, float]:
        """
        KEY & NONCE TO INITIAL STATE
        Transforms 256-bit key + 96-bit nonce into initial conditions
        Uses cryptographic mixing for sensitivity
        """
        if len(key_256bit) != KAOS_KEY_SIZE:
            raise ValueError(f"Key must be {KAOS_KEY_SIZE} bytes (256 bits)")
        if len(nonce_96bit) != KAOS_NONCE_SIZE:
            raise ValueError(f"Nonce must be {KAOS_NONCE_SIZE} bytes (96 bits)")
        
        h1 = 0
        h2 = 0
        h3 = 0
        
        # Mix key bytes (256 bits = 32 bytes)
        for i in range(32):
            key_byte = key_256bit[i]
            h1 = ((h1 << 5) | (h1 >> 59)) & 0xFFFFFFFFFFFFFFFF ^ (key_byte * 0x9E3779B97F4A7C15) & 0xFFFFFFFFFFFFFFFF
            h2 = ((h2 << 7) | (h2 >> 57)) & 0xFFFFFFFFFFFFFFFF ^ (key_byte * 0xBF58476D1CE4E5B9) & 0xFFFFFFFFFFFFFFFF
            h3 = ((h3 << 11) | (h3 >> 53)) & 0xFFFFFFFFFFFFFFFF ^ (key_byte * 0x94D049BB133111EB) & 0xFFFFFFFFFFFFFFFF
        
        # Mix nonce bytes (96 bits = 12 bytes)  
        for i in range(12):
            nonce_byte = nonce_96bit[i]
            h1 ^= (nonce_byte << (i * 3)) & 0xFFFFFFFFFFFFFFFF
            h2 = (h2 + (nonce_byte * (i + 1))) & 0xFFFFFFFFFFFFFFFF
            h3 = ((h3 << 13) | (h3 >> 51)) & 0xFFFFFFFFFFFFFFFF ^ nonce_byte
        
        # Final mixing for sensitivity
        h1 ^= h2 ^ h3
        h2 = (h2 + h1 * 3) & 0xFFFFFFFFFFFFFFFF
        h3 ^= ((h1 << 32) | (h2 >> 32)) & 0xFFFFFFFFFFFFFFFF
        
        # Normalize to [0,1] range using golden ratio primes
        prime1 = 0x9E3779B97F4A7C15  # Golden ratio prime
        prime2 = 0xBF58476D1CE4E5B9
        prime3 = 0x94D049BB133111EB
        
        x = math.fmod((h1 ^ prime1) / 18446744073709551615.0, 1.0)
        y = math.fmod((h2 ^ prime2) / 18446744073709551615.0, 1.0) 
        z = math.fmod((h3 ^ prime3) / 18446744073709551615.0, 1.0)
        
        # Ensure non-zero initial conditions
        if x == 0.0: x = 0.1234567890123456
        if y == 0.0: y = 0.9876543210987654
        if z == 0.0: z = 0.5555555555555555
        
        return x, y, z
    
    def kaos_keystream_byte(self, x: float, y: float, z: float, counter: int) -> int:
        """
        KEYSTREAM GENERATION - CORE INNOVATION
        Combines chaotic state variables with mathematical constants
        Non-linear post-processing
        """
        # Mathematical constants for mixing
        phi = 1.6180339887498948482   # Golden ratio
        e = 2.71828182845904523536    # Euler's number
        pi = 3.14159265358979323846   # Pi
        
        # Core Lorenz combination
        combined = (x * phi) + (y * e) + (z * pi)
        fractional = abs(combined) - math.floor(abs(combined))
        
        # Counter-based perturbation for uniqueness
        perturbation = counter * 0.0000001  # Small perturbation
        fractional = math.fmod(fractional + perturbation, 1.0)
        
        # Convert to byte
        byte_val = int(fractional * 256.0) & 0xFF
        
        # Non-linear post-processing
        byte_val = (byte_val + (counter & 0xFF)) & 0xFF           # Avalanche enhancement
        byte_val ^= (byte_val >> 4) ^ (byte_val << 3) ^ (counter % 97)  # Non-linearity
        byte_val = (byte_val * 167 + 123) & 0xFF                  # Random walk
        
        return byte_val
    
    def encrypt(self, plaintext: bytes, key_256bit: bytes, nonce_96bit: bytes) -> Optional[bytes]:
        """
        ENCRYPTION - Core cipher operation
        Uses 256-bit key + 96-bit nonce
        """
        # Validate inputs
        if not plaintext or not key_256bit or not nonce_96bit or len(plaintext) <= 0:
            return None
        
        if len(key_256bit) != KAOS_KEY_SIZE:
            raise ValueError(f"Key must be {KAOS_KEY_SIZE} bytes")
        if len(nonce_96bit) != KAOS_NONCE_SIZE:
            raise ValueError(f"Nonce must be {KAOS_NONCE_SIZE} bytes")
        
        # Initialize chaotic system from key+nonce
        x, y, z = self.kaos_key_to_state(key_256bit, nonce_96bit)
        
        # Warmup phase - Critical for chaos development
        for i in range(self.warmup):
            x, y, z = self.lorenz_step(x, y, z)
        
        # Encryption - XOR with keystream
        ciphertext = bytearray()
        for i in range(len(plaintext)):
            x, y, z = self.lorenz_step(x, y, z)
            k_byte = self.kaos_keystream_byte(x, y, z, i)
            ciphertext.append(plaintext[i] ^ k_byte)
        
        return bytes(ciphertext)
    
    def decrypt(self, ciphertext: bytes, key_256bit: bytes, nonce_96bit: bytes) -> Optional[bytes]:
        """
        DECRYPTION - Identical to encryption (XOR symmetry)
        Mathematical elegance preserved
        """
        # XOR symmetry - identical operation
        return self.encrypt(ciphertext, key_256bit, nonce_96bit)

# Convenience functions for direct API compatibility
def kaos_init() -> KaosCipher:
    """Initialize a new KAOS cipher instance"""
    return KaosCipher()

def kaos_encrypt(cipher: KaosCipher, plaintext: bytes, key_256bit: bytes, nonce_96bit: bytes) -> Optional[bytes]:
    """Encrypt plaintext using KAOS cipher"""
    return cipher.encrypt(plaintext, key_256bit, nonce_96bit)

def kaos_decrypt(cipher: KaosCipher, ciphertext: bytes, key_256bit: bytes, nonce_96bit: bytes) -> Optional[bytes]:
    """Decrypt ciphertext using KAOS cipher"""
    return cipher.decrypt(ciphertext, key_256bit, nonce_96bit)
