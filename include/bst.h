/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : my_queue
 * DESCRIPTION : This header defines a templated BST (binary search tree) and
 *      its associated functions. The BST can insert and erase a
 *bst_node::Tree_node at target.
 ******************************************************************************/
#ifndef BST_H
#define BST_H

#include "bst_node.h"

namespace bst {

template <typename T>
class BST {
public:
    // CONSTRUCTORS
    BST() : _root(nullptr){};
    BST(const T* sorted_list, int size = 0);

    // BIG THREE
    BST(const BST<T>& copy_me);
    BST<T>& operator=(const BST<T>& rhs);
    ~BST();

    // ACCESSORS
    bool search(const T& target, bst_node::Tree_node<T>*& found_ptr) const;

    // MUTATORS
    void insert(const T& insert_me);
    void erase(const T& target);

    // FRIENDS
    friend std::ostream& operator<<(std::ostream& outs, const BST<T>& tree) {
        bst_node::tree_print(tree->_root);
        return outs;
    }
    friend BST<T>& operator+=(BST<T>& lhs, const BST<T>& rhs) {
        bst_node::tree_add(lhs->_root, rhs->_root);
        return lhs;
    }

private:
    bst_node::Tree_node<T>* _root;
};

template <typename T>
BST<T>::BST(const T* sorted_list, int size) {
    _root = bst_node::tree_from_sorted_list(sorted_list, size);
}

template <typename T>
BST<T>::BST(const BST<T>& copy_me) {
    _root = bst_node::tree_copy(_root, copy_me);  // copy copy_me to _root
}

template <typename T>
BST<T>& BST<T>::operator=(const BST<T>& rhs) {
    if(this != &rhs) {
        if(_root) bst_node::tree_clear(_root);    // deallocate old data
        _root = bst_node::tree_copy(_root, rhs);  // copy rhs to _root
    }
}

template <typename T>
BST<T>::~BST() {
    bst_node::tree_clear(_root);  // deallocate old data
}

template <typename T>
bool BST<T>::search(const T& target, bst_node::Tree_node<T>*& found_ptr) const {
    return bst_node::tree_search(_root, target, found_ptr);
}

template <typename T>
void BST<T>::insert(const T& insert_me) {
    bst_node::tree_insert(_root, insert);
}

template <typename T>
void BST<T>::erase(const T& target) {
    bst_node::tree_erase(_root, target);
}

}  // namespace bst

#endif  // BST_H