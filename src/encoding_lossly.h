    //
// Created by Farouk on 08/11/24.
//
#ifndef ENCODING_LOSSLY_H
#define ENCODING_LOSSLY_H

#include <vector>
#include <cmath>
#include <functional> // Pour std::function
#include <cstddef> //pour std::size_t
#include <utility>
#include <cassert>


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
    struct DiscreteFourier{
            // encodage
            template<typename D, typename R>
            static std::pair<std::vector<R>, std::vector<R>> encode(const std::vector<D>& source, const std::size_t nb_coefs) {
                std::vector<R> res_real;
                std::vector<R> res_img;

                if (source.empty()) {
                    return std::make_pair(res_real, res_img);  // renvoie des vecteurs vides si la source est vide
                }

                for (std::size_t i = 0; i < nb_coefs; ++i) {
                    R real_sum = static_cast<R>(source[0]);
                    R img_sum = static_cast<R>(0);
                    for (std::size_t j = 1; j < source.size(); ++j) {
                        double angle = 2 * M_PI * i * j / nb_coefs;
                        real_sum += static_cast<R>(source[j]) * std::cos(angle);
                        img_sum += static_cast<R>(source[j]) * std::sin(angle);
                    }
                    res_real.push_back(real_sum);
                    res_img.push_back(img_sum);
                }
                return std::make_pair(res_real, res_img);
            }

            // decodage
            template<typename D, typename R>
            static std::vector<D> decode(const std::pair<std::vector<R>, std::vector<R>>& encoded, const std::size_t size) {
                const std::vector<R>& res_real = encoded.first;
                const std::vector<R>& res_img = encoded.second;

                // pour vérifier la taille des vecteurs encodés
                assert(!res_real.empty() && res_real.size() == res_img.size());

                std::size_t nb_coefs = res_real.size();
                std::vector<D> res;

                for (std::size_t i = 0; i < size; ++i) {
                    R sum = res_real[0];
                    for (std::size_t j = 1; j < nb_coefs; ++j) {
                        double theta = (2 * M_PI * j * i) / nb_coefs;
                        sum += res_real[j] * std::cos(theta) - res_img[j] * std::sin(theta);
                    }
                    sum /= nb_coefs;
                    res.push_back(static_cast<D>(sum));
                }
                return res;
            }
        };


}

#endif //ENCODING_LOSSLY_H
