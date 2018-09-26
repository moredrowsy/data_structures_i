#include <iostream>
#include "../include/bst_node.h"

int main() {
    bst_node::tree_node<int> *root = nullptr;

    bst_node::tree_insert(root, 50);
    bst_node::tree_insert(root, 60);
    bst_node::tree_insert(root, 80);
    bst_node::tree_insert(root, 70);
    bst_node::tree_insert(root, 75);
    bst_node::tree_insert(root, 40);
    bst_node::tree_insert(root, 30);
    bst_node::tree_insert(root, 45);

    bst_node::tree_print(root);
    std::cout << std::endl;

    bst_node::tree_clear(root);

    return 0;
}
