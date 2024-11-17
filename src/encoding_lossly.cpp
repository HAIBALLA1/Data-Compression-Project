#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include "encoding_lossly.h"

int main() {
    std::vector<double> source = {1.0, 2.0, 3.0, 4.0};
    std::size_t nb_coefs = source.size();

    std::cout << "Signal source : ";
    for (const auto& val : source) {
        std::cout << val << " ";
    }
    std::cout << "\n\n";

    auto encoded = encoding::lossly::DiscreteFourier::encode<double, double>(source, nb_coefs);

    std::cout << "Coefficients encodés (Partie réelle et imaginaire) :\n";
    std::cout << std::fixed << std::setprecision(6);
    for (std::size_t i = 0; i < nb_coefs; ++i) {
        std::cout << "Coefficient " << i << " : Réel = " << encoded.first[i]
                  << ", Imaginaire = " << encoded.second[i] << "\n";
    }
    std::cout << "\n";

    std::vector<double> decoded = encoding::lossly::DiscreteFourier::decode<double, double>(encoded, source.size());

    std::cout << "Signal décodé : ";
    for (const auto& val : decoded) {
        std::cout << val << " ";
    }
    std::cout << "\n\n";

    std::cout << "Différences entre le signal source et le signal décodé :\n";
    for (std::size_t i = 0; i < source.size(); ++i) {
        double difference = std::abs(source[i] - decoded[i]);
        std::cout << "Indice " << i << " : Source = " << source[i]
                  << ", Décodé = " << decoded[i]
                  << ", Différence = " << difference << "\n";
    }

    return 0;
}

