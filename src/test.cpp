#include <iostream>
#include <vector>
#include <variant>
#include "encoding_lossy.hpp"      // <-- orthographe corrigée + .hpp
#include "encoding_lossless.hpp"   // <-- .hpp
#include "utils.hpp"               // <-- .hpp

int main() {
    using namespace encoding::lossy;
    using namespace encoding::lossless;

    // ===== Discrete Cosinus =====
    std::cout << "=== Test Discrete Cosinus ===\n";
    std::vector<double> source_cos = {1.0, 2.0, 3.0, 4.0};
    auto encoded_cos  = DiscreteCosinus::encode<double, double>(source_cos, 2);
    auto decoded_cos  = DiscreteCosinus::decode<double, double>(encoded_cos, source_cos.size());

    std::cout << "Encoded (DCT): ";
    for (const auto& v : encoded_cos) std::cout << v << " ";
    std::cout << "\nDecoded (DCT): ";
    for (const auto& v : decoded_cos) std::cout << v << " ";
    std::cout << "\n\n";

    // ===== Quantization =====
    std::cout << "=== Test Quantization ===\n";
    double quantum = 0.5;
    auto quantized    = Quantization::encode<double>(source_cos, quantum);
    auto dequantized  = Quantization::decode<double>(quantized, quantum);

    std::cout << "Quantized: ";
    for (const auto& v : quantized) std::cout << v << " ";
    std::cout << "\nDequantized: ";
    for (const auto& v : dequantized) std::cout << v << " ";
    std::cout << "\n\n";

    // ===== Discrete Fourier =====
    std::cout << "=== Test Discrete Fourier ===\n";
    auto [f_real, f_imag] = DiscreteFourier::encode<double, double>(source_cos, 2);
    auto decoded_fourier  = DiscreteFourier::decode<double, double>({f_real, f_imag}, source_cos.size());

    std::cout << "Encoded Fourier (Real): ";
    for (const auto& v : f_real) std::cout << v << " ";
    std::cout << "\nEncoded Fourier (Imag): ";
    for (const auto& v : f_imag) std::cout << v << " ";
    std::cout << "\nDecoded Fourier: ";
    for (const auto& v : decoded_fourier) std::cout << v << " ";
    std::cout << "\n\n";

    // ===== Huffman =====
    std::cout << "=== Test Huffman Encoding ===\n";
    std::vector<char> source_huffman = {'a','b','a','c','b','a'};
    auto [huff_tree, huff_codes] = Huffman<char>::encode(source_huffman, false);

    // aplatir pour tester le décodeur
    std::vector<bool> bits;
    for (const auto& code : huff_codes) bits.insert(bits.end(), code.begin(), code.end());
    auto decoded_huffman = Huffman<char>::decode(huff_tree, bits);

    std::cout << "Encoded Huffman: ";
    for (const auto& code : huff_codes) {
        for (bool b : code) std::cout << (b ? '1' : '0');
        std::cout << " ";
    }
    std::cout << "\nDecoded Huffman: ";
    for (char c : decoded_huffman) std::cout << c << " ";
    std::cout << "\n\n";

    // ===== LZ77 =====
    std::cout << "=== Test LZ77 Encoding ===\n";
    std::vector<char> src_lz77 = {'a','b','a','b','a','b','c'};
    std::size_t buffer_size = 5;
    std::size_t chunk_size  = 3;

    // NB: nouvelle signature sans 'unit'
    auto enc_lz77 = LZ77<char>::encode(src_lz77, buffer_size, chunk_size);
    auto dec_lz77 = LZ77<char>::decode(enc_lz77, buffer_size);

    std::cout << "Encoded LZ77: ";
    for (const auto& t : enc_lz77) {
        if (std::holds_alternative<char>(t)) {
            std::cout << std::get<char>(t) << " ";
        } else {
            auto [off, len] = std::get<std::pair<std::size_t,std::size_t>>(t);
            std::cout << "[" << off << "," << len << "] ";
        }
    }
    std::cout << "\nDecoded LZ77: ";
    for (char c : dec_lz77) std::cout << c << " ";
    std::cout << "\n";

    return 0;
}

