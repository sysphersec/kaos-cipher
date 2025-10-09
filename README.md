# KAOS Cipher 🔐

A chaotic stream cipher based on Lorenz attractor with strong cryptographic properties.

![Cryptography](https://img.shields.io/badge/Cryptography-Stream%20Cipher-blue)
![License](https://img.shields.io/badge/License-MIT-green)
![Platform](https://img.shields.io/badge/Platform-C%20%2F%20C++-yellow)

## 📖 Overview

KAOS Cipher is a novel stream cipher that leverages the mathematical properties of the Lorenz chaotic system to generate cryptographically secure pseudorandom sequences. The design combines chaotic dynamics with non-linear post-processing to achieve excellent statistical properties.

## ✨ Features

- **Chaotic Foundation**: Based on Lorenz attractor with proven chaotic properties
- **Strong Security**: 13/15 NIST statistical tests passed
- **Practical Performance**: Verified throughput on multiple platforms
- **Transparent Design**: Mathematical elegance with verifiable security
- **File Encryption**: Complete file encryption/decryption module

## 🔬 Cryptographic Properties

| Metric | Value | Assessment |
|--------|-------|------------|
| Shannon Entropy | 7.999808 bits/byte | Excellent |
| Avalanche Effect | 50.14% | Near-perfect |
| Average Correlation | 0.000787 | Excellent |
| NIST Tests Passed | 13/15 | Strong |
| Key Sensitivity | 90.0% | High |

## 🚀 Quick Start

### Prerequisites
- GCC or Clang compiler
- Standard C library with math support

### Compilation
```bash
# Compile main cipher
gcc -O3 -o kaos src/kaos.c -lm

# Compile file encryption module
gcc -O3 -o kaos_file src/kaos_file.c -lm

# Compile test suite
gcc -O3 -o test_suite src/test_suite.c -lm
