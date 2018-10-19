/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : bst
 * DESCRIPTION : This header defines a templated BST (Binary Search Tree).
 *      Insertions and erasures of TreeNodes will not guarantee a full or
 *      complete binary tree.
 ******************************************************************************/
#ifndef BST_H
#define BST_H

#include <cassert>     // assert()
#include "bst_node.h"  // BST TreeNode class

namespace bst {

template <typename T>
class BST {
public:
    // CONSTRUCTORS
    BST() : _root(nullptr) {}
    BST(const T* sorted_list, int size = -1);  // with array of sorted items

    // BIG THREE
    BST(const BST<T>& copy_me);
    BST<T>& operator=(const BST<T>& rhs);
    ~BST();

    // ACCESSORS
    bool empty() const;
    T& front() const;
    const bst_node::TreeNode<T>* root() const;
    bool search(const T& target, bst_node::TreeNode<T>*& found_ptr) const;

    // MUTATORS
    void clear();
    bool erase(const T& target);
    bool insert(const T& insert_me);
    void pop_front();

    // FRIENDS
    friend std::ostream& operator<<(std::ostream& outs, const BST<T>& tree) {
        bst_node::tree_print(tree._root, outs);
        return outs;
    }
    friend BST<T>& operator+=(BST<T>& lhs, const BST<T>& rhs) {
        bst_node::tree_add(lhs._root, rhs._root);
        return lhs;
    }

private:
    bst_node::TreeNode<T>* _root;
};

/*******************************************************************************
 * DESCRIPTION:
 *  Overloaded constructor - arguments: pointer to array of ascending
 *  sorted items with size of array. Nodes added to root are full/complete
 *  binary tree.
 *
 * PRE-CONDITIONS:
 *  const T* sorted_list: pointer to array of sorted items
 *  int size            : array size
 *
 * POST-CONDITIONS:
 *  TreeNode<T>* _root: nullptr if size < 1, else non-empty root
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
BST<T>::BST(const T* sorted_list, int size) {
    _root = bst_node::tree_from_sorted_list(sorted_list, size);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Copy constructor: uniquely copies node from source to _root.
 *
 * PRE-CONDITIONS:
 *  const BST<T>& copy_me: BST source
 *
 * POST-CONDITIONS:
 *  TreeNode<T>* _root: nullptr if source is empty, else non-empty root
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
BST<T>::BST(const BST<T>& copy_me) {
    _root = bst_node::tree_copy(copy_me._root);  // copy copy_me to _root
}

/*******************************************************************************
 * DESCRIPTION:
 *  Assignment operator: uniquely copies node from source to _root.
 *
 * PRE-CONDITIONS:
 *  const BST<T>& rhs: BST source
 *
 * POST-CONDITIONS:
 *  TreeNode<T>* _root: nullptr if source is empty, else non-empty root
 *
 * RETURN:
 *  *this
 ******************************************************************************/
template <typename T>
BST<T>& BST<T>::operator=(const BST<T>& rhs) {
    if(this != &rhs) {
        if(_root) bst_node::tree_clear(_root);   // deallocate old data
        _root = bst_node::tree_copy(rhs._root);  // copy rhs to _root
    }

    return *this;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Deallocates all nodes linked to _root.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  TreeNode<T>* _root: nullptr
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
BST<T>::~BST() {
    bst_node::tree_clear(_root);  // deallocate old data
}

/*******************************************************************************
 * DESCRIPTION:
 *  Checks if AVL is empty.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool BST<T>::empty() const {
    return !_root;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Access _root's item.
 *
 * PRE-CONDITIONS:
 *  _root != nullptr
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  T: root's item by reference
 ******************************************************************************/
template <typename T>
T& BST<T>::front() const {
    assert(!empty());
    return _root->_item;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns access to copy BST's root.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  const bst_node::TreeNode<T>*: access to root
 ******************************************************************************/
template <typename T>
const bst_node::TreeNode<T>* BST<T>::root() const {
    return _root;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Searches for target item and return non-nullptr by reference if found.
 *
 * PRE-CONDITIONS:
 *  const T& target        : target item
 *  TreeNode<T>*& found_ptr: pointer to search result
 *
 * POST-CONDITIONS:
 *  TreeNode<T>*& found_ptr: nullptr if not found, else pointer to item
 *
 * RETURN:
 *  bool: search success/failure
 ******************************************************************************/
template <typename T>
bool BST<T>::search(const T& target, bst_node::TreeNode<T>*& found_ptr) const {
    return bst_node::tree_search(_root, target, found_ptr);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Deallocates all nodes linked to _root.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  TreeNode<T>* _root: nullptr
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void BST<T>::clear() {
    bst_node::tree_clear(_root);  // deallocate old data
}

/*******************************************************************************
 * DESCRIPTION:
 *  Erases node that matches target item.
 *
 * PRE-CONDITIONS:
 *  const T& target: target item
 *
 * POST-CONDITIONS:
 *  TreeNode<T>* _root: matching node removed
 *
 * RETURN:
 *  bool: erasure success/failure
 ******************************************************************************/
template <typename T>
bool BST<T>::erase(const T& target) {
    return bst_node::tree_erase(_root, target);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Inserts node with target item.
 *
 * PRE-CONDITIONS:
 *  const T& insert_me: target item
 *
 * POST-CONDITIONS:
 *  TreeNode<T>* _root: new node inserted
 *
 * RETURN:
 *  bool: insertion success/failure
 ******************************************************************************/
template <typename T>
bool BST<T>::insert(const T& insert_me) {
    return bst_node::tree_insert(_root, insert_me);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Remove root's item and rotate/rebalance tree.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  TreeNode<T>* _root: root points to next suitable child.
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void BST<T>::pop_front() {
    tree_pop_root(_root);
}

}  // namespace bst

#endif  // BST_H
