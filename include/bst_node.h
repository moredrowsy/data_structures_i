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
struct Tree_node {
    T _item;
    Tree_node<T>* _left;
    Tree_node<T>* _right;
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

    Tree_node(T item = T(), Tree_node* left = NULL, Tree_node* right = NULL)
        : _item(item), _left(left), _right(right), _height(1) {}

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

template <typename T>  // print tree rotated 90 degrees counterclockwise
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
void tree_print_node(Tree_node<T>* root);

template <typename T, typename F>
void inorder(Tree_node<T>* root, F f);

template <typename T, typename F>
void preorder(Tree_node<T>* root, F f);

template <typename T, typename F>
void postorder(Tree_node<T>* root, F f);

template <typename T>
void tree_insert(Tree_node<T>*& root, const T& insert_me) {
    if(!root)  // base: root is nullptr
        root = new Tree_node<T>(insert_me);
    else if(insert_me < root->_item)
        tree_insert(root->_left, insert_me);  // recurve left node
    else
        tree_insert(root->_right, insert_me);  // recurve right node

    root->update_height();
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
        outs << std::string(5 * level, ' ') << "|||" << std::endl;
        return;
    }

    tree_print(root->_right, level + 1, outs);  // recurve right
    outs << std::string(5 * level, ' ') << *root << std::endl;
    tree_print(root->_left, level + 1, outs);  // recurve left
}

template <typename T>  // prints details info about each node
void tree_print_debug(Tree_node<T>* root, int level, std::ostream& outs) {
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
        if(!root->_left) {
            Tree_node<T>* pop = root;
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

    if(target < root->_item)
        return tree_erase(root->_left, target);  // recurve left node
    else
        return tree_erase(root->_right, target);  // recurve right node

    root->update_height();
}

template <typename T>
void tree_remove_max(Tree_node<T>*& root, T& max_value) {
    if(!root) return;  // base: root is nullptr

    if(!root->_right) {
        max_value = root->_item;

        Tree_node<T>* pop = root;
        root = root->_left;

        delete pop;
        pop = nullptr;

        return;
    }

    tree_remove_max(root->_right, max_value);  // recurve right
    root->update_height();
}

template <typename T>
Tree_node<T>* tree_copy(Tree_node<T>* root) {
    if(!root) return nullptr;  // base: root is nullptr

    Tree_node<T>* new_node = new Tree_node<T>(
        root->_item, tree_copy(root->_left), tree_copy(root->_right));
    new_node->update_height();

    return new_node;
}

template <typename T>
void tree_add(Tree_node<T>*& dest, const Tree_node<T>* src) {
    if(src) {
        tree_add(dest, src->_left);
        tree_insert(dest, src->_item);
        dest->update_height();
        tree_add(dest, src->_right);
    }
}

template <typename T>
Tree_node<T>* tree_from_sorted_list(const T* a, int size) {
    if(!size) return nullptr;  // base: size is 0

    int midpoint = size / 2;

    if(size & 1) {  // when size is odd
        Tree_node<T>* new_node =
            new Tree_node<T>(a[midpoint], tree_from_sorted_list(a, midpoint),
                             tree_from_sorted_list(a + midpoint + 1, midpoint));
        new_node->update_height();

        return new_node;
    } else {  // when size is even
        Tree_node<T>* new_node = new Tree_node<T>(
            a[midpoint], tree_from_sorted_list(a, midpoint),
            tree_from_sorted_list(a + midpoint + 1, midpoint - 1));
        new_node->update_height();

        return new_node;
    }
}

template <typename T>
void tree_print_node(Tree_node<T>* root) {
    std::cout << *root << " ";
}

template <typename T, typename F>
void inorder(Tree_node<T>* root, F f) {
    if(root) {
        inorder(root->_left, f);
        f(root);
        inorder(root->_right, f);
    }
}

template <typename T, typename F>
void preorder(Tree_node<T>* root, F f) {
    if(root) {
        f(root);
        preorder(root->_left, f);
        preorder(root->_right, f);
    }
}

template <typename T, typename F>
void postorder(Tree_node<T>* root, F f) {
    if(root) {
        postorder(root->_left, f);
        postorder(root->_right, f);
        f(root);
    }
}

}  // namespace bst_node

#endif  // BST_NODE_H
