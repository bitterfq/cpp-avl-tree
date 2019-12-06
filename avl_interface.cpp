//
// Created by aman842 on 11/27/19.
//

#include "AVL.h"
#include <iostream>

int main(int argc, char **argv) {

  AVL tree;
  //  AVLnode node1(1);
  tree.Insert(3);
  tree.Insert(1);
  tree.Insert(5);
  tree.Insert(100);
  std::cout << tree.size() << std::endl;
  tree.Delete(12);
  std::cout << tree.size();
  tree.DeleteMin();
  std::cout << tree.size() << std::endl;

}