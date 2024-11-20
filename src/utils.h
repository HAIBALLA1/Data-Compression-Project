
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


    // leaf
    static std::shared_ptr<WeightedBinaryTree<W, D>> leaf(W weight, D data) {
        return std::make_shared<WeightedBinaryTree>(weight, data);
    }


    // complex
    static std::shared_ptr<WeightedBinaryTree<W, D>> complex(std::shared_ptr<WeightedBinaryTree<W, D>> left, std::shared_ptr<WeightedBinaryTree<W, D>> right) {
        W combined_weight = left->get_weight() + right->get_weight();
        return std::make_shared<WeightedBinaryTree>(combined_weight, std::make_pair(left, right));
    }



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


    std::shared_ptr<WeightedBinaryTree<W, D>> get_left() const {
        return std::get<std::pair<std::shared_ptr<WeightedBinaryTree<W, D>>, std::shared_ptr<WeightedBinaryTree<W, D>>>>(m_content).first;
    }


    std::shared_ptr<WeightedBinaryTree<W, D>> get_right() const {
        return std::get<std::pair<std::shared_ptr<WeightedBinaryTree<W, D>>, std::shared_ptr<WeightedBinaryTree<W, D>>>>(m_content).second;
    }



    void add_dummy() {
        if (is_complex()) {
            auto& subtrees = std::get<std::pair<std::shared_ptr<WeightedBinaryTree<W, D>>, std::shared_ptr<WeightedBinaryTree<W, D>>>>(m_content);
            auto A = subtrees.second;

            //  un std::shared_ptr nul explicite
            std::shared_ptr<WeightedBinaryTree<W, D>> null_ptr = nullptr;

            // le type lors de l'appel à make_shared
            auto new_subtree = std::make_shared<WeightedBinaryTree<W, D>>(
                A->get_weight(),
                std::make_pair(A, null_ptr)
            );
            subtrees.second = new_subtree;
        }
    }


    bool operator<(const WeightedBinaryTree<W, D>& other) const {
        return m_weight < other.m_weight;
    }

    bool operator>(const WeightedBinaryTree<W, D>& other) const {
        return m_weight > other.m_weight;
    }

    // 10o) Destructeur
    ~WeightedBinaryTree() = default;


private:

    W const m_weight;
    std::variant<D, std::pair<std::shared_ptr<WeightedBinaryTree<W, D>>, std::shared_ptr<WeightedBinaryTree<W, D>>> > m_content;

};

//Fonction insert_in_isorted
template<typename D>
void insert_in_isorted(std::vector<D>& v, D data) {
    auto it = std::lower_bound(v.rbegin(), v.rend(), data, std::greater<D>());
    v.insert(it.base(), data);
}


    // find_match
    template<typename D>
    std::optional<std::pair<std::size_t, std::size_t>>
    find_match(const std::vector<D>& search_buffer, const std::vector<D>& lookahead_buffer) {
    std::optional<std::pair<std::size_t, std::size_t>> best_match;
    std::size_t best_length = 0;
    std::size_t search_buffer_size = search_buffer.size();

    // Parcourir le buffer de recherche
    for (std::size_t i = 0; i < search_buffer_size; ++i) {
        std::size_t length = 0;
        // Comparaison des données du buffer de recherche avec celles du buffer de prélecture
        while (i + length < search_buffer_size &&
               length < lookahead_buffer.size() &&
               search_buffer[i + length] == lookahead_buffer[length]) {
            ++length;
               }
        // Mettre à jour la meilleure correspondance si une plus longue est trouvée
        if (length > best_length) {
            best_length = length;
            // Calcul de l'offset par rapport à la fin du buffer de recherche
            std::size_t offset = search_buffer_size - i;
            best_match = std::make_pair(offset, length);
        }
    }

    return best_match;
}


    // vector_shift
    template<typename D>
    void vector_shift(const std::vector<D>& source, std::vector<D>& windows, std::size_t const length) {
    // retire les 'length' premières données de 'windows'
    if (length >= windows.size()) {
        windows.clear();
    } else {
        windows.erase(windows.begin(), windows.begin() + length);
    }

    // ajout à la fin de 'windows' les 'length' premières données de 'source'
    windows.insert(windows.end(), source.begin(), source.begin() + length);
}

}





#endif //UTILS_H
