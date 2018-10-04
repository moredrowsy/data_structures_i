/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : bst_node
 * DESCRIPTION : This header defines a templated binary search tree node and
 *      templated functions that will process the nodes. These functions are
 *      the base to build the binary search tree class BST.
 ******************************************************************************/
#ifndef BST_NODE_H
#define BST_NODE_H

#include <algorithm>
#include <iostream>

namespace bst_node {

template <typename T>
struct TreeNode {
    T _item;
    TreeNode<T>* _left;
    TreeNode<T>* _right;
    int _height;

    // CONSTRUCTORS
    TreeNode(T item = T(), TreeNode* left = nullptr, TreeNode* right = nullptr)
        : _item(item), _left(left), _right(right), _height(update_height()) {}

    // ACCESSORS
    int balance_factor() const;
    int height() const;

    // MUTATORS
    int update_height();

    // FRIENDS
    friend std::ostream& operator<<(std::ostream& outs,
                                    const TreeNode<T>& t_node) {
        return outs << "|" << t_node._item << "|";
    }
};

template <typename T>  // add a node in the binary tree
bool tree_insert(TreeNode<T>*& root, const T& insert_me, bool balance = false);

template <typename T>  // search tree and return node if found
TreeNode<T>* tree_search(TreeNode<T>* root, const T& target);

template <typename T>  // search tree and return node plus boolean if found
bool tree_search(TreeNode<T>* root, const T& target, TreeNode<T>*& found_ptr);

template <typename T>  // print tree rotated 90 degrees counterclockwise
void tree_print(TreeNode<T>* root, std::ostream& outs = std::cout,
                int level = 0);

template <typename T>  // prints details info about each node
void tree_print_debug(TreeNode<T>* root, std::ostream& outs = std::cout,
                      int level = 0);

template <typename T>  // clear the tree
void tree_clear(TreeNode<T>*& root);

template <typename T>  // erase target from the tree
bool tree_erase(TreeNode<T>*& root, const T& target, bool balance = false);

template <typename T>  // erase rightmost node from the tree
                       // store the item in max_value
void tree_remove_max(TreeNode<T>*& root, T& max_value, bool balance = false);

template <typename T>  // return copy of tree pointed to by root
TreeNode<T>* tree_copy(TreeNode<T>* root, bool balance = false);

template <typename T>  // add tree src to dest
void tree_add(TreeNode<T>*& dest, const TreeNode<T>* src, bool balance = false);

template <typename T>  // sorted array -> tree
TreeNode<T>* tree_from_sorted_list(const T* a, int size);

template <typename T>  // root becomes child of left node
TreeNode<T>* rotate_left(TreeNode<T>*& root);

template <typename T>  // root becomes child of right node
TreeNode<T>* rotate_right(TreeNode<T>*& root);

template <typename T>  // decide which rotate is needed based on balance factor
TreeNode<T>* rotate(TreeNode<T>*& root);

template <typename T, typename F>  // traversal of node with function pointer
void inorder(TreeNode<T>*& root, F f);

template <typename T, typename F>  // traversal of node with function pointer
void preorder(TreeNode<T>*& root, F f);

template <typename T, typename F>  // traversal of node with function pointer
void postorder(TreeNode<T>*& root, F f);

/*******************************************************************************
 * DESCRIPTION:
 *  Calculates the balance factor: right's height - left's height.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  int: balance factor of node
 ******************************************************************************/
template <typename T>
int TreeNode<T>::balance_factor() const {
    return (this->_right ? this->_right->_height : -1) -
           (this->_left ? this->_left->_height : -1);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Pick maximum height of children node + 1.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  int: max height of children
 ******************************************************************************/
template <typename T>
int TreeNode<T>::height() const {
    return 1 + std::max(this->_left ? this->_left->_height : -1,
                        this->_right ? this->_right->_height : -1);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Updates _height via height()
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  int _height: height is updated
 *
 * RETURN:
 *  int: max height of children
 ******************************************************************************/
template <typename T>
int TreeNode<T>::update_height() {
    return _height = height();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Inserts an unique node in ascending order. If balance is true, will rotate
 *  tree to full or complete binary tree.
 *
 * PRE-CONDITIONS:
 *  TreeNode<T>*& root: root by reference
 *  const T& insert_me: target item
 *  bool balance      : rotate nodes to balance tree
 *
 * POST-CONDITIONS:
 *  TreeNode<T>*& root: node added to root
 *
 * RETURN:
 *  bool: insertion success/failure
 ******************************************************************************/
template <typename T>
bool tree_insert(TreeNode<T>*& root, const T& insert_me, bool balance) {
    bool is_insert = false;

    if(!root) {
        root = new TreeNode<T>(insert_me);
        is_insert = true;
    } else if(insert_me < root->_item)
        is_insert = tree_insert(root->_left, insert_me, balance);  // --> left
    else if(insert_me > root->_item)
        is_insert = tree_insert(root->_right, insert_me, balance);  // --> right

    root->update_height();
    if(balance) root = rotate(root);

    return is_insert;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Searches root node for given target. Search failure returns nullptr, else
 *  returns target node.
 *
 * PRE-CONDITIONS:
 *  TreeNode<T>*& root: root by value
 *  const T& insert_me: target item
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  TreeNode<T>*: nullptr or target
 ******************************************************************************/
template <typename T>
TreeNode<T>* tree_search(TreeNode<T>* root, const T& target) {
    // base: nullptr or target, return root (root = nullptr if not found)
    if(!root || target == root->_item) return root;

    if(target < root->_item)
        return tree_search(root->_left, target);  // recur left node
    else
        return tree_search(root->_right, target);  // recur right node
}

/*******************************************************************************
 * DESCRIPTION:
 *  Searches root node for given target. Search failure turns nullptr with
 *  boolean, else returns target node with boolean.
 *
 * PRE-CONDITIONS:
 *  TreeNode<T>*& root     : root by value
 *  const T& insert_me     : target item
 *  TreeNode<T>*& found_ptr: destination pointer for target node
 *
 * POST-CONDITIONS:
 *  TreeNode<T>*& found_ptr: node to target or nullptr
 *
 * RETURN:
 *  boolean: search success/failure
 ******************************************************************************/
template <typename T>
bool tree_search(TreeNode<T>* root, const T& target, TreeNode<T>*& found_ptr) {
    // base: nullptr or target, return bool, set found_ptr
    if(!root || target == root->_item) {
        found_ptr = root;
        return root ? true : false;
    }

    if(target < root->_item)
        return tree_search(root->_left, target, found_ptr);  // recur left
    else
        return tree_search(root->_right, target, found_ptr);  // recur right
}

/*******************************************************************************
 * DESCRIPTION:
 *  Prints node by reverse inorder traversal.
 *
 * PRE-CONDITIONS:
 *  TreeNode<T>* root : root by value
 *  std::ostream& outs: out stream
 *  int level         : recursion level for formatting
 *
 * POST-CONDITIONS:
 *  out stream insertions
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void tree_print(TreeNode<T>* root, std::ostream& outs, int level) {
    if(!root) {  // base: root is nullptr
        outs << std::string(5 * level, ' ') << "|||" << std::endl;
        return;
    }

    tree_print(root->_right, outs, level + 1);  // recur right
    outs << std::string(5 * level, ' ') << *root << std::endl;
    tree_print(root->_left, outs, level + 1);  // recur left
}

/*******************************************************************************
 * DESCRIPTION:
 *  Prints node by reverse inorder traversal with debug info: height and
 *  balance factor.
 *
 * PRE-CONDITIONS:
 *  TreeNode<T>* root : root by value
 *  std::ostream& outs: out stream
 *  int level         : recursion level for formatting
 *
 * POST-CONDITIONS:
 *  out stream insertions
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>  // prints details info about each node
void tree_print_debug(TreeNode<T>* root, std::ostream& outs, int level) {
    if(!root) {  // base: root is nullptr
        outs << std::string(5 * level, ' ') << "|||" << std::endl;
        return;
    }

    tree_print_debug(root->_right, outs, level + 1);  // recur right
    outs << std::string(5 * level, ' ') << *root << " <h" << root->_height
         << ", b" << root->balance_factor() << ">" << std::endl;
    tree_print_debug(root->_left, outs, level + 1);  // recur left
}

/*******************************************************************************
 * DESCRIPTION:
 *  Deallocates all nodes linked to root.
 *
 * PRE-CONDITIONS:
 *  TreeNode<T>*& root: root by reference
 *
 * POST-CONDITIONS:
 *  TreeNode<T>*& root: set to nullptr when all nodes deallocated
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void tree_clear(TreeNode<T>*& root) {
    if(root) {
        tree_clear(root->_left);   // recur left node
        tree_clear(root->_right);  // recur right node

        delete root;     // delete current root
        root = nullptr;  // reset ptr to nullptr
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Erase a target node by traversing the bst root. If balance is true, will
 *  rotate tree to full or complete binary tree.
 *
 * PRE-CONDITIONS:
 *  TreeNode<T>*& root: root by reference
 *  const T& insert_me: target item
 *  bool balance      : rotate nodes to balance tree
 *
 * POST-CONDITIONS:
 *  TreeNode<T>*& root: node erased from root
 *
 * RETURN:
 *  bool: erasure success/failure
 ******************************************************************************/
template <typename T>
bool tree_erase(TreeNode<T>*& root, const T& target, bool balance) {
    bool is_erased = false;

    if(!root) return is_erased;  // base: root is nullptr

    // erase found target
    if(target == root->_item) {
        if(!root->_left) {
            TreeNode<T>* pop = root;
            root = root->_right;  // root = nullptr when !root->_right

            delete pop;
            pop = nullptr;
        } else {
            T max;
            tree_remove_max(root->_left, max, balance);
            root->_item = max;
        }

        is_erased = true;
    } else if(target < root->_item)
        is_erased = tree_erase(root->_left, target, balance);  // recur left
    else
        is_erased = tree_erase(root->_right, target, balance);  // recur right

    if(root) {  // root == nullptr when target is max node
        root->update_height();
        if(balance) root = rotate(root);
    };

    return is_erased;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Removes the largest node in the bst root and return max item. If balance
 *  is true, will rotate tree to full or complete binary tree.
 *
 * PRE-CONDITIONS:
 *  TreeNode<T>*& root: root by reference
 *  T& max_value      : place holder for max item if successful remove
 *  bool balance      : rotate nodes to balance tree
 *
 * POST-CONDITIONS:
 *  TreeNode<T>*& root: max node erased from root
 *  T& max_value      : max item is set to removed item
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void tree_remove_max(TreeNode<T>*& root, T& max_value, bool balance) {
    if(!root) return;  // base: root is nullptr

    if(!root->_right) {
        max_value = root->_item;

        TreeNode<T>* pop = root;
        root = root->_left;

        delete pop;
        pop = nullptr;

        return;
    }

    tree_remove_max(root->_right, max_value, balance);  // recur right

    root->update_height();
    if(balance) root = rotate(root);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Copys all nodes from source root and returns destination root. If balance
 *  is true, will rotate tree to full or complete binary tree.
 *
 * PRE-CONDITIONS:
 *  TreeNode<T>*& root: source root
 *  bool balance      : rotate nodes to balance tree
 *
 * POST-CONDITIONS:
 *  all nodes copied to returned pointer
 *
 * RETURN:
 *  TreeNode<T>*: root with all newly copied nodes, else
 ******************************************************************************/
template <typename T>
TreeNode<T>* tree_copy(TreeNode<T>* root, bool balance) {
    if(root) {
        root = new TreeNode<T>(root->_item, tree_copy(root->_left, balance),
                               tree_copy(root->_right, balance));

        if(balance) root = rotate(root);
    }

    return root;  // base: root is nullptr, else return new root
}

/*******************************************************************************
 * DESCRIPTION:
 *  Add all nodes from a source node to destination node by preorder
 *  traversal of source node. If balance is true, will rotate tree to full or
 *  complete binary tree.
 *
 * PRE-CONDITIONS:
 *  TreeNode<T>*& root    : destination root by reference
 *  const TreeNode<T>* src: source root
 *  bool balance          : rotate nodes to balance tree
 *
 * POST-CONDITIONS:
 *  TreeNode<T>*& root: all nodes from source added to destination root
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void tree_add(TreeNode<T>*& dest, const TreeNode<T>* src, bool balance) {
    if(src) {
        tree_insert(dest, src->_item, balance);
        tree_add(dest, src->_left, balance);   // recur left
        tree_add(dest, src->_right, balance);  // recur right
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Creates a full or complete binary tree from array of sorted items by
 *  ascending order.
 *  Mechanism: Create new node by midpoint element, and recur on lower and
 *             upper portion of array.
 *
 * PRE-CONDITIONS:
 *  const T* a: array of sorted items by ascending order
 *  int size  : array size
 *
 * POST-CONDITIONS:
 *  Allocation of all nodes from array of sorted items
 *
 * RETURN:
 *  TreeNode<T>*: bst node with items from array as full/complete binary tree
 ******************************************************************************/
template <typename T>
TreeNode<T>* tree_from_sorted_list(const T* a, int size) {
    if(size < 1) return nullptr;  // base: size is 0 or negative

    return new TreeNode<T>(
        a[size / 2], tree_from_sorted_list(a, size / 2),
        tree_from_sorted_list(a + (size / 2) + 1, (size - 1) / 2));
}

/*******************************************************************************
 * DESCRIPTION:
 *  Rotate the root to the left, by making the root the child of the right
 *  node.
 *  Mechanism: Store top root and right root. Point root to the right, point
 *             top's right to right's left and point right's left to top.
 *
 * PRE-CONDITIONS:
 *  TreeNode<T>*& root: root by reference
 *
 * POST-CONDITIONS:
 *  original top root shifted to left
 *
 * RETURN:
 *  TreeNode<T>*: new top root
 ******************************************************************************/
template <typename T>
TreeNode<T>* rotate_left(TreeNode<T>*& root) {
    TreeNode<T>*top = root, *right = root->_right;

    root = right;
    top->_right = right->_left;
    right->_left = top;

    top->update_height();
    right->update_height();

    return root;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Rotate the root to the right, by making the root the child of the left
 *  node.
 *  Mechanism: Store top root and left root. Point root to the left, point
 *             top's left to left's right and point left's right to top.
 *
 * PRE-CONDITIONS:
 *  TreeNode<T>*& root: root by reference
 *
 * POST-CONDITIONS:
 *  original top root shifted to right
 *
 * RETURN:
 *  TreeNode<T>*: new top root
 ******************************************************************************/
template <typename T>
TreeNode<T>* rotate_right(TreeNode<T>*& root) {
    TreeNode<T>*top = root, *left = root->_left;

    root = left;
    top->_left = left->_right;
    left->_right = top;

    top->update_height();
    left->update_height();

    return root;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Balances root with specific rotations to produce full or complete binary
 *  tree.
 *
 * PRE-CONDITIONS:
 *  TreeNode<T>*& root: root by reference
 *
 * POST-CONDITIONS:
 *  nodes shifted to produce full or complete binarytree
 *
 * RETURN:
 *  TreeNode<T>*: new top root
 ******************************************************************************/
template <typename T>
TreeNode<T>* rotate(TreeNode<T>*& root) {
    if(root->balance_factor() == -2) {
        if(root->_left->balance_factor() == 1)
            root->_left = rotate_left(root->_left);
        root = rotate_right(root);
    }

    if(root->balance_factor() == 2) {
        if(root->_right->balance_factor() == -1)
            root->_right = rotate_right(root->_right);
        root = rotate_left(root);
    }

    return root;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Inorder traversal of node with function pointer to process root.
 *
 * PRE-CONDITIONS:
 *  TreeNode<T>*& root: root by reference
 *  F f               : function pointer to process root
 *
 * POST-CONDITIONS:
 *  Dependent on function pointer processing root
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename F>
void inorder(TreeNode<T>*& root, F f) {
    if(root) {
        inorder(root->_left, f);
        f(root);
        inorder(root->_right, f);
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Preorder traversal of node with function pointer to process root.
 *
 * PRE-CONDITIONS:
 *  TreeNode<T>*& root: root by reference
 *  F f               : function pointer to process root
 *
 * POST-CONDITIONS:
 *  Dependent on function pointer processing root
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename F>
void preorder(TreeNode<T>*& root, F f) {
    if(root) {
        f(root);
        preorder(root->_left, f);
        preorder(root->_right, f);
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Postorder traversal of node with function pointer to process root.
 *
 * PRE-CONDITIONS:
 *  TreeNode<T>*& root: root by reference
 *  F f               : function pointer to process root
 *
 * POST-CONDITIONS:
 *  Dependent on function pointer processing root
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename F>
void postorder(TreeNode<T>*& root, F f) {
    if(root) {
        postorder(root->_left, f);
        postorder(root->_right, f);
        f(root);
    }
}

}  // namespace bst_node

#endif  // BST_NODE_H
