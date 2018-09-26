#ifndef BST_NODE_H
#define BST_NODE_H

#include <iostream>

namespace bst_node {

template <typename T>
struct tree_node {
    T _item;
    tree_node<T>* _left;
    tree_node<T>* _right;
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

    tree_node(T item = T(), tree_node* left = NULL, tree_node* right = NULL)
        : _item(item), _left(left), _right(right), _height(-1) {
        // don't forget to set the _height.
    }
    friend std::ostream& operator<<(std::ostream& outs,
                                    const tree_node<T>& t_node) {
        return outs << "|" << t_node._item << "|";
    }
};

template <typename T>  // add a node in the binary tree
void tree_insert(tree_node<T>*& root, const T& insert_me);

template <typename T>  // search tree and return node if found
tree_node<T>* tree_search(tree_node<T>* root, const T& target);

template <typename T>  // search tree and return node plus boolean if found
bool tree_search(tree_node<T>* root, const T& target, tree_node<T>*& found_ptr);

template <typename T>  // print tree rotated 90 degree counterclockwise
void tree_print(tree_node<T>* root, int level = 0,
                std::ostream& outs = std::cout);

template <typename T>  // prints details info about each node
void tree_print_debug(tree_node<T>* root, int level = 0,
                      std::ostream& outs = std::cout);

template <typename T>  // clear the tree
void tree_clear(tree_node<T>*& root);

template <typename T>  // erase target from the tree
bool tree_erase(tree_node<T>*& root, const T& target);

template <typename T>  // erase rightmost node from the tree
                       // store the item in max_value
void tree_remove_max(tree_node<T>*& root, T& max_value);

template <typename T>  // return copy of tree pointed to by root
tree_node<T>* tree_copy(tree_node<T>* root);

template <typename T>  // Add tree src to dest
void tree_add(tree_node<T>*& dest, const tree_node<T>* src);

template <typename T>  // sorted array -> tree
tree_node<T>* tree_from_sorted_list(const T* a, int size);

template <typename T>
void tree_insert(tree_node<T>*& root, const T& insert_me) {
    if(!root) {  // add node to root if root is nullptr
        root = new tree_node<T>(insert_me);
        return;
    }

    if(insert_me < root->_item)
        tree_insert(root->_left, insert_me);  // recurve left node
    else
        tree_insert(root->_right, insert_me);  // recurve right node
}

template <typename T>
tree_node<T>* tree_search(tree_node<T>* root, const T& target) {
    if(!root) return nullptr;  // return nullptr if not found

    if(target == root->_item) return root;  // return current root if found

    if(target < root->_item)
        return tree_search(root->_left, target);  // recurve left node
    else
        return tree_search(root->_right, target);  // recurve right node
}

template <typename T>
bool tree_search(tree_node<T>* root, const T& target,
                 tree_node<T>*& found_ptr) {
    if(!root) return false;  // return false if not found

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
void tree_print(tree_node<T>* root, int level, std::ostream& outs) {
    if(!root) {  // print three pipes if nullptr
        outs << std::string(10 * level, ' ') << "|||" << std::endl;
        return;
    }

    tree_print(root->_right, level + 1, outs);  // recurve right
    outs << std::string(10 * level, ' ') << *root << std::endl;  // print root
    tree_print(root->_left, level + 1, outs);                    // recurve left
}

template <typename T>
void tree_clear(tree_node<T>*& root) {
    if(root) {
        tree_clear(root->_left);   // recurve left node
        tree_clear(root->_right);  // recurve right node

        delete root;  // delete current root
    }
}

template <typename T>
bool tree_erase(tree_node<T>*& root, const T& target) {
    if(!root) return false;  // return false if not found

    if(target == root->_item) {
        tree_clear(root);
        return true;
    }

    if(target < root->_item)
        return tree_erase(root->_left, target);  // recurve left node
    else
        return tree_erase(root->_right, target);  // recurve right node
}

}  // namespace bst_node

#endif  // BST_NODE_H
