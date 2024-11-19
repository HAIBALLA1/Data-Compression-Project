//
// Created by Farouk on 18/11/24.
//

#ifndef UTILS_H
#define UTILS_H

template<typename W ,typename D >
class WeightedBinaryTree {
  public:
    static WeightedBinaryTree<W, D> leaf(W weight, D data) {
        return WeightedBinaryTree<W, D>(weight, data);
    }



  private:
     W const m_weight; // Poids du nœud
     std::variant<D, std::pair<WeightedBinaryTree<W, D>*, WeightedBinaryTree<W, D>*>> m_content; // Contenu du nœud
     WeightedBinaryTree(W weight , D data) : m_weight(weight) , m_content(D) {};
};




#endif //UTILS_H
