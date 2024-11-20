//
// Created by Farouk on 18/11/24.
//

#ifndef UTILS_H
#define UTILS_H

#include <variant>
#include <utility>
#include <vector>
#include <functional>

template<typename W ,typename D >
class WeightedBinaryTree {
  public:

    //methode qui cree un nouds d'arbre.
    static WeightedBinaryTree<W, D> leaf(W weight, D data) {
        return WeightedBinaryTree<W, D>(weight, data);
    }

    // make_pair cree un objet  std::pair sans specifier les types des parametres std::make_pair(left->m_weight, right->m_weight) <==> std::pair<left,right> pr)
    static WeightedBinaryTree<W, D> complex(WeightedBinaryTree<W, D> const * left, WeightedBinaryTree<W, D> const * right) {
        return WeightedBinaryTree<W, D>(left->m_weight + right->m_weight,
                                        std::make_pair(left->m_weight, right->m_weight));
    }

    //verifier si c est un feuille ou pas .
    bool isLeaf() const {
        return std::holds_alternative<D>(m_content);
    }


    //verifier si le neoud est cree par deux subtrees
    bool isComplex() const {
        return std::holds_alternative<std::pair<WeightedBinaryTree<W, D>*, WeightedBinaryTree<W, D>*>>(m_content);
    }


    W const get_weight() {
        return m_weight ;
    }

    D const & get_data() {
        return m_content;
    }

    WeightedBinaryTree<W, D> const & get_left() {
        return *std::get<std::pair<WeightedBinaryTree<W, D>*, WeightedBinaryTree<W, D>*>>(m_content).first;
    }

    WeightedBinaryTree<W, D> const & get_right() {
        return *std::get<std::pair<WeightedBinaryTree<W, D>*, WeightedBinaryTree<W, D>*>>(m_content).second;
    }

    //je souleve une exception dans le ou ona jute une feuille
    void add_dummy() {
        if (!isComplex()) {
            throw std::runtime_error("Cannot add a dummy to a leaf node.");
        }

        //ici j'ai utilise const_cast , car d'apres l'enonce get_right() est const donc je suis oblige de caster pour enlever le const a cette fonct
        auto* dummy =
            new WeightedBinaryTree<W, D>(get_weight(), std::make_pair(const_cast<WeightedBinaryTree<W, D>>(&get_right()), static_cast<WeightedBinaryTree<W, D>>(nullptr))
        );

        std::get<std::pair<WeightedBinaryTree<W, D>, WeightedBinaryTree<W, D>>>(m_content).second = dummy;
    }

    bool operator < (WeightedBinaryTree<W, D>* t) {
        return t->m_weight < this->m_weight;
    }

    bool operator >(WeightedBinaryTree<W, D>* t) {
        return t->m_weight > this->m_weight;
    }

    ~WeightedBinaryTree() {
        if (isComplex()) {
            auto& subtrees = std::get<std::pair<WeightedBinaryTree<W, D>*, WeightedBinaryTree<W, D>*>>(m_content);
            delete subtrees.first;
            delete subtrees.second;
        }
    }


    void insert_in_isorted(std::vector<D> v, D data) {
        //v.begin() retourne  un itérateur
        auto it =v.begin();
        while (it != v.end() && it >data) {
            ++it;
        }
        v.insert(it, data);
    }

  private:

     W const m_weight; // Poids du nœud
     std::variant<D, std::pair<WeightedBinaryTree<W, D>*, WeightedBinaryTree<W, D>*>> m_content; // Contenu du nœud
     //constric pour créer un arbre a patir de 2 feuilles.
     WeightedBinaryTree(W weight, D data) : m_weight(weight), m_content(data) {}
     //constric pour créer un arbre a patir de 2 sous_arbres.
     WeightedBinaryTree(W weight, std::pair<WeightedBinaryTree<W, D>*, WeightedBinaryTree<W, D>*> subtrees)
    : m_weight(weight), m_content(subtrees) {}

};





#endif //UTILS_H
