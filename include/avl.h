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
    const bst_node::TreeNode<T>* root() const;
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
AVL<T>::AVL(const T* sorted_list, int size) {
    _root = bst_node::tree_from_sorted_list(sorted_list, size);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Copy constructor: uniquely copies node from source to _root. Nodes copied
 *  will produce a full/complete binary tree.
 *
 * PRE-CONDITIONS:
 *  const AVL<T>& copy_me: AVL source
 *
 * POST-CONDITIONS:
 *  TreeNode<T>* _root: nullptr if source is empty, else non-empty root
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
AVL<T>::AVL(const AVL<T>& copy_me) {
    _root = bst_node::tree_copy(copy_me._root, true);  // copy copy_me to _root
}

/*******************************************************************************
 * DESCRIPTION:
 *  Assignment operator: uniquely copies node from source to _root.
 *
 * PRE-CONDITIONS:
 *  const AVL<T>& rhs: AVL source
 *
 * POST-CONDITIONS:
 *  TreeNode<T>* _root: nullptr if source is empty, else non-empty root
 *
 * RETURN:
 *  *this
 ******************************************************************************/
template <typename T>
AVL<T>& AVL<T>::operator=(const AVL<T>& rhs) {
    if(this != &rhs) {
        if(_root) bst_node::tree_clear(_root);         // deallocate old data
        _root = bst_node::tree_copy(rhs._root, true);  // copy rhs to _root
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
AVL<T>::~AVL() {
    bst_node::tree_clear(_root);  // deallocate old data
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns access to copy AVL's root.
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
const bst_node::TreeNode<T>* AVL<T>::root() const {
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
bool AVL<T>::search(const T& target, bst_node::TreeNode<T>*& found_ptr) const {
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
void AVL<T>::clear() {
    bst_node::tree_clear(_root);  // deallocate old data
}

/*******************************************************************************
 * DESCRIPTION:
 *  Erases node that matches target item. AVL will rebalance to full/complete
 *  binary tree.
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
bool AVL<T>::erase(const T& target) {
    return bst_node::tree_erase(_root, target, true);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Inserts node with target item. AVL will rebalance to full/complete binary
 *  tree.
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
bool AVL<T>::insert(const T& insert_me) {
    return bst_node::tree_insert(_root, insert_me, true);
}

}  // namespace avl

#endif  // AVL_H
