#pragma once

#include <variant>
#include <utility>
#include <vector>
#include <algorithm>
#include <functional>
#include <memory>
#include <optional>
#include <cstddef>
#include <cassert>
#include <span>
#include <type_traits>

namespace utils {

template<typename W, typename D>
class WeightedBinaryTree {
public:
    using node_type = WeightedBinaryTree<W, D>;
    using node_ptr  = std::shared_ptr<node_type>;
    using pair_ptr  = std::pair<node_ptr, node_ptr>;
    using content_t = std::variant<D, pair_ptr>;

private:
    W         m_weight;
    content_t m_content;

public:
    // --- ctors ---
    explicit WeightedBinaryTree(W weight, D data)
        : m_weight(weight), m_content(std::move(data)) {}

    explicit WeightedBinaryTree(W weight, pair_ptr children)
        : m_weight(weight), m_content(std::move(children)) {}

    explicit WeightedBinaryTree(W weight, content_t content)
        : m_weight(weight), m_content(std::move(content)) {}

    WeightedBinaryTree(const WeightedBinaryTree&)            = default;
    WeightedBinaryTree(WeightedBinaryTree&&) noexcept        = default;
    WeightedBinaryTree& operator=(const WeightedBinaryTree&) = delete;
    WeightedBinaryTree& operator=(WeightedBinaryTree&&)      = delete;
    ~WeightedBinaryTree()                                    = default;

    // --- factories ---
    static node_ptr leaf(W weight, const D& data) {
        return std::make_shared<node_type>(weight, data);
    }

    static node_ptr complex(const node_ptr& left, const node_ptr& right) {
        const W combined = left->get_weight() + right->get_weight();
        return std::make_shared<node_type>(combined, pair_ptr{left, right});
    }

    // --- queries ---
    [[nodiscard]] bool is_leaf() const noexcept {
        return std::holds_alternative<D>(m_content);
    }

    [[nodiscard]] bool is_complex() const noexcept {
        return std::holds_alternative<pair_ptr>(m_content);
    }

    [[nodiscard]] W get_weight() const noexcept { return m_weight; }

    [[nodiscard]] const D& get_data() const {
        assert(is_leaf());
        return std::get<D>(m_content);
    }

    [[nodiscard]] node_ptr get_left() const {
        assert(is_complex());
        return std::get<pair_ptr>(m_content).first;
    }

    [[nodiscard]] node_ptr get_right() const {
        assert(is_complex());
        return std::get<pair_ptr>(m_content).second;
    }

    // Ajout d’un nœud factice sur la branche droite (comportement d’origine)
    void add_dummy() {
        if (!is_complex()) return;
        auto& subtrees = std::get<pair_ptr>(m_content);
        auto A = subtrees.second;
        if (!A) return; // rien à faire si déjà nul
        node_ptr null_ptr = nullptr;
        auto new_subtree = std::make_shared<node_type>(
            A->get_weight(),
            pair_ptr{A, null_ptr}
        );
        subtrees.second = std::move(new_subtree);
    }

    // tri par poids
    [[nodiscard]] bool operator<(const WeightedBinaryTree& other) const noexcept {
        return m_weight < other.m_weight;
    }
    [[nodiscard]] bool operator>(const WeightedBinaryTree& other) const noexcept {
        return m_weight > other.m_weight;
    }
};

// insère dans v (ordre décroissant) en gardant l’ordre trié
template<typename D>
void insert_in_isorted(std::vector<D>& v, const D& data) {
    auto it = std::lower_bound(v.rbegin(), v.rend(), data, std::greater<D>());
    v.insert(it.base(), data);
}

// find_match : retourne (offset depuis la fin du buffer de recherche, longueur)
template<typename D>
[[nodiscard]]
std::optional<std::pair<std::size_t, std::size_t>>
find_match(std::span<const D> search_buffer, std::span<const D> lookahead_buffer) {
    if (search_buffer.empty() || lookahead_buffer.empty()) return std::nullopt;

    std::optional<std::pair<std::size_t, std::size_t>> best_match;
    std::size_t best_length = 0;
    const std::size_t S = search_buffer.size();

    for (std::size_t i = 0; i < S; ++i) {
        std::size_t length = 0;
        while (i + length < S &&
               length < lookahead_buffer.size() &&
               search_buffer[i + length] == lookahead_buffer[length]) {
            ++length;
        }
        if (length > best_length) {
            best_length = length;
            const std::size_t offset = S - i;
            best_match = std::make_pair(offset, length);
        }
    }
    return best_match;
}

// vector_shift : fait glisser la fenêtre et ajoute les 'length' 1ers éléments de source
template<typename D>
void vector_shift(std::span<const D> source, std::vector<D>& window, std::size_t length) {
    if (length >= window.size()) window.clear();
    else window.erase(window.begin(), window.begin() + length);

    const std::size_t to_copy = std::min(length, source.size());
    window.insert(window.end(), source.begin(), source.begin() + to_copy);
}

} // namespace utils

