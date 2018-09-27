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

#include <iostream>

namespace bst_node {

template <typename T>
struct Tree_node {
    T _item;
    Tree_node<T>* _left;
    Tree_node<T>* _right;
    int _height;
    // int balance_factor() {
    //     // balance factor = height of the left subtree
    //     //                        - the height of the right subtree
    //     // a NULL child has a height of -1
    //     // a leaf has a height of 0
    // }

    // int height() {
    //     // Height of a node is 1 + height of the "taller" child
    //     // A leaf node has a height of zero: 1 + max(-1,-1)
    // }
    // int update_height() {
    //     // set the _height member variable (call height();)
    // }

    Tree_node(T item = T(), Tree_node* left = NULL, Tree_node* right = NULL)
        : _item(item), _left(left), _right(right), _height(-1) {
        // don't forget to set the _height.
    }
    friend std::ostream& operator<<(std::ostream& outs,
                                    const Tree_node<T>& t_node) {
        return outs << "|" << t_node._item << "|";
    }
};

template <typename T>  // add a node in the binary tree
void tree_insert(Tree_node<T>*& root, const T& insert_me);

template <typename T>  // search tree and return node if found
Tree_node<T>* tree_search(Tree_node<T>* root, const T& target);

template <typename T>  // search tree and return node plus boolean if found
bool tree_search(Tree_node<T>* root, const T& target, Tree_node<T>*& found_ptr);

template <typename T>  // print tree rotated 90 degree counterclockwise
void tree_print(Tree_node<T>* root, int level = 0,
                std::ostream& outs = std::cout);

template <typename T>  // prints details info about each node
void tree_print_debug(Tree_node<T>* root, int level = 0,
                      std::ostream& outs = std::cout);

template <typename T>  // clear the tree
void tree_clear(Tree_node<T>*& root);

template <typename T>  // erase target from the tree
bool tree_erase(Tree_node<T>*& root, const T& target);

template <typename T>  // erase rightmost node from the tree
                       // store the item in max_value
void tree_remove_max(Tree_node<T>*& root, T& max_value);

template <typename T>  // return copy of tree pointed to by root
Tree_node<T>* tree_copy(Tree_node<T>* root);

template <typename T>  // add tree src to dest
void tree_add(Tree_node<T>*& dest, const Tree_node<T>* src);

template <typename T>  // sorted array -> tree
Tree_node<T>* tree_from_sorted_list(const T* a, int size);

template <typename T>
void tree_insert(Tree_node<T>*& root, const T& insert_me) {
    if(!root) {  // base: root is nullptr
        root = new Tree_node<T>(insert_me);
        return;
    }

    if(insert_me < root->_item)
        tree_insert(root->_left, insert_me);  // recurve left node
    else
        tree_insert(root->_right, insert_me);  // recurve right node
}

template <typename T>
Tree_node<T>* tree_search(Tree_node<T>* root, const T& target) {
    if(!root) return nullptr;  // base: root is nullptr

    if(target == root->_item) return root;  // return current root if found

    if(target < root->_item)
        return tree_search(root->_left, target);  // recurve left node
    else
        return tree_search(root->_right, target);  // recurve right node
}

template <typename T>
bool tree_search(Tree_node<T>* root, const T& target,
                 Tree_node<T>*& found_ptr) {
    if(!root) return false;  // base: root is nullptr

    if(target == root->_item) {  // return true/current root if found
        found_ptr = root;
        return true;
    }

    if(target < root->_item)
        return tree_search(root->_left, target);  // recurve left node
    else
        return tree_search(root->_right, target);  // recurve right node
}

template <typename T>
void tree_print(Tree_node<T>* root, int level, std::ostream& outs) {
    if(!root) {  // base: root is nullptr
        outs << std::string(10 * level, ' ') << "|||" << std::endl;
        return;
    }

    tree_print(root->_right, level + 1, outs);  // recurve right
    outs << std::string(10 * level, ' ') << *root << std::endl;  // print root
    tree_print(root->_left, level + 1, outs);                    // recurve left
}

template <typename T>
void tree_clear(Tree_node<T>*& root) {
    if(root) {
        tree_clear(root->_left);   // recurve left node
        tree_clear(root->_right);  // recurve right node

        delete root;     // delete current root
        root = nullptr;  // reset ptr to nullptr
    }
}

template <typename T>
bool tree_erase(Tree_node<T>*& root, const T& target) {
    if(!root) return false;  // base: root is nullptr

    if(target == root->_item) {
        tree_clear(root);
        return true;
    }

    if(target < root->_item)
        return tree_erase(root->_left, target);  // recurve left node
    else
        return tree_erase(root->_right, target);  // recurve right node
}

template <typename T>
Tree_node<T>* tree_copy(Tree_node<T>* root) {
    if(!root) return nullptr;  // base: root is nullptr

    return new Tree_node<T>(root->_item, tree_copy(root->_left),
                            tree_copy(root->_right));
}

template <typename T>
void tree_add(Tree_node<T>*& dest, const Tree_node<T>* src) {
    if(dest != src) {               // copy if dest is not src
        if(dest) tree_clear(dest);  // clear dest if not empty
        dest = tree_copy(src);      // copy src to dest
    }
}

template <typename T>
Tree_node<T>* tree_from_sorted_list(const T* a, int size) {
    if(!size) return nullptr;  // base: size is 0

    int midpoint = size / 2;

    if(size & 1)  // when size is odd
        return new Tree_node<T>(
            a[midpoint], tree_from_sorted_list(a, midpoint),
            tree_from_sorted_list(a + midpoint + 1, midpoint));
    else  // when size is even
        return new Tree_node<T>(
            a[midpoint], tree_from_sorted_list(a, midpoint),
            tree_from_sorted_list(a + midpoint + 1, midpoint - 1));
}

}  // namespace bst_node

#endif  // BST_NODE_H
