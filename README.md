# KAOS Cipher 🔐

A chaotic stream cipher based on Lorenz attractor with comprehensive cryptographic validation.

![Cryptography](https://img.shields.io/badge/Cryptography-Stream%20Cipher-blue)
![License](https://img.shields.io/badge/License-MIT-green)
![Platform](https://img.shields.io/badge/Platform-C%20%2F%20C++-yellow)
![Validation](https://img.shields.io/badge/Validation-ENT%2FDieharder%2FNIST-success)

## 📖 Overview

KAOS Cipher is a novel stream cipher that leverages the mathematical properties of the Lorenz chaotic system to generate cryptographically secure pseudorandom sequences. The design combines chaotic dynamics with non-linear post-processing to achieve exceptional statistical properties validated through industry-standard test suites.

## ✨ Features

- **Chaotic Foundation**: Based on Lorenz attractor with proven chaotic properties
- **Professional Implementation**: 256-bit keys + 96-bit nonces, raw cryptographic keys
- **Comprehensive Validation**: Triple validation with ENT, Dieharder, and NIST STS
- **Mathematical Transparency**: Verifiable security based on established chaotic dynamics

## 🔬 Cryptographic Properties

| Metric | Value | Assessment |
|--------|-------|------------|
| Shannon Entropy | 7.999998 bits/byte | Near-perfect |
| Avalanche Effect | 50.27% | Ideal |
| Average Correlation | 0.000739 | Excellent |
| Dieharder Tests Passed | 111/114 (97.4%) | Excellent |
| NIST STS Pass Rate | 99%+ | Validated |
| Performance | 50-70 MB/s | Practical |

### Basic Usage
```C
#include "kaos.h"

KaosCipher cipher;
kaos_init(&cipher);

uint8_t key[32];      // 256-bit key
uint8_t nonce[12];    // 96-bit nonce
const char* message = "Hello, World!";

// Encrypt
uint8_t* encrypted = kaos_encrypt(&cipher, (uint8_t*)message,
strlen(message), key, nonce);

// Decrypt
uint8_t* decrypted = kaos_decrypt(&cipher, encrypted,
strlen(message), key, nonce);
```
📊 Validation Results

### ENT Analysis
- Entropy: 7.999998 bits/byte (near perfect)
- Chi-square: 235.38 (80.58% random)
- Arithmetic Mean: 127.4995 (ideal: 127.5)
- Serial Correlation: -0.000036 (near zero)
- Dieharder Suite (111/114 passed)
- Birthdays, Operm5, Rank Tests: 100% passed
- Count Tests, Parking Lot/Spheres: 100% passed
- RGB Tests: 57/58 (98.3%) passed
- STS Tests: 34/36 (94.4%) passed

### NIST Statistical Suite

- Frequency, Block Frequency, Cumulative Sums: PASSED
- Runs, Longest Run, Rank, FFT: PASSED
- NonOverlapping Template: 148/149 (99.3%)
- Overall Pass Rate: 99%+

🔬 Research Integrity  

This project emphasizes verifiable results and research transparency:  

- All cryptographic tests are reproducible with fixed test vectors
- Performance data is empirically measured across multiple platforms
- Complete mathematical foundation based on Lorenz chaotic system
- Open methodology and implementation for peer review

📄 Academic Papers  
English Paper - Complete academic paper with formal validation    
Spanish Paper - Paper en español    
IACR ePrint: Coming soon  

🤝 Contributing  
Contributions are welcome! Please feel free to submit pull requests or open issues for discussions.

📜 License  
This project is licensed under the MIT License - see the LICENSE file for details.

🙏 Acknowledgments  
- Edward Lorenz for the chaotic system discovery  
- NIST for statistical test suites  
- Dieharder and ENT test suite developers  
- Cryptography research community  

📞 Contact  
Simón M. Guiñazú - Independent Researcher  
GitHub: @sysphersec  
Email: sysphersec@gmail.com  
ORCID: 0009-0006-3572-9143  
