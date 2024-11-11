//
// Created by ing on 08/11/24.
//

#include <iostream>
#include "encoding_lossly.h"

int main() {
    using namespace encoding::lossly;

    std::vector<int>source ={1,2,3,4,5};
    std::size_t nb_coefs=4;
    std::vector<double> resu=DiscreteCosinus::encode<int, double>(source, nb_coefs);;
    std::cout << "Résultat de l'encodage :" << std::endl;
    for (std::size_t i = 0; i < resu.size(); ++i) {
        std::cout << "Coefficient " << i << " : " << resu[i] << std::endl;
    }
    return 0;
}
