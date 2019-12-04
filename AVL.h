#include <memory>
#include <string>

#define LEFT_HEAVY (1)
#define RIGHT_HEAVY (-1)
#define BALANCED (0)

class AVL;
using std::shared_ptr;
using std::make_shared;
using std::weak_ptr;

class AVL {
 public:
  struct node {
    int key;
    shared_ptr<node> left;
    shared_ptr<node> right;
    weak_ptr<node> parent;
    int balance;
    int height;
    node(int k) {
      height = 1;
      key = k;
      left = nullptr;
      right = nullptr;
    }

    node (int k, weak_ptr<node> parent):
      key(k),
      left(nullptr),
      right(nullptr),
      parent(parent) {}

    bool is_left_child () const {
      shared_ptr<node> parent_sp = parent.lock();
      if (parent_sp)
      {
        if (parent_sp->left)
        {
          return key == parent_sp->left->key;
        }
      }
      return false ;
    }
    bool is_right_child () const {
      shared_ptr<node> parent_sp = parent.lock();
      if (parent_sp)
      {
        if (parent_sp->right)
        {
          return key == parent_sp->right->key;
        }
      }
      return false;
    }
  };

  std::string JSON() const;
  size_t size() const;
  bool empty() const;
  size_t size_;
  AVL();

   int height(shared_ptr<node> head) {
    if(head == nullptr) return 0;
    return head->height;
  }

  void Insert(int x) {
    root_ = insertUtil(root_, x);
  }
  void inorder() {
    inorderUtil(root_);
  }
 private:
  shared_ptr<node> root_;



  shared_ptr<node> rightRotation (shared_ptr<node> head) ;
  shared_ptr<node> leftRotation (shared_ptr<node> head) ;

  void retrace_insertion(shared_ptr<node> node_) {
    shared_ptr<node> current;
    shared_ptr<node> parent;
    for (current = node_; current->parent.lock() != nullptr; current = parent) {
      parent = current->parent.lock();
      if (current->is_left_child()) {
        if (parent->balance == LEFT_HEAVY) {
          if (current->balance == RIGHT_HEAVY) {
            left_right(parent);
          } else {
            rightRotation(parent);
            return;
          }
        } else {
          parent->balance += LEFT_HEAVY;
          if (parent->balance == BALANCED) {
            return;
          } else
            continue;
        }
      } else {
        if (parent->balance == RIGHT_HEAVY) {
          if (current->balance == LEFT_HEAVY) {
            right_left(parent);
            return;
          } else {
            leftRotation(parent);
            return;
          }
        } else {
          parent->balance += RIGHT_HEAVY;
          if (parent->balance == BALANCED) {
            return;
          } else
            continue;
        }
      }
    }
  }

  shared_ptr<node> left_right(shared_ptr<node> old_root) {
    leftRotation(old_root->left);
    return rightRotation(old_root);
  }

  shared_ptr<node> right_left(shared_ptr<node> old_root) {
    rightRotation(old_root);
    return leftRotation(old_root);
  }

  void inorderUtil(shared_ptr<node> head);

  shared_ptr<node> insertUtil(shared_ptr<node> head, int x) {
    if (head == nullptr) {
      head = make_shared<node>(x);
      return head;
    }

    if ( x < head->key){
      head->left = make_shared<node>(x, head->left);
      head->left = insertUtil(head->left, x);
     // retrace_insertion(head->left);
    }else if(x > head->key) {
      head->right = make_shared<node>(x, head->right);
      head->right = insertUtil(head->right, x);
     // retrace_insertion(head->right);
    }
    head->height = 1 + std::max(AVL::height(head->left), AVL::height(head->right));
    head->balance = AVL::height(head->left) - AVL::height(head->right);
    if (head->balance > 1) {
      if (x < head->left->key){
        return AVL::rightRotation(head);
      } else {
        head->left = leftRotation(head->left);
        return rightRotation(head);
      }
  } else if (head->balance < -1) {
      if (x > head->right->key){
        return leftRotation(head);
      } else {
        head->right = rightRotation(head->right);
        return leftRotation(head);
      }
    }
    return head;
  }

};