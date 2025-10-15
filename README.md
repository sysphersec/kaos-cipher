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
- **Transparent Design**: Mathematical elegance with verifiable security

## 🔬 Cryptographic Properties

| Metric | Value | Assessment |
|--------|-------|------------|
| Shannon Entropy | 7.999808 bits/byte | Excellent |
| Avalanche Effect | 50.14% | Near-perfect |
| Average Correlation | 0.000787 | Excellent |
| NIST Tests Passed | 13/15 | Strong |
| Key Sensitivity | 90.0% | High |

## 🛠️ Compilation

```bash
# Compile main cipher
gcc -O3 -o kaos src/kaos.c -lm

# Compile file encryption module  
gcc -O3 -o kaos_file src/kaos_file.c -lm

# Compile test suite
gcc -O3 -o test_suite src/test_suite.c -lm
```

### Basic Usage
```C++

#include "kaos.h"

KaosCipher cipher;
kaos_init_default(&cipher);

const char* message = "Hello, World!";
const char* password = "MySecurePassword123";

// Encrypt
uint8_t* encrypted = kaos_encrypt(&cipher, (uint8_t*)message,
strlen(message), password);

// Decrypt
uint8_t* decrypted = kaos_decrypt(&cipher, encrypted,
strlen(message), password);
```

## 🔒 File Encryption

```bash
# Encrypt file
./kaos_file encrypt document.txt document.kaos "Password123"

# Decrypt file
./kaos_file decrypt document.kaos output.txt "Password123"
```
## 📄 Academic Papers

- **[English Paper](papers/kaos_paper_english.pdf)** - Complete academic paper
- **[Spanish Paper](papers/kaos_paper_spanish.pdf)** - Paper en español
- **IACR ePrint**: *Coming soon*

## 🔬 Research Integrity

This project emphasizes verifiable results and research transparency:
- All cryptographic tests are reproducible
- Performance data is empirically measured
- Multi-platform verification completed
- Open methodology and implementation

## 🤝 Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for discussions.

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- Edward Lorenz for the chaotic system discovery
- NIST for statistical test suites
- Cryptography research community

## 📞 Contact

**Simón Matías Guiñazú** - Independent Researcher  
GitHub: [@sysphersec](https://github.com/sysphersec)  
ORCID: [0009-0006-3572-9143](https://orcid.org/0009-0006-3572-9143)

---

**⭐ Repository currently private pending academic publication**
