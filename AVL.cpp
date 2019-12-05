#include <cassert>
#include <iostream>
#include <string>
#include <queue>

#include "json.hpp"
#include "AVL.h"

void AVL::Insert(long int key) {
  shared_ptr<AVLnode> insertnode = node_search(key);

  if (!insertnode) { // if empty new node, root
    root_ = make_shared<AVLnode>(key);
    root_->height = std::max(height(root_->left_), height(root_->right_)) + 1;
  } else if (insertnode->key_ == key)
    return;
  else if (insertnode->key_ > key) // else new node is left child
  {
    insertnode->left_ = make_shared<AVLnode>(key, insertnode);
    retrace_insertion(insertnode->left_);
    update(insertnode->left_);
  } else {
    insertnode->right_ = make_shared<AVLnode>(key, insertnode);
    retrace_insertion(insertnode->right_);
    update(insertnode->right_);
  }

  size_++;
}

optional AVL::Find(long int key) const {
  shared_ptr<AVLnode> found = node_search(key);
  if (found && (key == found->key_))
    return reinterpret_cast<int &>(found->key_);
  return optional();
}

void AVL::Delete(long int key) {
  shared_ptr<AVLnode> target = node_search(key);

  if (!(target && (key == target->key_)))
    return;

  if (target->left_ || target->right_) {
    if (target->left_ && target->right_) {
      shared_ptr<AVLnode> successor = target->right_;
      shared_ptr<AVLnode> successor_parent;
      while (successor->right_)
        successor = successor->left_;
      successor_parent = successor->parent_.lock();

      if (successor->right_) {
        if (successor->is_left_child())
          AVLnode::set_left_child(successor_parent, successor->right_);
        else
          AVLnode::set_right_child(successor_parent, successor->right_);
      } else {
        if (successor->is_left_child())
          successor_parent->left_ = nullptr;
        else
          successor_parent->right_ = nullptr;
      }

      target->key_ = successor->key_;
      if (successor->is_left_child())
        retrace_deletion(successor_parent, -LEFT_HEAVY);
      else
        retrace_deletion(successor_parent, -RIGHT_HEAVY);
    } else {
      shared_ptr<AVLnode> orphan = (target->left_) ? target->left_ : target->right_;
      if (target == root_)
        root_ = orphan;
      else if (target->is_left_child())
        AVLnode::set_left_child(target->parent_.lock(), orphan);
      else
        AVLnode::set_right_child(target->parent_.lock(), orphan);
    }
  } else {
    if (target == root_)
      root_ = nullptr;
    else if (target->is_left_child()) {
      target->parent_.lock()->left_ = nullptr;
      retrace_deletion(target->parent_.lock(), -LEFT_HEAVY);
    } else {
      target->parent_.lock()->right_ = nullptr;
      retrace_deletion(target->parent_.lock(), -RIGHT_HEAVY);
    }
  }
  size_--;
}

shared_ptr<AVLnode> AVL::node_search(long int key) const {
  if (!root_)
    return nullptr;
  shared_ptr<AVLnode> current = root_;
  int height = 1;
  for (;;) {
    if (key < current->key_)
      if (current->left_) {
        current = current->left_;
        height++;
      } else
        return current;

    else if (key > current->key_)
      if (current->right_) {
        current = current->right_;
        height++;
      } else
        return current;

    else {
      current->height = height;
      return current;
    }
  }
}

void AVL::retrace_insertion(shared_ptr<AVLnode> node) {
  shared_ptr<AVLnode> current;
  shared_ptr<AVLnode> parent;
  for (current = node; current->parent_.lock() != nullptr; current = parent) {
    parent = current->parent_.lock();
    if (current->is_left_child()) {
      if (parent->balance_factor == LEFT_HEAVY) {
        if (current->balance_factor == RIGHT_HEAVY) {
          left_right(parent);
          return;
        } else {
          right_rotate(parent);
          return;
        }
      } else {
        parent->balance_factor += LEFT_HEAVY;
        if (parent->balance_factor == BALANCED) {
          return;
        } else
          continue;
      }
    } else {
      if (parent->balance_factor == RIGHT_HEAVY) {
        if (current->balance_factor == LEFT_HEAVY) {
          right_left(parent);
          return;
        } else {
          left_rotate(parent);
          return;
        }
      } else {
        parent->balance_factor += RIGHT_HEAVY;
        if (parent->balance_factor == BALANCED) {
          return;
        } else
          continue;
      }
    }
  }
}

void AVL::retrace_deletion(shared_ptr<AVLnode> subtree_root, int8_t balance_factor_change) {
  shared_ptr<AVLnode> current = subtree_root;
  shared_ptr<AVLnode> parent;

  if (current->balance_factor == BALANCED) {
    current->balance_factor += balance_factor_change;
    return;
  } else if ((current->balance_factor + balance_factor_change) != BALANCED) {
    if (balance_factor_change == LEFT_HEAVY) {
      if (subtree_root->left_->balance_factor == LEFT_HEAVY) {
        right_rotate(subtree_root);
      } else {
        left_right(subtree_root);
      }
    } else {
      if (subtree_root->right_->balance_factor == RIGHT_HEAVY) {
        left_rotate(subtree_root);
      } else {
        right_left(subtree_root);
      }
    }
    return;

  }

  for (current = subtree_root; current->parent_.lock() != nullptr; current = parent) {
    parent = current->parent_.lock();
    if (current->is_left_child()) {
      if (current->balance_factor == RIGHT_HEAVY) {
        if (current->balance_factor == RIGHT_HEAVY) {
          left_rotate(parent);
          return;
        } else {
          right_left(parent);
          return;
        }
      } else {
        parent->balance_factor -= LEFT_HEAVY;
        if (parent->balance_factor == BALANCED)
          return;
        else
          continue;
      }
    } else {
      if (parent->balance_factor == LEFT_HEAVY) {
        if (current->balance_factor == LEFT_HEAVY) {
          right_rotate(parent);
          return;
        } else {
          left_right(parent);
          return;
        }
      } else {
        parent->balance_factor -= RIGHT_HEAVY;
        if (parent->balance_factor == BALANCED)
          return;
        else
          continue;
      }
    }
  }

}

shared_ptr<AVLnode> AVL::left_rotate(shared_ptr<AVLnode> old_root) {
  shared_ptr<AVLnode> grandparent = old_root->parent_.lock();
  shared_ptr<AVLnode> new_subtreeroot = old_root->right_;
  shared_ptr<AVLnode> orphan = new_subtreeroot->left_;

  if (old_root->is_left_child())
    AVLnode::set_left_child(grandparent, new_subtreeroot);
  else if (old_root->is_right_child())
    AVLnode::set_right_child(grandparent, new_subtreeroot);
  else {
    root_ = new_subtreeroot;
    new_subtreeroot->parent_.reset();
  }
  AVLnode::set_left_child(new_subtreeroot, old_root);
  AVLnode::set_right_child(new_subtreeroot, old_root);

  if (old_root->balance_factor == BALANCED) {
    new_subtreeroot->balance_factor = RIGHT_HEAVY;
    old_root->balance_factor = LEFT_HEAVY;
  } else {
    new_subtreeroot->balance_factor = BALANCED;
    old_root->balance_factor = BALANCED;
  }
  return new_subtreeroot;
}

shared_ptr<AVLnode> AVL::right_rotate(shared_ptr<AVLnode> old_root) {
  shared_ptr<AVLnode> new_subtree_root = old_root->left_;
  shared_ptr<AVLnode> grandparent = old_root->parent_.lock();
  shared_ptr<AVLnode> orphan = new_subtree_root->right_;  // may be nullptr

  if (old_root->is_left_child())
    AVLnode::set_left_child(grandparent, new_subtree_root);
  else if (old_root->is_right_child())
    AVLnode::set_right_child(grandparent, new_subtree_root);
  else {
    root_ = new_subtree_root;
    new_subtree_root->parent_.reset();
  }
  AVLnode::set_right_child(new_subtree_root, old_root);
  AVLnode::set_left_child(old_root, orphan);

  if (old_root->balance_factor == BALANCED) {
    new_subtree_root->balance_factor = LEFT_HEAVY;
    old_root->balance_factor = RIGHT_HEAVY;
  } else {
    new_subtree_root->balance_factor = BALANCED;
    old_root->balance_factor = BALANCED;
  }
  return new_subtree_root;

}

shared_ptr<AVLnode> AVL::left_right(shared_ptr<AVLnode> old_root) {
  left_rotate(old_root->left_);
  return right_rotate(old_root);
}

shared_ptr<AVLnode> AVL::right_left(shared_ptr<AVLnode> old_root) {
  right_rotate(old_root->right_);
  return left_rotate(old_root);
}
size_t AVL::size() const {
  return size_;
}

bool AVL::empty() const {
  return size_ == 0;
}
std::string AVL::JSON() const {
  nlohmann::json result;
  std::queue<std::shared_ptr<AVLnode> > nodes;
  if (root_ != nullptr) {
    result["root"] = root_->key_;
    nodes.push(root_);
    while (!nodes.empty()) {
      auto v = nodes.front();
      nodes.pop();
      std::string key = std::to_string(v->key_);
      if (v->left_ != nullptr) {
        result[key]["left"] = v->left_->key_;
        nodes.push(v->left_);
      }
      if (v->right_ != nullptr) {
        result[key]["right"] = v->right_->key_;
        nodes.push(v->right_);
      }
      if (v->parent_.lock() != nullptr) {
        result[key]["parent"] = v->parent_.lock()->key_;
      } else {
        result[key]["root"] = true;
      }
    }
  }
  result["size"] = size_;
  return result.dump(2) + "\n";
}

AVL::AVL() {
  size_ = 0;
}

void AVL::DeleteMin() {
  DeleteMin(root_);
}

//Geeks for geeks deleteion method specified

//https://www.geeksforgeeks.org/avl-tree-set-2-deletion/
long int AVL::DeleteMin(shared_ptr<AVLnode> currentNode) {
  if ((root_->left_) || (root_->right_)) {
    shared_ptr<AVLnode> temp = root_->left_ ? root_->left_ : root_->right_;

    if (temp == nullptr) {
      temp = root_;
      root_ = nullptr;
    } else
      root_ = temp;
    temp.reset();
  } // else two children -- in-order succesion
  else {
    shared_ptr<AVLnode> successor = currentNode->right_;
    shared_ptr<AVLnode> successor_parent;
    while (successor->left_)
      successor = successor->left_;
    successor_parent = successor->parent_.lock();

    if (successor->right_) {
      if (successor->is_left_child())
        AVLnode::set_left_child(successor_parent, successor->right_);
      else
        AVLnode::set_right_child(successor_parent, successor->right_);
    } else {
      if (successor->is_left_child())
        successor_parent->left_ = nullptr;
      else
        successor_parent->right_ = nullptr;
    }
  }
  size_--;

}
void AVL::update(shared_ptr<AVLnode> node) {
  shared_ptr<AVLnode> temp = node_search(node->key_);
}
int AVL::height(shared_ptr<AVLnode> n) {
  if (n == nullptr)
    return 0;
  else
    n->height = std::max(height(n->left_), height(n->right_)) + 1;

  return n->height;
}

