#include <cassert>
#include <iostream>
#include <string>
#include <queue>

#include "json.hpp"
#include "AVL.h"

using node = AVL::node ;
using namespace std;

size_t AVL::size() const {
  return size_;
}

bool AVL::empty() const {
  return size_ == 0;
}
std::string AVL::JSON() const {
  nlohmann::json result;
  std::queue<std::shared_ptr<node> > nodes;
  if (root_ != nullptr) {
    result["root"] = root_->key;
    nodes.push(root_);
    while (!nodes.empty()) {
      auto v = nodes.front();
      nodes.pop();
      std::string key = std::to_string(v->key);
      if (v->left != nullptr) {
        result[key]["left"] = v->left->key;
        nodes.push(v->left);
      }
      if (v->right != nullptr) {
        result[key]["right"] = v->right->key;
        nodes.push(v->right);
      }
      if (v->parent.lock() != nullptr) {
        result[key]["parent"] = v->parent.lock()->key;
      } else {
        result[key]["root"] = true;
      }
    }
  }
  result["size"] = size_;
  return result.dump(2) + "\n";
}

AVL::AVL () {
  size_ = 0;
}


//Geeks for geeks deleteion method specified

//https://www.geeksforgeeks.org/avl-tree-set-2-deletion/

// Tried reusing Delete method but the specification of the root node made it hard to copy deletion method from geeksforgeeks, the idea is the same
// Bad practice here, rewriting the entire Delete again for one parameter difference.



shared_ptr<AVL::node> AVL::rightRotation(shared_ptr<node> head) {
  shared_ptr<node> newhead = head->left;
  shared_ptr<node> grandparent = head->parent.lock();
  shared_ptr<node> orphan = newhead->right  ;

  head->left = newhead->right;
  newhead->right = head;
  head->height = 1 + max(height(head->left), height(head->right));

  /*if (head->balance == BALANCED) {
    newhead->balance = LEFT_HEAVY;
    head->balance = RIGHT_HEAVY;
  } else {
    newhead->balance = BALANCED;
    head->balance = BALANCED;
  }*/
  return newhead;
}

shared_ptr<AVL::node> AVL::leftRotation(shared_ptr<node> head) {
  shared_ptr<node> newhead = head->right;
  shared_ptr<node> grandparent = head->parent.lock();
  shared_ptr<node> orphan = newhead->left  ;

  head->right = newhead->left;
  newhead->left = head;
  head->height = 1 + max(height(head->left), height(head->right));

  /*if (head->balance == BALANCED) {
    newhead->balance = RIGHT_HEAVY;
    head->balance = LEFT_HEAVY;
  } else {
    newhead->balance = BALANCED;
    head->balance = BALANCED;
  }*/
  return newhead;
}

void inorderUtil(shared_ptr<node> head) {
  if (head == nullptr) return;
  inorderUtil(head->left);
  inorderUtil(head->right);
}

