#pragma once

#include <vector>
#include <cmath>
#include <concepts>
#include <numbers>
#include <functional>
#include <cstddef>
#include <utility>
#include <cassert>
#include <span>
#include <type_traits>
#include <algorithm>
#include <numeric>

namespace encoding::lossy {

    struct DiscreteCosinus {
        // D = type des données d'entrée (arithmétique), R = type retour (float/double)
        template <typename D, typename R>
        requires (std::is_arithmetic_v<D> && std::is_arithmetic_v<R>)
        [[nodiscard]]
        static std::vector<R> encode(std::span<const D> source, std::size_t nb_coefs) {
            if (source.empty() || nb_coefs == 0) return {};
            using std::cos;
            const auto N = source.size();
            std::vector<R> res(nb_coefs, R{0});
            for (std::size_t k = 0; k < nb_coefs; ++k) {
                R sum = R{0};
                for (std::size_t n = 0; n < N; ++n) {
                    sum += static_cast<R>(source[n]) *
                           static_cast<R>(cos(std::numbers::pi_v<long double> * (static_cast<long double>(n) + 0.5L)
                                              * static_cast<long double>(k) / static_cast<long double>(N)));
                }
                res[k] = sum * static_cast<R>(std::sqrt(2.0 / static_cast<long double>(N)));
            }
            return res;
        }

        template <typename D, typename R>
        requires (std::is_arithmetic_v<D> && std::is_arithmetic_v<R>)
        [[nodiscard]]
        static std::vector<D> decode(std::span<const R> encoded, std::size_t size) {
            if (encoded.empty() || size == 0) return std::vector<D>(size, D{});
            using std::cos;
            const auto K = encoded.size();
            std::vector<D> res(size, D{});
            for (std::size_t n = 0; n < size; ++n) {
                long double acc = 0.0L;
                for (std::size_t k = 0; k < K; ++k) {
                    acc += static_cast<long double>(encoded[k]) *
                           cos(std::numbers::pi_v<long double> *
                               (static_cast<long double>(n) + 0.5L) *
                               static_cast<long double>(k) / static_cast<long double>(size));
                }
                acc *= std::sqrt(2.0L / static_cast<long double>(size));
                res[n] = static_cast<D>(acc);
            }
            return res;
        }
    };

    struct Quantization {
        using get_quantum_t = std::function<double(std::size_t)>;

        // quantum constant
        template<typename D>
        requires std::is_arithmetic_v<D>
        [[nodiscard]]
        static std::vector<int> encode(std::span<const D> source, double quantum) {
            std::vector<int> res; res.reserve(source.size());
            for (auto v : source) res.push_back(static_cast<int>(std::lround(static_cast<double>(v) / quantum)));
            return res;
        }

        template<typename D>
        requires std::is_arithmetic_v<D>
        [[nodiscard]]
        static std::vector<D> decode(std::span<const int> source, double quantum) {
            std::vector<D> res; res.reserve(source.size());
            for (auto q : source) res.push_back(static_cast<D>(static_cast<double>(q) * quantum));
            return res;
        }

        // quantum par coefficient (version std::function)
        template<typename D>
        requires std::is_arithmetic_v<D>
        [[nodiscard]]
        static std::vector<int> encode(std::span<const D> source, const get_quantum_t& get_quantum) {
            std::vector<int> res; res.reserve(source.size());
            for (std::size_t i = 0; i < source.size(); ++i) {
                const double q = get_quantum(i);
                res.push_back(static_cast<int>(std::lround(static_cast<double>(source[i]) / q)));
            }
            return res;
        }

        template<typename D>
        requires std::is_arithmetic_v<D>
        [[nodiscard]]
        static std::vector<D> decode(std::span<const int> source, const get_quantum_t& get_quantum) {
            std::vector<D> res; res.reserve(source.size());
            for (std::size_t i = 0; i < source.size(); ++i) {
                const double q = get_quantum(i);
                res.push_back(static_cast<D>(static_cast<double>(source[i]) * q));
            }
            return res;
        }

        // variante générique (meilleure perf) : tout callable (lambda, functor) — évite std::function.
        template<typename D, typename F>
        requires (std::is_arithmetic_v<D> && std::invocable<F, std::size_t>)
        [[nodiscard]]
        static std::vector<int> encode_fn(std::span<const D> source, F&& get_quantum) {
            std::vector<int> res; res.reserve(source.size());
            for (std::size_t i = 0; i < source.size(); ++i) {
                const double q = std::invoke(get_quantum, i);
                res.push_back(static_cast<int>(std::lround(static_cast<double>(source[i]) / q)));
            }
            return res;
        }

        template<typename D, typename F>
        requires (std::is_arithmetic_v<D> && std::invocable<F, std::size_t>)
        [[nodiscard]]
        static std::vector<D> decode_fn(std::span<const int> source, F&& get_quantum) {
            std::vector<D> res; res.reserve(source.size());
            for (std::size_t i = 0; i < source.size(); ++i) {
                const double q = std::invoke(get_quantum, i);
                res.push_back(static_cast<D>(static_cast<double>(source[i]) * q));
            }
            return res;
        }
    };

    struct DiscreteFourier {
        // encode renvoie (parties réelle, imaginaire)
        template<typename D, typename R>
        requires (std::is_arithmetic_v<D> && std::is_arithmetic_v<R>)
        [[nodiscard]]
        static std::pair<std::vector<R>, std::vector<R>>
        encode(std::span<const D> source, std::size_t nb_coefs) {
            std::vector<R> real, imag;
            if (source.empty() || nb_coefs == 0) return {real, imag};

            real.assign(nb_coefs, R{});
            imag.assign(nb_coefs, R{});
            const auto N = source.size();
            for (std::size_t k = 0; k < nb_coefs; ++k) {
                R rr = R{}, ii = R{};
                for (std::size_t n = 0; n < N; ++n) {
                    const long double theta =
                        (2.0L * std::numbers::pi_v<long double> * static_cast<long double>(k) * static_cast<long double>(n))
                        / static_cast<long double>(N);
                    rr += static_cast<R>(source[n]) * static_cast<R>(std::cos(theta));
                    ii += static_cast<R>(source[n]) * static_cast<R>(std::sin(theta));
                }
                real[k] = rr;
                imag[k] = ii;
            }
            return {real, imag};
        }

        template<typename D, typename R>
        requires (std::is_arithmetic_v<D> && std::is_arithmetic_v<R>)
        [[nodiscard]]
        static std::vector<D>
        decode(const std::pair<std::vector<R>, std::vector<R>>& enc, std::size_t size) {
            const auto& real = enc.first;
            const auto& imag = enc.second;
            assert(real.size() == imag.size());
            const auto K = real.size();
            std::vector<D> out(size, D{});
            if (size == 0 || K == 0) return out;

            for (std::size_t n = 0; n < size; ++n) {
                long double acc = 0.0L;
                for (std::size_t k = 0; k < K; ++k) {
                    const long double theta =
                        (2.0L * std::numbers::pi_v<long double> * static_cast<long double>(k) * static_cast<long double>(n))
                        / static_cast<long double>(K);
                    acc += static_cast<long double>(real[k]) * std::cos(theta)
                         - static_cast<long double>(imag[k]) * std::sin(theta);
                }
                // normalisation simple par K (cohérente avec encode ci-dessus)
                out[n] = static_cast<D>(acc / static_cast<long double>(K));
            }
            return out;
        }
    };

} // namespace encoding::lossy

