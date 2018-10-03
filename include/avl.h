/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : avl
 * DESCRIPTION : This header defines a templated AVL (Adelson-Velsky and
 *      Landis), which is a balanced version of templated BST. Insertions and
 *      erasures of TreeNodes will guarantee a full or complete binary tree.
 ******************************************************************************/
#ifndef AVL_H
#define AVL_H

#include "bst_node.h"

namespace avl {

template <typename T>
class AVL {
public:
    // CONSTRUCTORS
    AVL() : _root(nullptr) {}
    AVL(const T* sorted_list, int size = -1);  // with array of sorted items

    // BIG THREE
    AVL(const AVL<T>& copy_me);
    AVL<T>& operator=(const AVL<T>& rhs);
    ~AVL();

    // ACCESSORS
    bool search(const T& target, bst_node::TreeNode<T>*& found_ptr) const;

    // MUTATORS
    void clear();
    bool erase(const T& target);
    bool insert(const T& insert_me);

    // FRIENDS
    friend std::ostream& operator<<(std::ostream& outs, const AVL<T>& tree) {
        bst_node::tree_print(tree._root, outs);
        return outs;
    }
    friend AVL<T>& operator+=(AVL<T>& lhs, const AVL<T>& rhs) {
        bst_node::tree_add(lhs._root, rhs._root, true);
        return lhs;
    }

private:
    bst_node::TreeNode<T>* _root;
};

template <typename T>
AVL<T>::AVL(const T* sorted_list, int size) {
    _root = bst_node::tree_from_sorted_list(sorted_list, size);
}

template <typename T>
AVL<T>::AVL(const AVL<T>& copy_me) {
    _root = bst_node::tree_copy(copy_me._root, true);  // copy copy_me to _root
}

template <typename T>
AVL<T>& AVL<T>::operator=(const AVL<T>& rhs) {
    if(this != &rhs) {
        if(_root) bst_node::tree_clear(_root);         // deallocate old data
        _root = bst_node::tree_copy(rhs._root, true);  // copy rhs to _root
    }

    return *this;
}

template <typename T>
AVL<T>::~AVL() {
    bst_node::tree_clear(_root);  // deallocate old data
}

template <typename T>
bool AVL<T>::search(const T& target, bst_node::TreeNode<T>*& found_ptr) const {
    return bst_node::tree_search(_root, target, found_ptr);
}

template <typename T>
void AVL<T>::clear() {
    bst_node::tree_clear(_root);  // deallocate old data
}

template <typename T>
bool AVL<T>::erase(const T& target) {
    return bst_node::tree_erase(_root, target, true);
}

template <typename T>
bool AVL<T>::insert(const T& insert_me) {
    return bst_node::tree_insert(_root, insert_me, true);
}

}  // namespace avl

#endif  // AVL_H
