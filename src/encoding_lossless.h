//
// Created by naben on 18/11/24.
//
#ifndef ENCODING_LOSSLESS_HPP
#define ENCODING_LOSSLESS_HPP

#include <vector>
#include <utility>

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
}
#endif // ENCODING_LOSSLESS_HPP
