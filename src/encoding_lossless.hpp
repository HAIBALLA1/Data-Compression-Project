#pragma once

#include <vector>
#include <utility>
#include <memory>
#include <unordered_map>
#include <algorithm>
#include <variant>
#include <optional>
#include <cassert>
#include <cstddef>
#include "utils.hpp"

namespace encoding::lossless {

    // ===== RLE ciblé (CompressRepeating) =====
    template<typename D>
    struct CompressRepeating {
        [[nodiscard]]
        static std::pair<std::vector<std::pair<D, std::size_t>>, std::size_t>
        encode(const std::vector<D>& source, const D& repeating) {
            std::vector<std::pair<D, std::size_t>> encoded;
            encoded.reserve(source.size());
            std::size_t cnt = 0;
            for (const auto& el : source) {
                if (el == repeating) ++cnt;
                else { encoded.emplace_back(el, cnt); cnt = 0; }
            }
            return {std::move(encoded), cnt};
        }

        [[nodiscard]]
        static std::vector<D> decode(const std::vector<std::pair<D, std::size_t>>& encoded,
                                     std::size_t count_end,
                                     const D& repeating) {
            std::vector<D> out;
            out.reserve(encoded.size() + count_end);
            for (auto& p : encoded) {
                out.insert(out.end(), p.second, repeating);
                out.push_back(p.first);
            }
            out.insert(out.end(), count_end, repeating);
            return out;
        }
    };

    // ===== Huffman (s’appuie sur utils::WeightedBinaryTree) =====
    template<typename D>
    struct Huffman {
        using WBT = utils::WeightedBinaryTree<std::size_t, D>;

        static void generate_codes(const std::shared_ptr<WBT>& t,
                                   std::vector<bool>& path,
                                   std::unordered_map<D, std::vector<bool>>& map) {
            if (t->is_leaf()) { map[t->get_data()] = path; return; }
            if (auto l = t->get_left()) { path.push_back(false); generate_codes(l, path, map); path.pop_back(); }
            if (auto r = t->get_right()){ path.push_back(true ); generate_codes(r, path, map); path.pop_back(); }
        }

        [[nodiscard]]
        static std::pair<std::shared_ptr<WBT>, std::vector<std::vector<bool>>>
        encode(const std::vector<D>& src, bool add_dummy) {
            std::unordered_map<D, std::size_t> freq;
            freq.reserve(src.size());
            for (auto& d : src) freq[d]++;

            std::vector<std::shared_ptr<WBT>> trees;
            trees.reserve(freq.size());
            for (auto& [d,f] : freq) trees.push_back(WBT::leaf(f, d));
            if (trees.empty()) return {nullptr, {}};

            while (trees.size() > 1) {
                std::sort(trees.begin(), trees.end(),
                          [](auto& a, auto& b){ return a->get_weight() > b->get_weight(); });
                auto L = trees.back(); trees.pop_back();
                auto R = trees.back(); trees.pop_back();
                trees.push_back(WBT::complex(L, R));
            }
            auto tree = trees.front();
            if (add_dummy) tree->add_dummy();

            std::unordered_map<D, std::vector<bool>> encmap;
            encmap.reserve(freq.size());
            std::vector<bool> path;
            generate_codes(tree, path, encmap);

            std::vector<std::vector<bool>> out;
            out.reserve(src.size());
            for (auto& d : src) out.push_back(encmap[d]);
            return {tree, std::move(out)};
        }

        [[nodiscard]]
        static std::vector<D> decode(const std::shared_ptr<WBT>& tree,
                                     const std::vector<bool>& bits) {
            std::vector<D> out;
            if (!tree) return out;
            auto n = tree;
            for (bool b : bits) {
                n = b ? n->get_right() : n->get_left();
                if (n->is_leaf()) { out.push_back(n->get_data()); n = tree; }
            }
            return out;
        }
    };

    // ===== LZ77 (token = littéral D ou (offset,length)) =====
    template<typename D>
    struct LZ77 {
        using token = std::variant<D, std::pair<std::size_t, std::size_t>>;

        static std::optional<std::pair<std::size_t, std::size_t>>
        find_match(const std::vector<D>& search, const std::vector<D>& look) {
            if (search.empty() || look.empty()) return std::nullopt;
            std::size_t best_len=0, best_off=0;
            for (std::size_t i=0;i<search.size();++i){
                std::size_t len=0;
                while (i+len<search.size() && len<look.size() && search[i+len]==look[len]) ++len;
                if (len>best_len){ best_len=len; best_off = search.size()-i; }
            }
            if (best_len>0) return std::make_pair(best_off,best_len);
            return std::nullopt;
        }

        [[nodiscard]]
        static std::vector<token>
        encode(const std::vector<D>& src, std::size_t buffer_size, std::size_t chunk_size) {
            assert(chunk_size < buffer_size);
            std::vector<token> out; out.reserve(src.size());
            std::size_t cur=0, n=src.size();
            while (cur<n) {
                std::size_t sbeg = (cur>=buffer_size)? (cur-buffer_size) : 0;
                std::vector<D> search(src.begin()+sbeg, src.begin()+cur);
                std::vector<D> look  (src.begin()+cur,  src.begin()+std::min(cur+chunk_size, n));
                auto m = find_match(search, look);
                if (m && m->second>0) { out.emplace_back(*m); cur += m->second; }
                else { out.emplace_back(src[cur]); ++cur; }
            }
            return out;
        }

        [[nodiscard]]
        static std::vector<D>
        decode(const std::vector<token>& enc, std::size_t buffer_size) {
            std::vector<D> res; res.reserve(enc.size());
            std::vector<D> win; win.reserve(buffer_size);
            for (auto& t : enc) {
                if (std::holds_alternative<D>(t)) {
                    D v = std::get<D>(t); res.push_back(v); win.push_back(v);
                } else {
                    auto [off,len] = std::get<std::pair<std::size_t,std::size_t>>(t);
                    std::size_t ws = win.size(), start = ws - off;
                    for (std::size_t i=0;i<len;++i){ D v=win[start+i]; res.push_back(v); win.push_back(v); }
                }
                if (win.size()>buffer_size)
                    win.erase(win.begin(), win.begin() + (win.size()-buffer_size));
            }
            return res;
        }
    };

} // namespace encoding::lossless

