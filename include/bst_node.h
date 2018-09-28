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

    int balance_factor() {
        return (this->_left ? this->_left->_height : -1) -
               (this->_right ? this->_right->_height : -1);
    }

    int height() {
        if(!this->_left && !this->_right) return 1 + std::max(-1, -1);

        return 1 + std::max(this->_left ? this->_left->_height : -1,
                            this->_right ? this->_right->_height : -1);
    }

    int update_height() { return _height = height(); }

    TreeNode(T item = T(), TreeNode* left = NULL, TreeNode* right = NULL)
        : _item(item), _left(left), _right(right), _height(0) {
        update_height();  // updates height when node is created recursively
    }

    friend std::ostream& operator<<(std::ostream& outs,
                                    const TreeNode<T>& t_node) {
        return outs << "|" << t_node._item << "|";
    }
};

template <typename T>  // add a node in the binary tree
void tree_insert(TreeNode<T>*& root, const T& insert_me);

template <typename T>  // search tree and return node if found
TreeNode<T>* tree_search(TreeNode<T>* root, const T& target);

template <typename T>  // search tree and return node plus boolean if found
bool tree_search(TreeNode<T>* root, const T& target, TreeNode<T>*& found_ptr);

template <typename T>  // print tree rotated 90 degrees counterclockwise
void tree_print(TreeNode<T>* root, int level = 0,
                std::ostream& outs = std::cout);

template <typename T>  // prints details info about each node
void tree_print_debug(TreeNode<T>* root, int level = 0,
                      std::ostream& outs = std::cout);

template <typename T>  // clear the tree
void tree_clear(TreeNode<T>*& root);

template <typename T>  // erase target from the tree
bool tree_erase(TreeNode<T>*& root, const T& target);

template <typename T>  // erase rightmost node from the tree
                       // store the item in max_value
void tree_remove_max(TreeNode<T>*& root, T& max_value);

template <typename T>  // return copy of tree pointed to by root
TreeNode<T>* tree_copy(TreeNode<T>* root);

template <typename T>  // add tree src to dest
void tree_add(TreeNode<T>*& dest, const TreeNode<T>* src);

template <typename T>  // sorted array -> tree
TreeNode<T>* tree_from_sorted_list(const T* a, int size);

template <typename T>  // print a single node (for function pointers)
void tree_print_node(TreeNode<T>* root);

template <typename T, typename F>
void inorder(TreeNode<T>* root, F f);

template <typename T, typename F>
void preorder(TreeNode<T>* root, F f);

template <typename T, typename F>
void postorder(TreeNode<T>* root, F f);

template <typename T>
void tree_insert(TreeNode<T>*& root, const T& insert_me) {
    if(!root)  // base: root is nullptr
        root = new TreeNode<T>(insert_me);
    else if(insert_me < root->_item)
        tree_insert(root->_left, insert_me);  // recurve left node
    else
        tree_insert(root->_right, insert_me);  // recurve right node

    root->update_height();
}

template <typename T>
TreeNode<T>* tree_search(TreeNode<T>* root, const T& target) {
    if(!root) return nullptr;  // base: root is nullptr

    if(target == root->_item) return root;  // return current root if found

    if(target < root->_item)
        return tree_search(root->_left, target);  // recurve left node
    else
        return tree_search(root->_right, target);  // recurve right node
}

template <typename T>
bool tree_search(TreeNode<T>* root, const T& target, TreeNode<T>*& found_ptr) {
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
void tree_print(TreeNode<T>* root, int level, std::ostream& outs) {
    if(!root) {  // base: root is nullptr
        outs << std::string(5 * level, ' ') << "|||" << std::endl;
        return;
    }

    tree_print(root->_right, level + 1, outs);  // recurve right
    outs << std::string(5 * level, ' ') << *root << std::endl;
    tree_print(root->_left, level + 1, outs);  // recurve left
}

template <typename T>  // prints details info about each node
void tree_print_debug(TreeNode<T>* root, int level, std::ostream& outs) {
    if(!root) {  // base: root is nullptr
        outs << std::string(5 * level, ' ') << "|||" << std::endl;
        return;
    }

    tree_print_debug(root->_right, level + 1, outs);  // recurve right
    outs << std::string(5 * level, ' ') << *root << " h" << root->_height
         << std::endl;
    tree_print_debug(root->_left, level + 1, outs);  // recurve left
}

template <typename T>
void tree_clear(TreeNode<T>*& root) {
    if(root) {
        tree_clear(root->_left);   // recurve left node
        tree_clear(root->_right);  // recurve right node

        delete root;     // delete current root
        root = nullptr;  // reset ptr to nullptr
    }
}

template <typename T>
bool tree_erase(TreeNode<T>*& root, const T& target) {
    if(!root) return false;  // base: root is nullptr

    // erase found target
    if(target == root->_item) {
        if(!root->_left) {
            TreeNode<T>* pop = root;
            root = root->_right;

            delete pop;
            pop = nullptr;
        } else {
            T max;
            tree_remove_max(root->_left, max);
            root->_item = max;
        }

        return true;
    }

    // recurve if target not found
    bool is_erased = false;

    if(target < root->_item) {
        is_erased = tree_erase(root->_left, target);  // recurve left node
        root->update_height();

        return is_erased;
    } else {
        is_erased = tree_erase(root->_right, target);  // recurve right node
        root->update_height();

        return is_erased;
    }
}

template <typename T>
void tree_remove_max(TreeNode<T>*& root, T& max_value) {
    if(!root) return;  // base: root is nullptr

    if(!root->_right) {
        max_value = root->_item;

        TreeNode<T>* pop = root;
        root = root->_left;

        delete pop;
        pop = nullptr;

        return;
    }

    tree_remove_max(root->_right, max_value);  // recurve right
    root->update_height();
}

template <typename T>
TreeNode<T>* tree_copy(TreeNode<T>* root) {
    if(!root) return nullptr;  // base: root is nullptr

    return new TreeNode<T>(root->_item, tree_copy(root->_left),
                           tree_copy(root->_right));
}

template <typename T>
void tree_add(TreeNode<T>*& dest, const TreeNode<T>* src) {
    if(src) {
        tree_add(dest, src->_left);  // recurve left
        tree_insert(dest, src->_item);
        tree_add(dest, src->_right);  // recurve right
    }
}

template <typename T>
TreeNode<T>* tree_from_sorted_list(const T* a, int size) {
    if(!size) return nullptr;  // base: size is 0

    int midpoint = size / 2;

    if(size & 1)  // when size is odd
        return new TreeNode<T>(
            a[midpoint], tree_from_sorted_list(a, midpoint),
            tree_from_sorted_list(a + midpoint + 1, midpoint));
    else  // when size is even
        return new TreeNode<T>(
            a[midpoint], tree_from_sorted_list(a, midpoint),
            tree_from_sorted_list(a + midpoint + 1, midpoint - 1));
}

template <typename T>
void tree_print_node(TreeNode<T>* root) {
    std::cout << *root << " ";
}

template <typename T, typename F>
void inorder(TreeNode<T>* root, F f) {
    if(root) {
        inorder(root->_left, f);
        f(root);
        inorder(root->_right, f);
    }
}

template <typename T, typename F>
void preorder(TreeNode<T>* root, F f) {
    if(root) {
        f(root);
        preorder(root->_left, f);
        preorder(root->_right, f);
    }
}

template <typename T, typename F>
void postorder(TreeNode<T>* root, F f) {
    if(root) {
        postorder(root->_left, f);
        postorder(root->_right, f);
        f(root);
    }
}

}  // namespace bst_node

#endif  // BST_NODE_H
