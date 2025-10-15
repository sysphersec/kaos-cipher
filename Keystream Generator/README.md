# KAOS Cipher - Keystream Generator

Cryptographic keystream generator for the KAOS-256 chaotic stream cipher.  
Generates high-quality pseudorandom sequences for cryptographic validation and testing.

## Features

- **Fixed test vectors** - Reproducible 0x42 key & 0x99 nonce for consistent validation
- **Progress tracking** - Real-time generation progress for large files
- **Built-in quality checks** - Bit balance and chi-squared analysis
- **Performance metrics** - Throughput measurement and timing statistics
- **Multi-size ready** - Optimized for 1MB to 1GB+ test streams

## Usage

```bash
# Compile and generate keystream
make && ./keystream_generator <size> <output>
```
```bash
# Examples
./keystream_generator 1000000 kaos_1mb.bin        # 1MB - Quick tests
./keystream_generator 100000000 kaos_100mb.bin    # 100MB - NIST STS  
./keystream_generator 1000000000 kaos_1gb.bin     # 1GB - Extensive testing
```
## Recommended Test Sizes

Test Suite | Minimum Size | Recommended Size
-----------|--------------|-----------------
ENT | 1 MB | 10 MB
Dieharder | 100 MB | 1 GB
NIST STS | 1 MB | 100 MB
TestU01 | 1 GB | 10 GB

---
Part of the KAOS Cipher project - Chaotic cryptography based on Lorenz attractor
