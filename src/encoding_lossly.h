    //
// Created by Farouk on 08/11/24.
//
#ifndef ENCODING_LOSSLY_H
#define ENCODING_LOSSLY_H

#include <vector>
#include <cmath>
#include <functional> // Pour std::function
#include <cstddef> //pour std::size_t

namespace encoding::lossly {
    struct DiscreteCosinus {
        template<typename D, typename R>
        static std::vector<R> encode(std::vector<D> const & source, std::size_t const & nb_coefs) {
            std::vector<R> res;

            if (source.empty()) {
                return res;

            }
            for(std::size_t i = 0; i < nb_coefs; ++i) {
                res.push_back(source[0]);
                for (std::size_t j = 1; j < source.size(); ++j) {
                    res[i] =res[i] + source[j] * std::cos((2 * M_PI * i * j) / nb_coefs);
                }
            }
            return res;
        }
        template<typename D, typename R>
        static  std::vector<D> decode(std::vector<R> const & encoded, std::size_t const size) {
            std::vector<D> res;
            if (encoded.empty()) {
                return res;

            }
            std::size_t nb_coefs =encoded.size();
            for (std::size_t i = 0; i < size; ++i) {
                res.push_back(encoded[0]);
                for (std::size_t j = 1; j < encoded.size(); ++j) {
                    double theta = (2 * M_PI * j * i) / nb_coefs;
                    res[i] = res[i] + encoded[j] * cos(theta);
                }
                res[i] = res[i] / nb_coefs;
            }
            return res;

        }

    };


   struct quantization {
        using  get_quantum_t = std::function<double(std::size_t k)>;
        template<typename D>
        static std::vector<int> encode(std::vector<D> const & source, double const quantum) {
            std::vector<int> res;
            res.reserve(source.size()); //pour reserver de la memoire pour res.
            for (std::size_t i = 0; i < source.size(); ++i) {
                res.push_back(std::round(source[i] / quantum));

            }
            return res;
        }
        template<typename D>
        static std::vector<D> decode(std::vector<int> const & source, double const quantum) {
            std::vector<D> res;
            res.reserve(source.size());

            // Boucle pour reconstruire les éléments du vecteur res depuis le vecteur encodé
            for (const auto& value : source) {
                res.push_back(value * quantum);
            }

            return res;

        }

        template<typename D>
        static std::vector<int> encode(std::vector<D> const & source, get_quantum_t get_quantum) {
            std::vector<int> res;
            res.reserve(source.size());
            for (std::size_t i = 0; i < source.size(); ++i) {
                res.push_back(std::round(source[i] / get_quantum(i)));
            }

            return res;
        }

       template<typename D>
       static std::vector<D> decode(std::vector<int> const & source, get_quantum_t get_quantum) {
            std::vector<D> res;
            res.reserve(source.size());
            for (std::size_t i = 0; i < source.size(); ++i) {
                res.push_back(std::round(source[i]*get_quantum(i)));
            }
            return res;
        }



    };
}

#endif //ENCODING_LOSSLY_H
