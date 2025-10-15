from kaos import KaosCipher
import os

# Initialize cipher
cipher = KaosCipher()

# Generate key and nonce
key = os.urandom(32)  # 256 bits
nonce = os.urandom(12)  # 96 bits

# Encrypt
plaintext = b"Secret message"
ciphertext = cipher.encrypt(plaintext, key, nonce)

# Decrypt
decrypted = cipher.decrypt(ciphertext, key, nonce)

print(f"Original: {plaintext}")
print(f"Decrypted: {decrypted}")
assert plaintext == decrypted