# KAOS Cipher - Keystream Generator

Cryptographic keystream generator for the KAOS-256 chaotic stream cipher.
Generates high-quality pseudorandom sequences for cryptographic validation and testing.

## Features

- 256-bit keys & 96-bit nonces - Professional cryptographic parameters
- Lorenz attractor-based - Chaotic system for unpredictability
- Reproducible output - Fixed test vectors for consistent validation
- Performance metrics - Throughput and quality analysis
- Multiple test suite ready - Output compatible with NIST STS, Dieharder, ENT

## Usage

# Compile
```bash 
make
```
# Generate keystream for testing
```bash 
./keystream_generator <size_in_bytes> <output_file>
```
### Examples

## 1MB for quick validation
```bash 
./keystream_generator 1000000 kaos_1mb.bin
```
## 100MB for NIST STS testing  
```bash 
./keystream_generator 100000000 kaos_100mb.bin
```
## 1GB for extensive statistical testing
```bash 
./keystream_generator 1000000000 kaos_1gb.bin
```
## Output Validation

The generator includes built-in quality checks:
- Bit balance analysis (target: 50%)
- Chi-squared uniformity test
- Generation throughput metrics

## Recommended Test Sizes

Test Suite | Minimum Size | Recommended Size
-----------|--------------|-----------------
ENT | 1 MB | 10 MB
Dieharder | 100 MB | 1 GB
NIST STS | 1 MB | 100 MB
TestU01 | 1 GB | 10 GB

## Integration

Uses the core KAOS library from ../src/ with fixed test parameters for reproducible cryptographic validation.

---
Part of the KAOS Cipher project - Chaotic cryptography based on Lorenz attractor
