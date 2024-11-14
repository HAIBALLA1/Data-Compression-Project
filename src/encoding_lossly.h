#ifndef ENCODING_LOSSLY_HPP
#define ENCODING_LOSSLY_HPP

#include <vector>
#include <cmath>
#include <utility>
#include <cassert>

namespace encoding::lossly {

        // cosinus
        struct DiscreteCosinus {
            // encodage
            template<typename D, typename R>
            static std::vector<R> encode(const std::vector<D>& source, const std::size_t& nb_coefs) {
                std::vector<R> res;

                if (source.empty()) {
                    return res;
                }
                for (std::size_t i = 0; i < nb_coefs; ++i) {
                    R sum = static_cast<R>(source[0]);
                    for (std::size_t j = 1; j < source.size(); ++j) {
                        sum += static_cast<R>(source[j]) * std::cos((2 * M_PI * i * j) / nb_coefs);
                    }
                    res.push_back(sum);
                }
                return res;
            }

            // decodage
            template<typename D, typename R>
            static std::vector<D> decode(const std::vector<R>& encoded, const std::size_t& size) {
                std::vector<D> res;
                if (encoded.empty()) {
                    return res;
                }
                std::size_t nb_coefs = encoded.size();
                for (std::size_t i = 0; i < size; ++i) {
                    R sum = encoded[0];
                    for (std::size_t j = 1; j < nb_coefs; ++j) {
                        double theta = (2 * M_PI * j * i) / nb_coefs;
                        sum += encoded[j] * std::cos(theta);
                    }
                    sum /= nb_coefs;
                    res.push_back(static_cast<D>(sum));
                }
                return res;
            }
        };

        // fourier
        struct DiscreteFourier {
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

#endif // ENCODING_LOSSLY_HPP
