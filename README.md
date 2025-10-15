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

## 🔧  Basic Usage
```c
#include "kaos.h"
#include <string.h>

int main() {
    KaosCipher cipher;
    kaos_init(&cipher);
    
    uint8_t key[32] = {0};      // 256-bit key
    uint8_t nonce[12] = {0};    // 96-bit nonce
    char message[] = "Secret";
    
    // Encrypt & Decrypt
    uint8_t* encrypted = kaos_encrypt(&cipher, (uint8_t*)message, 
                                     strlen(message), key, nonce);
    uint8_t* decrypted = kaos_decrypt(&cipher, encrypted, 
                                     strlen(message), key, nonce);
    
    free(encrypted);
    free(decrypted);
    return 0;
}
```
Compile: `gcc -o example example.c ../src/kaos.c -I../src -lm`   

## 📊 Cryptographic Validation

### ENT Test Suite Results
| Metric | Value | Assessment |
|--------|-------|------------|
| Shannon Entropy | 7.999998 bits/byte | Near-perfect |
| Chi-square | 235.38 | 80.58% random |
| Arithmetic Mean | 127.4995 | Ideal (127.5) |
| Serial Correlation | -0.000036 | Near-zero |

### Dieharder Battery (111/114 passed)
| Test Category | Passed | Rate |
|---------------|--------|------|
| Birthdays, Operm5, Rank | 4/4 | 100% |
| Count Tests, Parking Lot | 5/5 | 100% |
| RGB Tests | 57/58 | 98.3% |
| STS Tests | 34/36 | 94.4% |

### NIST Statistical Test Suite

| Test Category | Results | Pass Rate |
|---------------|---------|-----------|
| Core Tests | Frequency, Block Frequency, Cumulative Sums | 100% |
| Advanced Tests | Runs, Longest Run, Rank, FFT | 100% |
| NonOverlapping Template | 148/149 tests passed | 99.3% |
| Overall Suite | All required tests | 99%+ |
---

## 🔬 Research & Integrity

- **Verifiable Results**: All tests reproducible with fixed vectors  
- **Mathematical Foundation**: Based on established Lorenz chaotic system  
- **Open Methodology**: Complete transparency for peer review  

---

## 📚 Academic Papers

- **[English Paper](Paper/document.pdf)** - Complete formal validation
- **[Spanish Paper](Paper/document_es.pdf)** - Paper en español
- **IACR ePrint**: *Coming soon*

---

## 🤝 Contributing

We welcome contributions! Feel free to:
- Submit pull requests
- Open issues for discussion
- Suggest improvements or tests

## 📜 License

MIT Licensed - See [LICENSE](LICENSE) for details.

## 🙏 Acknowledgments

- **Edward Lorenz** - Chaotic system discovery
- **Test Suite Developers** - NIST STS, Dieharder, ENT teams  
- **Cryptography Research Community** - Ongoing advancements

## 📞 Contact

**Simón M. Guiñazú** - Independent Researcher  
📧 Email: sysphersec@gmail.com  
🔐 PGP: `5E5B 6B66 F095 8A24 AEA1 1CA9 854E 0825 E334 6C93`  
🔗 GitHub: [@sysphersec](https://github.com/sysphersec)  
📚 ORCID: [0009-0006-3572-9143](https://orcid.org/0009-0006-3572-9143)
