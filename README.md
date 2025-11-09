# Data Compression Project — Modern C++20

This project implements a suite of **lossless** and **lossy** data compression algorithms in **modern C++20**, emphasizing clarity, generic programming, and computational reasoning. 
It demonstrates practical compression techniques used in systems such as audio/video codecs, storage systems, and communication pipelines.

---

## Features Overview

| Category | Algorithms | Description |
|---------|------------|-------------|
| **Lossless Compression** | **Huffman**, **LZ77**, **CompressRepeating (Run-Length Encoding)** | Reversible compression preserving the original data exactly. |
| **Lossy Compression** | **DCT (Discrete Cosine Transform)**, **DFT (Discrete Fourier Transform)**, **Quantization** | Irreversible compression where less important information is reduced or approximated. |
| **Utilities** | `WeightedBinaryTree`, `find_match`, `vector_shift` | Shared structures and helpers for Huffman and LZ77 implementations. |

---

## Folder Structure
src/
├── encoding_lossless.hpp # Huffman, LZ77, RLE
├── encoding_lossy.hpp # DCT, DFT, Quantization
├── utils.hpp # WeightedBinaryTree + helper algorithms
└── test.cpp # Demonstration / verification program
CMakeLists.txt # C++20 project configuration


---

## Requirements

- **GCC ≥ 12** or **Clang ≥ 14**
- **CMake ≥ 3.16**
- Linux, macOS or WSL environment recommended

---

## Build & Run

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
./build/dc_test


Algorithm Highlights
Huffman Coding

Builds a weighted binary tree to assign shorter codes to frequent symbols, achieving effective compression.

LZ77 Sliding Window

Uses a backward reference buffer to encode repeated patterns as (offset, length) pairs instead of literal data.

Discrete Cosine Transform (DCT)

Transforms spatial data into frequency space. Low-frequency components carry most significance — ideal for image compression.

Quantization

Reduces numeric precision into discrete quantum steps. Used in JPEG, MP3, video compression.

Discrete Fourier Transform (DFT)

Represents data as sinusoidal components. Useful for analyzing periodic or oscillatory signals.
