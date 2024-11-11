    //
// Created by ing on 08/11/24.
//

#include <vector>
#include <cmath>

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
                    res[i] = (res[i] + source[j] * std::cos(2 * M_PI * i * j / nb_coefs));
                }
            }
            return res;
        }

    };


};
#ifndef ENCODING_LOSSLY_H
#define ENCODING_LOSSLY_H

#endif //ENCODING_LOSSLY_H
