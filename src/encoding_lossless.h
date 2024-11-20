
#ifndef ENCODING_LOSSLESS_H
#define ENCODING_LOSSLESS_H

#include <vector>
#include <utility>
#include "utils.h"
using namespace utils;
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

       std::pair<std::shared_ptr<WeightedBinaryTree<std::size_t, D>>, std::vector<std::vector<bool>>> encode(const std::vector<D>& source, bool add_dummy) {
           // Compter les occurrences de chaque donnée dans source
           std::unordered_map<D, std::size_t> frequency_map;
           for (const auto& data : source) {
             frequency_map[data]++;
           }

          // un vecteur de feuilles
          std::vector<std::shared_ptr<WeightedBinaryTree<std::size_t, D>>> trees;
          for (const auto& [data, frequency] : frequency_map) {
              trees.push_back(WeightedBinaryTree<std::size_t, D>::leaf(frequency, data));
          }

          //  l'arbre de Huffman
          while (trees.size() > 1) {
              // Trier le vecteur dans l'ordre croissant des poids (les deux éléments les plus petits à la fin)
              std::sort(trees.begin(), trees.end(), [](const std::shared_ptr<WeightedBinaryTree<std::size_t, D>>& a, const std::shared_ptr<WeightedBinaryTree<std::size_t, D>>& b) {
                  return a->get_weight() > b->get_weight();
              });


              // Retirez les deux arbres de poids minimum
              auto left = trees.back();
              trees.pop_back();
              auto right = trees.back();
              trees.pop_back();

              // Créez un nouvel arbre complexe
              auto combined_tree = WeightedBinaryTree<std::size_t, D>::complex(left, right);


              // insértion du nouvel arbre dans le vecteur de manière triée
              trees.push_back(combined_tree);
          }

             // L'arbre final est le seul arbre restant dans le vecteur
             auto huffman_tree = trees.front();


         // Ajout d un nœud factice si add_dummy est vrai
         if (add_dummy) {
            huffman_tree->add_dummy();
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
        void generate_codes(std::shared_ptr<WeightedBinaryTree<std::size_t, D>> tree, std::vector<bool>& path, std::unordered_map<D, std::vector<bool>>& encoding_map) {
            if (tree->is_leaf()) {
                encoding_map[tree->get_data()] = path;
            } else {
                // Explorer le sous-arbre gauche
                if (tree->get_left()) {
                    path.push_back(false);
                    generate_codes(tree->get_left(), path, encoding_map);
                    path.pop_back();
                }
                // Explorer le sous-arbre droit
                if (tree->get_right()) {
                    path.push_back(true);
                    generate_codes(tree->get_right(), path, encoding_map);
                    path.pop_back();
                }
            }
        }

    // Fonction pour décoder les données encodées
    static std::vector<D> decode(std::shared_ptr<WeightedBinaryTree<std::size_t, D>> tree, const std::vector<bool>& encoded) {
        std::vector<D> result;

        // parcourir le vecteur encoded pour suivre les instructions de déplacement dans l'arbre
            auto current_node = tree; // Si tree est déjà un std::shared_ptr

            for (bool direction : encoded) {
                if (direction == false) {
                    current_node = current_node->get_left();
                } else {
                    current_node = current_node->get_right();
                }

                if (current_node->is_leaf()) {
                    result.push_back(current_node->get_data());
                    current_node = tree;
                }
            }


        return result;
    }

    };

        // LZ77

    template<typename D>
    struct LZ77 {
        // Fonction d'encodage LZ77
        static std::vector<std::variant<D, std::pair<std::size_t, std::size_t>>>
        encode(const std::vector<D>& source, const D& unit, std::size_t buffer_size, std::size_t chunk_size) {
            assert(chunk_size < buffer_size);

            std::vector<std::variant<D, std::pair<std::size_t, std::size_t>>> encoded;
            std::size_t current = 0;
            std::size_t source_size = source.size();

            while (current < source_size) {
                // Déterminer les limites du buffer de recherche
                std::size_t search_buffer_start = (current >= buffer_size) ? (current - buffer_size) : 0;
                std::vector<D> search_buffer(source.begin() + search_buffer_start, source.begin() + current);
                // Obtenir le buffer de prélecture
                std::vector<D> lookahead_buffer(source.begin() + current, source.begin() + std::min(current + chunk_size, source_size));

                // Trouver la meilleure correspondance
                auto match_result = find_match(search_buffer, lookahead_buffer);

                if (match_result.has_value() && match_result->second > 0) {
                    // Correspondance trouvée
                    encoded.push_back(match_result.value());
                    current += match_result->second;
                } else {
                    // Aucune correspondance, encoder le symbole directement
                    encoded.push_back(source[current]);
                    ++current;
                }
            }

            return encoded;
        }


        // Fonction de décodage LZ77
        static std::vector<D> decode(
            const std::vector<std::variant<D, std::pair<std::size_t, std::size_t>>>& encoded,
            const D& unit, std::size_t buffer_size, std::size_t chunk_size
        ) {
            std::vector<D> result;
            std::vector<D> window;

            for (const auto& item : encoded) {
                if (std::holds_alternative<D>(item)) {
                    // Symbole encodé directement
                    D value = std::get<D>(item);
                    result.push_back(value);
                    window.push_back(value);
                } else {
                    // Paire (offset, length)
                    auto [offset, length] = std::get<std::pair<std::size_t, std::size_t>>(item);
                    std::size_t window_size = window.size();
                    std::size_t start_index = window_size - offset;

                    for (std::size_t i = 0; i < length; ++i) {
                        D value = window[start_index + i];
                        result.push_back(value);
                        window.push_back(value);
                    }
                }

                // Maintenir la taille de la fenêtre
                if (window.size() > buffer_size) {
                    window.erase(window.begin(), window.begin() + (window.size() - buffer_size));
                }
            }

            return result;
        }

    };
}
#endif // ENCODING_LOSSLESS_H
