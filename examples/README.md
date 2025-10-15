# KAOS Cipher - Examples

Practical usage examples demonstrating the KAOS-256 chaotic stream cipher for real-world applications.

## Examples Included

### Text Encryption (`example_text.c`)
- **In-memory string encryption/decryption**
- **Hexadecimal and text output** for verification
- **Complete encryption cycle** demonstration
- **Security features showcase**

### File Encryption (`example_file.c`)
- **Complete file protection** system
- **Key management** with file-based storage
- **Progress indicators** for large files
- **End-to-end encryption/decryption**

## Usage

## Compile all examples
`make all`

## Run text encryption example
`make test`

## Run file encryption example  
`make run-file`
## Text Encryption Example
`make test`
## Direct execution
`./text_example`

# Demonstrates:
- 256-bit key generation
- 96-bit nonce usage
- In-memory encryption
- Complete cycle verification
- Security features display

# Encrypt a file
`./file_example document.pdf`

# Process:

- Input: document.pdf
- Output: encrypted.kaos (encrypted file)
- Output: decrypted_file (restored file) 
- Output: encryption_key.bin (key file - KEEP SAFE)

## Key Features Demonstrated

- Raw 256-bit keys - Professional cryptographic parameters
- Chaotic encryption - Lorenz attractor-based security
- File handling - Complete I/O operations with error checking
- Key management - Secure storage and retrieval
- Progress tracking - Visual feedback for large operations

## Security Notes
- Key files contain sensitive data - Store securely
- Different nonce for each encryption - Prevents replay attacks
- Perfect encryption cycle - Lossless encryption/decryption
- Chaotic properties - Built-in avalanche effect and diffusion

---
Part of the KAOS Cipher project - Practical chaotic cryptography examples
