//
// Created by naben on 18/11/24.
//
#ifndef ENCODING_LOSSLESS_HPP
#define ENCODING_LOSSLESS_HPP

#include <vector>
#include <utility>
#include "utils.h"

namespace encoding::lossless {

    template<typename D>
    struct CompressRepeating {

        // encodage
        static std::pair<std::vector<std::pair<D, std::size_t>>, std::size_t>
        encode(const std::vector<D>& source, const D& repeating) {
            std::vector<std::pair<D, std::size_t>> encoded;
            std::size_t count_repeating = 0;

            for (const auto& element : source) {
                if (element == repeating) {
                    ++count_repeating;
                } else {
                    // enregistre la donnée différente de 'repeating' et le nombre d'occurrences précédentes de 'repeating'
                    encoded.emplace_back(element, count_repeating);
                    count_repeating = 0;
                }
            }

            // occurrences de 'repeating' à la fin du vecteur
            std::size_t count_end = count_repeating;

            return std::make_pair(encoded, count_end);
        }

        // décodage
        static std::vector<D> decode(
            const std::vector<std::pair<D, std::size_t>>& encoded,
            const std::size_t count_end,
            const D& repeating) {

            std::vector<D> decoded;

            for (const auto& pair : encoded) {
                decoded.insert(decoded.end(), pair.second, repeating);
                decoded.push_back(pair.first);
            }

            // ajoute 'repeating' à la fin
            decoded.insert(decoded.end(), count_end, repeating);

            return decoded;
        }
    };

    //Huffman

    template<typename D>
    struct Huffman {

        std::pair<WeightedBinaryTree<std::size_t, D>, std::vector<std::vector<bool>>> encode(std::vector<D> const & source, bool add_dummy) {
           // Compter les occurrences de chaque donnée dans source
           std::unordered_map<D, std::size_t> frequency_map;
           for (const auto& data : source) {
             frequency_map[data]++;
           }

          // un vecteur de feuilles
          std::vector<WeightedBinaryTree<std::size_t, D>> trees;
          for (const auto& [data, frequency] : frequency_map) {
              trees.push_back(WeightedBinaryTree<std::size_t, D>::leaf(frequency, data));
          }

          //  l'arbre de Huffman
          while (trees.size() > 1) {
              // Trier le vecteur dans l'ordre croissant des poids (les deux éléments les plus petits à la fin)
              std::sort(trees.begin(), trees.end(), [](const WeightedBinaryTree<std::size_t, D>& a, const WeightedBinaryTree<std::size_t, D>& b) {
                  return a.get_weight() > b.get_weight();
              });

              // ici je retire les deux arbres de poids minimum
              auto left = new WeightedBinaryTree<std::size_t, D>(trees.back());
              trees.pop_back();
              auto right = new WeightedBinaryTree<std::size_t, D>(trees.back());
              trees.pop_back();

              //  un nouvel arbre complexe à partir de ces deux arbres
              auto combined_tree = WeightedBinaryTree<std::size_t, D>::complex(left, right);

              // insértion du nouvel arbre dans le vecteur de manière triée
              trees.push_back(combined_tree);
          }

             // L'arbre final est le seul arbre restant dans le vecteur
          WeightedBinaryTree<std::size_t, D> huffman_tree = trees.front();

         // Ajout d un nœud factice si add_dummy est vrai
         if (add_dummy) {
            huffman_tree.add_dummy();
         }

         // un mapping associant chaque donnée à son code
         std::unordered_map<D, std::vector<bool>> encoding_map;
         std::vector<bool> path;
         generate_codes(huffman_tree, path, encoding_map);

         // le résultat de l  encodage
         std::vector<std::vector<bool>> encoded_data;
         for (const auto& data : source) {
            encoded_data.push_back(encoding_map[data]);
         }

         //  la paire de l'arbre de Huffman et des données encodées
         return {huffman_tree, encoded_data};
    }

    // Fonction récursive pour générer les codes binaires à partir de l'arbre de Huffman
    void generate_codes(const WeightedBinaryTree<std::size_t, D>& tree, std::vector<bool>& path, std::unordered_map<D, std::vector<bool>>& encoding_map) {
        if (tree.isLeaf()) {
            // si on est sur une feuille associ le chemin à la donnée
            encoding_map[tree.get_data()] = path;
        } else {
            // Sinon, explorer les sous-arbres gauche et droit
            if (tree.isComplex()) {
                path.push_back(false); // vers la gauche
                generate_codes(tree.get_left(), path, encoding_map);
                path.pop_back();

                path.push_back(true); // vers la droite
                generate_codes(tree.get_right(), path, encoding_map);
                path.pop_back();
            }
        }
    }

    // Fonction pour décoder les données encodées
    static std::vector<D> decode(WeightedBinaryTree<std::size_t, D> const & tree, std::vector<bool> const & encoded) {
        std::vector<D> result;

        // parcourir le vecteur encoded pour suivre les instructions de déplacement dans l'arbre
        const WeightedBinaryTree<std::size_t, D>* current_node = &tree;

        for (bool direction : encoded) {
            // je dois partie  vers le sous-arbre de gauche ou de droite selon la direction
            if (direction == false) {
                current_node = &current_node->get_left();
            } else {
                current_node = &current_node->get_right();
            }

            // Si on atteint une feuille on  ajoute la donnee au vecteur resultat
            if (current_node->isLeaf()) {
                result.push_back(current_node->get_data());
                // Repartir de la racine après avoir ajouté la donnée
                current_node = &tree;
            }
        }

        return result;
    }

    };

        // LZ77

    template<typename D>
    struct LZ77 {

        static std::vector<std::variant<D, std::pair<std::size_t, std::size_t> > >
        encode(const std::vector<D>& source, const D& unit, std::size_t const buffer_size, std::size_t const chunk_size) {
                // 5.1
            assert(chunk_size < buffer_size);

                // 5.2
            std::vector<D> windows(buffer_size - chunk_size, unit);
            windows.insert(windows.end(), source.begin(), source.begin() + chunk_size);

            std::size_t current = chunk_size;

                // 5.3
            std::vector<std::variant<D, std::pair<std::size_t, std::size_t> > > encoded;
            for (std::size_t i = 0; i < chunk_size; ++i) {
                encoded.push_back(source[i]);
            }

                // 5.4
            while (current < source.size()) {
                    // 5.4.a
                std::size_t end = std::min(source.size(), current + chunk_size);
                std::vector<D> match(source.begin() + current, source.begin() + end);

                auto match_result = find_match(windows, match);
                std::size_t length = 1;

                if (!match_result.has_value()) {
                        // 5.4.b
                    encoded.push_back(source[current]);
                }

                else {
                        // 5.4.c
                    encoded.push_back(match_result.value());
                    length = match_result.value().second;
                }

                    // 5.4.d
                std::vector<D> shift_source(source.begin() + current, source.begin() + current + length);
                vector_shift(shift_source, windows, length);

                    // 5.4.e
                current += length;
            }

            return encoded;
        }

            // 6
        static std::vector<D> decode(
        const std::vector<std::variant<D, std::pair<std::size_t, std::size_t> > >& encoded,
        const D& unit, std::size_t const buffer_size, std::size_t const chunk_size
        ) {
                // initialisation de windows avec des copies de 'unit'
            std::vector<D> windows(buffer_size - chunk_size, unit);
            std::vector<D> result;

                // chunk_size premières données
            for (std::size_t i = 0; i < chunk_size; ++i) {
                D value = std::get<D>(encoded[i]);
                result.push_back(value);
                windows.push_back(value);
            }

                // reste des données encodées
            for (std::size_t idx = chunk_size; idx < encoded.size(); ++idx) {
                const auto& item = encoded[idx];
                if (std::holds_alternative<D>(item)) {
                    D value = std::get<D>(item);
                    result.push_back(value);
                    windows.push_back(value);

                    if (windows.size() > buffer_size) {
                        windows.erase(windows.begin(), windows.begin() + (windows.size() - buffer_size));
                    }
                }
                else {
                        // cas ou paire
                    auto [offset, length] = std::get<std::pair<std::size_t, std::size_t> >(item);

                    for (std::size_t i = 0; i < length; ++i) {
                        D value = windows[offset + i];
                        result.push_back(value);
                        windows.push_back(value);

                        if (windows.size() > buffer_size) {
                            windows.erase(windows.begin(), windows.begin() + (windows.size() - buffer_size));
                        }
                    }
                }
            }

            return result;
        }

    };
}
#endif // ENCODING_LOSSLESS_HPP
