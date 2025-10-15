# KAOS Cipher - Test Suite

Comprehensive cryptographic validation suite for the KAOS-256 chaotic stream cipher.  
Internal development tool for rapid testing during implementation.

## Features

- **NIST Test Implementations** - Runs, Cumulative Sums, Longest Run, Serial tests
- **Avalanche Effect Testing** - Measures bit change sensitivity with raw 256-bit keys
- **Performance Benchmarking** - Throughput analysis with 10MB test data
- **Entropy Analysis** - Shannon entropy, min-entropy, and chi-squared statistics
- **Correlation Analysis** - 50-lag autocorrelation testing
- **Raw Key Support** - 256-bit keys + 96-bit nonces for professional testing

## Compile the test suite
`make`

## Run comprehensive tests
`make test`

## Test Coverage

## Statistical Tests

- NIST Runs Test - Sequence randomness validation
- Cumulative Sums Test - Distribution uniformity
- Longest Run Test - Maximum sequence patterns
- Serial Test - Overlapping pattern frequency

## Cryptographic Tests

- Avalanche Effect - 50+ iterations with single-bit changes
- Entropy Analysis - Shannon and min-entropy calculations
- Correlation Analysis - 50-lag autocorrelation checks
- Performance Benchmark - 10MB throughput measurement

## Technical Details

- 1MB Test Keystream - Fixed key/nonce for reproducibility
- Raw Cryptographic Keys - No key derivation, direct 256-bit testing
- Mathematical Rigor - Proper statistical distributions and p-values
- Development Focused - Quick feedback during implementation

## Important Note
This is an INTERNAL development tool. For formal cryptographic validation, use:

- NIST Statistical Test Suite
- Dieharder Battery
- ENT Test Suite
- TestU01 Battery

---
Part of the KAOS Cipher project - Internal testing suite for chaotic cryptography
