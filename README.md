# cpp-avl-tree
C++ avl tree using smart pointers. 
This is an AVL tree implementation - a binary search tree that self balances. It uses C++11, namely smart pointers to showcases it wonderful new features, to manage memory and deletion of objects stored in the tree. There is also a very basic "optiona" type to handle search cases where handle cases of not finding the key. 

## Usage ##
An example of it being used can be found in [AVLCommands.cxx](AVLCommands.cxx) where it uses instructions provided by a json file. 
Essentially, you just include the header and declare an object to start using it:
```
#include "avltree.h"
...
  AVL<int> tree;
```

You can *Insert*, *Delete* and *Delete the Minimum* of the tree:
```
  tree.Insert(1);
  tree.Delete(1);
  tree.DeleteMin();
  ```
*Searching* is simple enough with the Search Method which calls upon the node_search method:
```
  const optional<int> &result = tree.search(1);
  if(result != nullptr)
    cout << "Key is in the tree!" << std::endl;
   
  Output:
    Key is in the tree
 ```
 
 ## Testing ##
 I am not promising future updation of this project, but have included some basic tests in the [BSTSanityCheck.cxx](BSTSanityCheck.cxx) which goes over the *insertion*, *deletion* and *updation* of key's. No specific consideration of space and speed optimization is endorsed here too, other than the use of the good ol 'valgrind'.
 
