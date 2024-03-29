//
// created by aman842 on 11/26/19.
//

#include <memory>
#include <string>

#define LEFT_HEAVY (1)
#define RIGHT_HEAVY (-1)
#define BALANCED (0)

class AVL;
using std::shared_ptr;
using std::make_shared;
using std::weak_ptr;

struct optional {
  // Construct without value
  optional() : _has_value(false) {}

  // Construct with value
  optional(int &value) : _value(value), _has_value(true) {}

  // Return true if the struct has a value.
  bool has_value() const { return _has_value; }

  // Get the value. Will still return reference to default constructed value if _has_value is false, so it's up to
  // the user to check if the structure contains a valid value before using it.
  long int &value() { return _value; }
  const int &value() const { return _value; }

 private:
  long int _value;
  bool _has_value;
};

class AVLnode {
 public:
  AVLnode(int key) : key_(key),
                     parent_(std::weak_ptr<AVLnode>()),
                     left_(nullptr),
                     right_(nullptr) {}

  AVLnode(int key, std::weak_ptr<AVLnode> parent) :
      key_(key),
      parent_(parent),
      left_(nullptr),
      right_(nullptr) {}

  bool is_left_child() const {
    shared_ptr<AVLnode> parent_sp = parent_.lock();
    if (parent_sp) {
      if (parent_sp->left_) {
        return key_ == parent_sp->left_->key_;
      }
    }
    return false;
  }
  bool is_right_child() const {
    shared_ptr<AVLnode> parent_sp = parent_.lock();
    if (parent_sp) {
      if (parent_sp->right_) {
        return key_ == parent_sp->right_->key_;
      }
    }
    return false;
  }

  //strangley never found a use for this.
  shared_ptr<AVLnode> sibiling() const {
    if (is_left_child())
      return parent_.lock()->left_;
    else
      return parent_.lock()->right_;
  }

  // function to correct balance if needed.
  void correct_balance() {
    if (left_ && right_) {
      if (abs(left_->balance_factor) == abs(right_->balance_factor))
        balance_factor = 0;
      else if (left_->balance_factor)
        balance_factor = LEFT_HEAVY;
      else
        balance_factor = RIGHT_HEAVY;
    } else if (left_)
      balance_factor = LEFT_HEAVY;
    else
      balance_factor = RIGHT_HEAVY;
  }

  static void set_left_child(shared_ptr<AVLnode> parent_node, shared_ptr<AVLnode> child) {
    parent_node->left_ = child;
    if (child) { child->parent_ = parent_node; }

  }

  static void set_right_child(shared_ptr<AVLnode> parent_node, shared_ptr<AVLnode> child) {
    parent_node->right_ = child;
    if (child) { child->parent_ = parent_node; }
  }

 private:
  int key_;
  std::weak_ptr<AVLnode> parent_;
  std::shared_ptr<AVLnode> left_;
  std::shared_ptr<AVLnode> right_;
  int8_t balance_factor;
  int height;

  friend AVL;
}; // class AVLnode

class AVL {
 public:
  AVL(); // default constructor

  void Insert(long int key);
  void Delete(long int key);
  optional Find(long int key) const;
  std::string JSON() const;
  size_t size() const;
  bool empty() const;
  void DeleteMin();
  int flipheight(); //i love my naming convention, basically a function to get maxdepth in a tree.

 private:
  void update(shared_ptr<AVLnode> node); //update the node's height
  long int DeleteMin(shared_ptr<AVLnode> currentNode); // i spent two days on this and delete thinking it would be required :( !
  shared_ptr<AVLnode> node_search(long int key) const; // function that iterates through tree to look for node->key
  void retrace_insertion(shared_ptr<AVLnode> node); // function to retrace insertion and balance out the avl tree
  void retrace_deletion(shared_ptr<AVLnode> subtree_root,
                        int8_t balance_factor); // function to retrace deletion which sadly..... is not used :(

  // Rotate functions !
  shared_ptr<AVLnode> left_rotate(shared_ptr<AVLnode> old_root);
  shared_ptr<AVLnode> right_rotate(shared_ptr<AVLnode> old_root);
  shared_ptr<AVLnode> left_right(shared_ptr<AVLnode> old_root);
  shared_ptr<AVLnode> right_left(shared_ptr<AVLnode> old_root);

  //Data functions
  std::shared_ptr<AVLnode> root_;
  size_t size_;

  //Height functions !
  int height(shared_ptr<AVLnode> n);
  int maxDepth(shared_ptr<AVLnode> node);
  void traverse(shared_ptr<AVLnode> root);

  //Testing functions
  bool check_balance(long int key);
  bool check_height(long int key);

}; // class AVL