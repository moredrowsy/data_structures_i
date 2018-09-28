#include <iostream>
#include "../include/bst.h"

int main() {
    using namespace bst_node;

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
    bst_node::tree_print_debug(root1);
    std::cout << std::endl;

    // delete max value of root1
    int max = 0;
    bst_node::tree_remove_max(root1, max);

    // print root1
    std::cout << "root 1 with deleted max = " << max << std::endl
              << std::string(80, '-') << std::endl;
    bst_node::tree_print_debug(root1);
    std::cout << std::endl;

    // delete max value of root1
    max = 0;
    bst_node::tree_remove_max(root1, max);

    // print root1
    std::cout << "root 1 with deleted max = " << max << std::endl
              << std::string(80, '-') << std::endl;
    bst_node::tree_print_debug(root1);
    std::cout << std::endl;

    bst_node::tree_erase(root1, 60);

    // print root1
    std::cout << "root 1 with 60 removed" << std::endl
              << std::string(80, '-') << std::endl;
    bst_node::tree_print_debug(root1);
    std::cout << std::endl;

    // copy root2 with root1
    root2 = bst_node::tree_copy(root1);

    // print root2
    std::cout << "root 2 copy from root 1" << std::endl
              << std::string(80, '-') << std::endl;
    bst_node::tree_print_debug(root2);
    std::cout << std::endl;

    bst_node::tree_erase(root2, 40);

    // print root2
    std::cout << "root 2 with 40 removed" << std::endl
              << std::string(80, '-') << std::endl;
    bst_node::tree_print_debug(root2);
    std::cout << std::endl;

    // deallocate all roots
    bst_node::tree_clear(root1);
    bst_node::tree_clear(root2);

    bst_node::Tree_node<int> *root_a = nullptr, *root_b = nullptr,
                             *root_c = nullptr, *root_d = nullptr,
                             *root_e = nullptr, *root_f = nullptr;

    // create bst from array 'a' with odd size
    const int size_a = 1;
    int a[size_a] = {1};
    std::cout << "a[" << size_a << "] = ";
    for(int i = 0; i < size_a; ++i) std::cout << a[i] << " ";
    std::cout << std::endl;
    root_a = bst_node::tree_from_sorted_list(a, size_a);

    // print root_a
    std::cout << std::string(80, '-') << std::endl;
    bst_node::tree_print_debug(root_a);
    std::cout << std::endl;

    // delete max value of root_a
    max = 0;
    bst_node::tree_remove_max(root_a, max);

    // print root_a
    std::cout << "root_a with deleted max = " << max << std::endl
              << std::string(80, '-') << std::endl;
    bst_node::tree_print_debug(root_a);
    std::cout << std::endl;

    // create bst from array 'b' with even size
    const int size_b = 2;
    int b[size_b] = {2, 4};
    std::cout << "b[" << size_b << "] = ";
    for(int i = 0; i < size_b; ++i) std::cout << b[i] << " ";
    std::cout << std::endl;
    root_b = bst_node::tree_from_sorted_list(b, size_b);

    // print root_b
    std::cout << std::string(80, '-') << std::endl;
    bst_node::tree_print_debug(root_b);
    std::cout << std::endl;

    // delete max value of root_b
    max = 0;
    bst_node::tree_remove_max(root_b, max);

    // print root_b
    std::cout << "root_b with deleted max = " << max << std::endl
              << std::string(80, '-') << std::endl;
    bst_node::tree_print_debug(root_b);
    std::cout << std::endl;

    // create bst from array 'c' with odd size
    const int size_c = 3;
    int c[size_c] = {1, 3, 5};
    std::cout << "c[" << size_c << "] = ";
    for(int i = 0; i < size_c; ++i) std::cout << c[i] << " ";
    std::cout << std::endl;
    root_c = bst_node::tree_from_sorted_list(c, size_c);

    // print root_c
    std::cout << std::string(80, '-') << std::endl;
    bst_node::tree_print_debug(root_c);
    std::cout << std::endl;

    // delete max value of root_c
    max = 0;
    bst_node::tree_remove_max(root_c, max);

    // print root_c
    std::cout << "root_c with deleted max = " << max << std::endl
              << std::string(80, '-') << std::endl;
    bst_node::tree_print_debug(root_c);
    std::cout << std::endl;

    // create bst from array 'd' with even size
    const int size_d = 4;
    int d[size_d] = {2, 4, 6, 8};
    std::cout << "d[" << size_d << "] = ";
    for(int i = 0; i < size_d; ++i) std::cout << d[i] << " ";
    std::cout << std::endl;
    root_d = bst_node::tree_from_sorted_list(d, size_d);

    // print root_d
    std::cout << std::string(80, '-') << std::endl;
    bst_node::tree_print_debug(root_d);
    std::cout << std::endl;

    // create bst from array 'e' with odd size
    const int size_e = 13;
    int e[size_e] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25};
    std::cout << "e[" << size_e << "] = ";
    for(int i = 0; i < size_e; ++i) std::cout << e[i] << " ";
    std::cout << std::endl;
    root_e = bst_node::tree_from_sorted_list(e, size_e);

    // print root_e
    std::cout << std::string(80, '-') << std::endl;
    bst_node::tree_print_debug(root_e);
    std::cout << std::endl;

    // delete max value of root_e
    max = 0;
    bst_node::tree_remove_max(root_e, max);

    // print root_e
    std::cout << "root_e with deleted max = " << max << std::endl
              << std::string(80, '-') << std::endl;
    bst_node::tree_print_debug(root_e);
    std::cout << std::endl;

    // create bst from array 'f' with even size
    const int size_f = 12;
    int f[size_f] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24};
    std::cout << "f[" << size_f << "] = ";
    for(int i = 0; i < size_f; ++i) std::cout << f[i] << " ";
    std::cout << std::endl;
    root_f = bst_node::tree_from_sorted_list(f, size_f);

    // print root_f
    std::cout << std::string(80, '-') << std::endl;
    bst_node::tree_print_debug(root_f);
    std::cout << std::endl;

    // delete max value of root_f
    max = 0;
    bst_node::tree_remove_max(root_f, max);

    // print root_f
    std::cout << "root_f with deleted max = " << max << std::endl
              << std::string(80, '-') << std::endl;
    bst_node::tree_print_debug(root_f);
    std::cout << std::endl;

    // add tree
    bst_node::tree_add(root_e, root_f);

    std::cout << "root_e = root_e + root_f" << max << std::endl
              << std::string(80, '-') << std::endl;
    bst_node::tree_print_debug(root_e);
    std::cout << std::endl;

    // tree_search
    bst_node::Tree_node<int> *find = nullptr;
    int find_t = 0;

    // find in e, size odd
    find_t = 1;
    find = bst_node::tree_search(root_e, find_t);
    if(find)
        std::cout << "Found " << *find << " in root_e" << std::endl;
    else
        std::cout << "Couldn't find " << find_t << " in root_e" << std::endl;

    // find in e, size odd
    find_t = 13;
    find = bst_node::tree_search(root_e, find_t);
    if(find)
        std::cout << "Found " << *find << " in root_e" << std::endl;
    else
        std::cout << "Couldn't find " << find_t << " in root_e" << std::endl;

    // find in e, size odd
    find_t = 23;
    find = bst_node::tree_search(root_e, find_t);
    if(find)
        std::cout << "Found " << *find << " in root_e" << std::endl;
    else
        std::cout << "Couldn't find " << find_t << " in root_e" << std::endl;

    // find in e, size odd
    find_t = 27;
    find = bst_node::tree_search(root_e, find_t);
    if(find)
        std::cout << "Found " << *find << " in root_e" << std::endl;
    else
        std::cout << "Couldn't find " << find_t << " in root_e" << std::endl;

    // find in e, size odd
    find_t = 2;
    find = bst_node::tree_search(root_e, find_t);
    if(find)
        std::cout << "Found " << *find << " in root_e" << std::endl;
    else
        std::cout << "Couldn't find " << find_t << " in root_e" << std::endl;

    // find in f, size even
    find_t = 2;
    find = bst_node::tree_search(root_f, find_t);
    if(find)
        std::cout << "Found " << *find << " in root_f" << std::endl;
    else
        std::cout << "Couldn't find " << find_t << " in root_f" << std::endl;

    // find in f, size even
    find_t = 12;
    find = bst_node::tree_search(root_f, find_t);
    if(find)
        std::cout << "Found " << *find << " in root_f" << std::endl;
    else
        std::cout << "Couldn't find " << find_t << " in root_f" << std::endl;

    // find in f, size even
    find_t = 22;
    find = bst_node::tree_search(root_f, find_t);
    if(find)
        std::cout << "Found " << *find << " in root_f" << std::endl;
    else
        std::cout << "Couldn't find " << find_t << " in root_f" << std::endl;

    // find in f, size even
    find_t = 26;
    find = bst_node::tree_search(root_f, find_t);
    if(find)
        std::cout << "Found " << *find << " in root_f" << std::endl;
    else
        std::cout << "Couldn't find " << find_t << " in root_f" << std::endl;

    // find in f, size even
    find_t = 3;
    find = bst_node::tree_search(root_f, find_t);
    if(find)
        std::cout << "Found " << *find << " in root_f" << std::endl;
    else
        std::cout << "Couldn't find " << find_t << " in root_f" << std::endl;

    // call inorder with function pointer
    std::cout << std::endl
              << "Calling inorder with function pointer" << std::endl;
    bst_node::inorder(root_f, bst_node::tree_print_node<int>);
    std::cout << std::endl;

    // call preorder with function pointer
    std::cout << std::endl
              << "Calling preorder with function pointer" << std::endl;
    bst_node::preorder(root_f, bst_node::tree_print_node<int>);
    std::cout << std::endl;

    // call postorder with function pointer
    std::cout << std::endl
              << "Calling postorder with function pointer" << std::endl;
    bst_node::postorder(root_f, bst_node::tree_print_node<int>);
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
