#include <iostream>
#include <vector>
#include "encoding_lossly.h"
#include "encoding_lossless.h"
#include "utils.h"

int main() {
    using namespace encoding::lossly;
    using namespace encoding::lossless;
    using namespace utils;

    // Test Discrete Cosinus
    std::cout << "=== Test Discrete Cosinus ===" << std::endl;
    std::vector<double> source_cos = {1.0, 2.0, 3.0, 4.0};
    auto encoded_cos = DiscreteCosinus::encode<double, double>(source_cos, 2);
    auto decoded_cos = DiscreteCosinus::decode<double, double>(encoded_cos, source_cos.size());
    std::cout << "Encoded (DCT): ";
    for (const auto& val : encoded_cos) std::cout << val << " ";
    std::cout << "\nDecoded (DCT): ";
    for (const auto& val : decoded_cos) std::cout << val << " ";
    std::cout << "\n\n";

    // Test Quantization
    std::cout << "=== Test Quantization ===" << std::endl;
    double quantum = 0.5;
    auto quantized = Quantization::encode(source_cos, quantum);
    auto dequantized = Quantization::decode<double>(quantized, quantum);
    std::cout << "Quantized: ";
    for (const auto& val : quantized) std::cout << val << " ";
    std::cout << "\nDequantized: ";
    for (const auto& val : dequantized) std::cout << val << " ";
    std::cout << "\n\n";

    // Test Discrete Fourier
    std::cout << "=== Test Discrete Fourier ===" << std::endl;
    auto [encoded_fourier_real, encoded_fourier_img] = DiscreteFourier::encode<double, double>(source_cos, 2);
    auto decoded_fourier = DiscreteFourier::decode<double, double>({encoded_fourier_real, encoded_fourier_img}, source_cos.size());
    std::cout << "Encoded Fourier (Real): ";
    for (const auto& val : encoded_fourier_real) std::cout << val << " ";
    std::cout << "\nEncoded Fourier (Imaginary): ";
    for (const auto& val : encoded_fourier_img) std::cout << val << " ";
    std::cout << "\nDecoded Fourier: ";
    for (const auto& val : decoded_fourier) std::cout << val << " ";
    std::cout << "\n\n";

    // Test Huffman Encoding
    std::cout << "=== Test Huffman Encoding ===" << std::endl;
    std::vector<char> source_huffman = {'a', 'b', 'a', 'c', 'b', 'a'};
    Huffman<char> huffman_encoder;
    auto [huffman_tree, encoded_huffman] = huffman_encoder.encode(source_huffman, false);

    // Aplatir le vecteur de vecteurs de bits en un seul vecteur de bits
    std::vector<bool> flattened_encoded_huffman;
    for (const auto& code : encoded_huffman) {
        flattened_encoded_huffman.insert(flattened_encoded_huffman.end(), code.begin(), code.end());
    }

    // Décodage en utilisant le vecteur aplati
    auto decoded_huffman = Huffman<char>::decode(huffman_tree, flattened_encoded_huffman);

    std::cout << "Encoded Huffman: ";
    for (const auto& code : encoded_huffman) {
        for (bool bit : code) std::cout << bit;
        std::cout << " ";
    }
    std::cout << "\nDecoded Huffman: ";
    for (const auto& val : decoded_huffman) std::cout << val << " ";
    std::cout << "\n\n";

    // Test LZ77 Encoding
    std::cout << "=== Test LZ77 Encoding ===" << std::endl;
    std::vector<char> source_lz77 = {'a', 'b', 'a', 'b', 'a', 'b', 'c'};
    char unit = 'x';
    std::size_t buffer_size = 5;
    std::size_t chunk_size = 3;
    auto encoded_lz77 = LZ77<char>::encode(source_lz77, unit, buffer_size, chunk_size);
    auto decoded_lz77 = LZ77<char>::decode(encoded_lz77, unit, buffer_size, chunk_size);
    std::cout << "Encoded LZ77: ";
    for (const auto& val : encoded_lz77) {
        if (std::holds_alternative<char>(val)) {
            std::cout << std::get<char>(val) << " ";
        } else {
            auto [offset, length] = std::get<std::pair<std::size_t, std::size_t>>(val);
            std::cout << "[" << offset << "," << length << "] ";
        }
    }
    std::cout << "\nDecoded LZ77: ";
    for (const auto& val : decoded_lz77) std::cout << val << " ";
    std::cout << "\n\n";

    return 0;
}
