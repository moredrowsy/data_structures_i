#include <iostream>
#include "../include/bst_node.h"

int main() {
    bst_node::Tree_node<int> *root1 = nullptr, *root2 = nullptr;

    // make BST
    bst_node::tree_insert(root1, 50);
    bst_node::tree_insert(root1, 60);
    bst_node::tree_insert(root1, 80);
    bst_node::tree_insert(root1, 70);
    bst_node::tree_insert(root1, 75);
    bst_node::tree_insert(root1, 40);
    bst_node::tree_insert(root1, 30);
    bst_node::tree_insert(root1, 45);

    // print root1
    std::cout << "root 1" << std::endl << std::string(80, '-') << std::endl;
    bst_node::tree_print(root1);
    std::cout << std::endl;

    // copy root2 with root1
    root2 = bst_node::tree_copy(root1);

    // print root2
    std::cout << "root 2" << std::endl << std::string(80, '-') << std::endl;
    bst_node::tree_print(root2);
    std::cout << std::endl;

    // deallocate all roots
    bst_node::tree_clear(root1);
    bst_node::tree_clear(root2);

    bst_node::Tree_node<int> *root_a = nullptr, *root_b = nullptr,
                             *root_c = nullptr, *root_d = nullptr,
                             *root_e = nullptr, *root_f = nullptr;

    // create bst from array 'a' with odd size
    const int size_a = 1;
    int a[size_a] = {0};
    root_a = bst_node::tree_from_sorted_list(a, size_a);

    // print root_a
    std::cout << "root_a" << std::endl << std::string(80, '-') << std::endl;
    bst_node::tree_print(root_a);
    std::cout << std::endl;

    // create bst from array 'b' with even size
    const int size_b = 2;
    int b[size_b] = {0, 1};
    root_b = bst_node::tree_from_sorted_list(b, size_b);

    // print root_b
    std::cout << "root_b" << std::endl << std::string(80, '-') << std::endl;
    bst_node::tree_print(root_b);
    std::cout << std::endl;

    // create bst from array 'c' with odd size
    const int size_c = 3;
    int c[size_c] = {0, 1, 2};
    root_c = bst_node::tree_from_sorted_list(c, size_c);

    // print root_c
    std::cout << "root_c" << std::endl << std::string(80, '-') << std::endl;
    bst_node::tree_print(root_c);
    std::cout << std::endl;

    // create bst from array 'd' with even size
    const int size_d = 4;
    int d[size_d] = {0, 1, 2, 3};
    root_d = bst_node::tree_from_sorted_list(d, size_d);

    // print root_d
    std::cout << "root_d" << std::endl << std::string(80, '-') << std::endl;
    bst_node::tree_print(root_d);
    std::cout << std::endl;

    // create bst from array 'e' with odd size
    const int size_e = 13;
    int e[size_e] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    root_e = bst_node::tree_from_sorted_list(e, size_e);

    // print root_e
    std::cout << "root_e" << std::endl << std::string(80, '-') << std::endl;
    bst_node::tree_print(root_e);
    std::cout << std::endl;

    // create bst from array 'f' with even size
    const int size_f = 12;
    int f[size_f] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    root_f = bst_node::tree_from_sorted_list(f, size_f);

    // print root_f
    std::cout << "root_f" << std::endl << std::string(80, '-') << std::endl;
    bst_node::tree_print(root_f);
    std::cout << std::endl;

    // deallocate all roots
    bst_node::tree_clear(root_a);
    bst_node::tree_clear(root_b);
    bst_node::tree_clear(root_c);
    bst_node::tree_clear(root_d);
    bst_node::tree_clear(root_e);
    bst_node::tree_clear(root_f);

    return 0;
}
