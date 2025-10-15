# KAOS Cipher - Visualization

Interactive 3D visualization of the KAOS cipher's chaotic system based on the Lorenz attractor.  
Generates real-time animations of the cryptographic process.

## Features

- **3D Lorenz Attractor Visualization** - Real-time rendering of chaotic trajectories
- **Interactive Controls** - Speed adjustment, camera presets, and trail length
- **Cryptographic Integration** - Shows actual keystream generation process
- **High-Density Sampling** - Captures 100% of chaotic system points
- **Professional Interface** - Real-time statistics and phase information

## Usage

# Compile the visualizer
`make`
# Generate visualization HTML
`make generate`
Output: `animation.html` - Self-contained interactive 3D visualization

Technical Details

- Three.js WebGL rendering for high-performance 3D graphics
- Real KAOS cipher data - Uses actual cryptographic computations
- 2000+ trajectory points - High-density sampling of chaotic system
- Smooth animations - Fractional speed control with accumulation
- Multiple viewing modes - Top, side, and orbital camera presets

---
Visualization Components

- Trajectory Trail - Green path showing chaotic system evolution
- Phase Markers - Warmup (red) vs Encryption (green) phases
- Real-time Stats - Position, step count, keystream bytes, FPS
- Interactive Controls - Play/pause, restart, loop, speed adjustment

Part of the KAOS Cipher project - Visualizing chaotic cryptography in action
