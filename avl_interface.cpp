//
// Created by aman842 on 11/27/19.
//

#include "AVL.h"
#include <iostream>
int main () {
    AVL tree;
  //  AVLnode node1(1);
    tree.Insert(9);
    tree.Insert(-94);
    tree.Insert(948);
    tree.Insert(289);
    std::cout << tree.size();
}