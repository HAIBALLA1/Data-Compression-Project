//
// Created by ing on 08/11/24.
//
#include "encoding_lossly.h"
#include <iostream>
#include "encoding_lossless.h"

#include <iomanip>// pour  setprecision

#include "utils.h"

int main() {
    using namespace encoding::lossly;

    std::vector<int>source ={1,2,3,4,5};
    std::size_t nb_coefs=4;
    std::vector<double> resu=DiscreteCosinus::encode<int, double>(source, nb_coefs);;
    std::cout << "Résultat de l'encodage :" << std::endl;
    for (std::size_t i = 0; i < resu.size(); ++i) {
        std::cout << "Coefficient " << i << " : " << resu[i] << std::endl;
    }

    std::vector<double> data = {10.0, 20.5, 30.2, 40.8};
    double quantum = 5.0;

    // Appel de la méthode encode
    std::vector<int> encoded_data = quantization::encode<double>(data, quantum);

    // Affichage des résultats encodés
    std::cout << "Données encodées : ";
    for (int val : encoded_data) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Appel de la méthode decode
    std::vector<double> decoded_data = quantization::decode<double>(encoded_data, quantum);

    // Affichage des résultats décodés
    std::cout << "Données décodées : ";
    for (double val : decoded_data) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Définition de la fonction de quantum dynamique
    quantization::get_quantum_t get_quantum = [](std::size_t i) {
        return 2.0 + i * 0.5;  // Exemple de quantum qui augmente à chaque élément
    };

    // Test de l'encodage et du décodage avec quantum dynamique
    std::vector<int> encoded_data_dynamic = quantization::encode(data, get_quantum);
    std::vector<double> decoded_data_dynamic = quantization::decode<double>(encoded_data_dynamic, get_quantum);

    // Affichage des résultats
    std::cout << "Encodage avec quantum dynamique:" << std::endl;
    for (auto val : encoded_data_dynamic) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    std::cout << "Décodage avec quantum dynamique:" << std::endl;
    for (auto val : decoded_data_dynamic) {
        std::cout << val << " ";
    }
    std::cout << std::endl;


    //teste de la partie DiscreteFourier

    std::vector<double> sources = {1.0, 2.0, 3.0, 4.0};


    std::cout << "Signal source : ";
    for (const auto& val : sources) {
        std::cout << val << " ";
    }
    std::cout << "\n\n";

    auto encoded = encoding::lossly::DiscreteFourier::encode<double, double>(sources, nb_coefs);

    std::cout << "Coefficients encodés (Partie réelle et imaginaire) :\n";
    std::cout << std::fixed << std::setprecision(6);
    for (std::size_t i = 0; i < nb_coefs; ++i) {
        std::cout << "Coefficient " << i << " : Réel = " << encoded.first[i]
                  << ", Imaginaire = " << encoded.second[i] << "\n";
    }
    std::cout << "\n";

    std::vector<double> decoded =DiscreteFourier::decode<double, double>(encoded, sources.size());

    std::cout << "Signal décodé : ";
    for (const auto& val : decoded) {
        std::cout << val << " ";
    }
    std::cout << "\n\n";

    std::cout << "Différences entre le signal source et le signal décodé :\n";
    for (std::size_t i = 0; i < sources.size(); ++i) {
        double difference = std::abs(sources[i] - decoded[i]);
        std::cout << "Indice " << i << " : Source = " << sources[i]
                  << ", Décodé = " << decoded[i]
                  << ", Différence = " << difference << "\n";
    }

    WeightedBinaryTree<int , int > tree= WeightedBinaryTree<int , int>::leaf(3,5) ;
    encoding::lossless::Huffman<double> huffman;

    return 0;
}
