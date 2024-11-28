# Data Compression Project

This project focuses on implementing both lossless and lossy data compression algorithms in C++. It includes well-known algorithms such as Huffman, LZ77, Discrete Fourier Transform, Discrete Cosine Transform, and Quantization. The project aims to provide a versatile compression suite to demonstrate encoding and decoding techniques that help in data reduction.

## Project Overview

The project includes two main namespaces:

- **encoding::lossless**: Contains implementations for lossless data compression techniques like Huffman, LZ77, and custom run-length encoding (CompressRepeating).
- **encoding::lossly**: Contains implementations for lossy data compression techniques, including Discrete Cosine Transform, Quantization, and Discrete Fourier Transform.

### Project Files

1. **encoding_lossless.h**: Contains definitions for various lossless compression algorithms:
    - **CompressRepeating**: Custom run-length encoding for repeating symbols.
    - **Huffman**: A classic Huffman coding implementation for efficient data compression.
    - **LZ77**: Implements the LZ77 algorithm for dictionary-based compression.

2. **encoding_lossly.h**: Contains definitions for lossy compression algorithms:
    - **DiscreteCosinus**: Implements the Discrete Cosine Transform (DCT) for compressing data with minimal loss.
    - **Quantization**: Performs scalar quantization of the data for reducing precision with configurable quantum levels.
    - **DiscreteFourier**: Implements the Discrete Fourier Transform (DFT) to encode and decode signals in the frequency domain.

3. **utils.h**: Contains utility functions and a generic implementation of **WeightedBinaryTree** used in the Huffman encoding process.
    - Also includes helper functions like **find_match** (for LZ77) and **insert_in_isorted** (for maintaining sorted lists).

4. **main.cpp**: Contains test cases for each of the implemented algorithms to validate the encoding and decoding processes.

### Compression Algorithms

- **CompressRepeating** (Run-Length Encoding):
    - Used for compressing repeating elements in a sequence.
    - Efficient for data that contains long runs of identical values.

- **Huffman Encoding**:
    - Uses a frequency-based binary tree to encode symbols with variable-length codes.
    - Ensures that more frequent symbols use shorter codes to achieve compression.

- **LZ77**:
    - Employs a sliding window technique to find and encode repeating patterns.
    - Suitable for compressing large sequences by using references to previously seen data.

- **Discrete Cosine Transform (DCT)**:
    - Transforms spatial data to frequency domain for efficient representation.
    - Typically used in lossy image compression to retain significant information while discarding less noticeable details.

- **Quantization**:
    - Reduces data precision by dividing it into quantum levels.
    - Especially effective in conjunction with DCT to reduce the size of encoded data.

- **Discrete Fourier Transform (DFT)**:
    - Converts data from the time domain to the frequency domain.
    - The resulting frequency components are used for compression, typically by discarding less significant frequencies.

### Usage

To compile the project, use the following command:

```bash
# Assuming you have a C++ compiler supporting C++17
g++ -std=c++17 main.cpp -o compression -lm
```

To run the tests provided in `main.cpp`:

```bash
./compression
```

### How to Contribute

If you want to contribute to this project, you can do the following:

- Fork the repository on GitHub.
- Make your changes or add new features.
- Create a pull request, and explain the changes you made.

