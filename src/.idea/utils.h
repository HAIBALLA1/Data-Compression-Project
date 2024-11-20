//
// Created by Farouk on 18/11/24.
//

#ifndef UTILS_H
#define UTILS_H
#include <utility>
#include <vector>

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



  private:
     W const m_weight; // Poids du nœud
     std::variant<D, std::pair<WeightedBinaryTree<W, D>*, WeightedBinaryTree<W, D>*>> m_content; // Contenu du nœud
    //constric pour créer un arbre a patir de 2 feuilles.
     WeightedBinaryTree(W weight , D data) : m_weight(weight) , m_content(D) {};
    //constric pour créer un arbre a patir de 2 sous_arbres.
    WeightedBinaryTree(W weight, std::pair<WeightedBinaryTree<W, D>*, WeightedBinaryTree<W, D>*> subtrees)
    : m_weight(weight), m_content(subtrees) {}

};




#endif //UTILS_H
