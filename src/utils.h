//
// Created by Farouk on 18/11/24.
//

#ifndef UTILS_H
#define UTILS_H

#include <variant>
#include <utility>
#include <vector>
#include <algorithm>
#include <functional>
#include <memory>
#include <optional>

namespace utils {

template<typename W, typename D>
class WeightedBinaryTree {
public:

    // 3o) Constructeur
    // Constructeur pour une feuille
    WeightedBinaryTree(W weight, D data)
        : m_weight(weight), m_content(data) {}

    // Constructeur pour un nœud complexe
    WeightedBinaryTree(W weight, std::pair<std::shared_ptr<WeightedBinaryTree<W, D>>, std::shared_ptr<WeightedBinaryTree<W, D>>> children)
        : m_weight(weight), m_content(children) {}

    // Constructeur générique
    WeightedBinaryTree(W weight, std::variant<D, std::pair<std::shared_ptr<WeightedBinaryTree<W, D>>, std::shared_ptr<WeightedBinaryTree<W, D>>>> content)
        : m_weight(weight), m_content(content) {}
    // Constructeur par copie
    WeightedBinaryTree(const WeightedBinaryTree& other)
        : m_weight(other.m_weight), m_content(other.m_content) {}

    // Constructeur par déplacement
    WeightedBinaryTree(WeightedBinaryTree&& other)
        : m_weight(other.m_weight), m_content(std::move(other.m_content)) {}

    // Opérateur d'affectation par copie supprimé
    WeightedBinaryTree& operator=(const WeightedBinaryTree& other) = delete;

    // Opérateur d'affectation par déplacement supprimé
    WeightedBinaryTree& operator=(WeightedBinaryTree&& other) = delete;


    // 4o) Méthode statique publique leaf
    static std::shared_ptr<WeightedBinaryTree<W, D>> leaf(W weight, D data) {
        return std::make_shared<WeightedBinaryTree>(weight, data);
    }


    // 5o) Méthode statique publique complex
    static std::shared_ptr<WeightedBinaryTree<W, D>> complex(std::shared_ptr<WeightedBinaryTree<W, D>> left, std::shared_ptr<WeightedBinaryTree<W, D>> right) {
        W combined_weight = left->get_weight() + right->get_weight();
        return std::make_shared<WeightedBinaryTree>(combined_weight, std::make_pair(left, right));
    }


    // 6o) Méthodes publiques constantes
    bool is_leaf() const {
        return std::holds_alternative<D>(m_content);
    }

    bool is_complex() const {
        return std::holds_alternative<std::pair<std::shared_ptr<WeightedBinaryTree<W, D>>, std::shared_ptr<WeightedBinaryTree<W, D>>>>(m_content);
    }


    W const get_weight() const {
        return m_weight;
    }

    D const& get_data() const {
        return std::get<D>(m_content);
    }

    // 7o) Méthodes publiques
    std::shared_ptr<WeightedBinaryTree<W, D>> get_left() const {
        return std::get<std::pair<std::shared_ptr<WeightedBinaryTree<W, D>>, std::shared_ptr<WeightedBinaryTree<W, D>>>>(m_content).first;
    }


    std::shared_ptr<WeightedBinaryTree<W, D>> get_right() const {
        return std::get<std::pair<std::shared_ptr<WeightedBinaryTree<W, D>>, std::shared_ptr<WeightedBinaryTree<W, D>>>>(m_content).second;
    }


    // 8o) Méthode publique add_dummy
    void add_dummy() {
        if (is_complex()) {
            auto& subtrees = std::get<std::pair<std::shared_ptr<WeightedBinaryTree<W, D>>, std::shared_ptr<WeightedBinaryTree<W, D>>>>(m_content);
            auto A = subtrees.second;

            // Créer un std::shared_ptr nul explicite
            std::shared_ptr<WeightedBinaryTree<W, D>> null_ptr = nullptr;

            // Spécifier le type lors de l'appel à make_shared
            auto new_subtree = std::make_shared<WeightedBinaryTree<W, D>>(
                A->get_weight(),
                std::make_pair(A, null_ptr)
            );
            subtrees.second = new_subtree;
        }
    }



    // 9o) Opérateurs de comparaison
    bool operator<(const WeightedBinaryTree<W, D>& other) const {
        return m_weight < other.m_weight;
    }

    bool operator>(const WeightedBinaryTree<W, D>& other) const {
        return m_weight > other.m_weight;
    }

    // 10o) Destructeur
    ~WeightedBinaryTree() = default;


private:



    // 2o) Champs privés
    W const m_weight;
    std::variant<D, std::pair<std::shared_ptr<WeightedBinaryTree<W, D>>, std::shared_ptr<WeightedBinaryTree<W, D>>> > m_content;

};

// 11o) Fonction insert_in_isorted
template<typename D>
void insert_in_isorted(std::vector<D>& v, D data) {
    auto it = std::lower_bound(v.rbegin(), v.rend(), data, std::greater<D>());
    v.insert(it.base(), data);
}


    // find_match ( q1)
    template<typename D>
    std::optional<std::pair<std::size_t, std::size_t> >
    find_match(const std::vector<D>& source, const std::vector<D>& match) {
    std::optional<std::pair<std::size_t, std::size_t>> best_match;
    std::size_t best_length = 0;

    for (std::size_t i = 0; i < source.size(); ++i) {
        if (source[i] == match[0]) {
            std::size_t j = 0;
            while ((i + j) < source.size() && j < match.size() && source[i + j] == match[j]) {
                ++j;
            }
            if (j > best_length) {
                best_length = j;
                best_match = std::make_pair(i, j);
            }
        }
    }

    return best_match;
}

    // vector_shift ( q2 )
    template<typename D>
    void vector_shift(const std::vector<D>& source, std::vector<D>& windows, std::size_t const length) {
    // retire les 'length' premières données de 'windows'
    if (length >= windows.size()) {
        windows.clear();
    } else {
        windows.erase(windows.begin(), windows.begin() + length);
    }

    // ajoute à la fin de 'windows' les 'length' premières données de 'source'
    windows.insert(windows.end(), source.begin(), source.begin() + length);
}

}





#endif //UTILS_H
