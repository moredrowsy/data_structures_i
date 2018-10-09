/*******************************************************************************
 * AUTHOR        : Thuan Tang
 * ID            : 00991588
 * CLASS         : CS008
 * ASSIGNMENT 06 : BST Node and Functions
 * DESCRIPTION   : This program is a test harness for binary TreeNode.
 ******************************************************************************/
#include <iostream>          // io stream objects
#include <string>            // string objects
#include "../include/bst.h"  // binary TreeNode struct and functions

int main() {
    using namespace bst_node;

    TreeNode<int> *root1 = nullptr, *root2 = nullptr;

    // make BST
    tree_insert(root1, 50);
    tree_insert(root1, 60);
    tree_insert(root1, 80);
    tree_insert(root1, 70);
    tree_insert(root1, 75);
    tree_insert(root1, 40);
    tree_insert(root1, 30);
    tree_insert(root1, 45);

    // print root1
    std::cout << "root 1" << std::endl << std::string(80, '-') << std::endl;
    tree_print_debug(root1);
    std::cout << std::endl;

    // delete max value of root1
    int max = 0;
    tree_remove_max(root1, max);

    // print root1
    std::cout << "root 1 with deleted max = " << max << std::endl
              << std::string(80, '-') << std::endl;
    tree_print_debug(root1);
    std::cout << std::endl;

    // delete max value of root1
    max = 0;
    tree_remove_max(root1, max);

    // print root1
    std::cout << "root 1 with deleted max = " << max << std::endl
              << std::string(80, '-') << std::endl;
    tree_print_debug(root1);
    std::cout << std::endl;

    tree_erase(root1, 60);

    // print root1
    std::cout << "root 1 with 60 removed" << std::endl
              << std::string(80, '-') << std::endl;
    tree_print_debug(root1);
    std::cout << std::endl;

    // copy root2 with root1
    root2 = tree_copy(root1);

    // print root2
    std::cout << "root 2 copy from root 1" << std::endl
              << std::string(80, '-') << std::endl;
    tree_print_debug(root2);
    std::cout << std::endl;

    tree_erase(root2, 40);

    // print root2
    std::cout << "root 2 with 40 removed" << std::endl
              << std::string(80, '-') << std::endl;
    tree_print_debug(root2);
    std::cout << std::endl;

    // deallocate all roots
    tree_clear(root1);
    tree_clear(root2);

    TreeNode<int> *root_a = nullptr, *root_b = nullptr, *root_c = nullptr,
                  *root_d = nullptr, *root_e = nullptr, *root_f = nullptr;

    // create bst from array 'a' with odd size
    const int size_a = 1;
    int a[size_a] = {1};
    std::cout << "a[" << size_a << "] = ";
    for(int i = 0; i < size_a; ++i) std::cout << a[i] << " ";
    std::cout << std::endl;
    root_a = tree_from_sorted_list(a, size_a);

    // print root_a
    std::cout << std::string(80, '-') << std::endl;
    tree_print_debug(root_a);
    std::cout << std::endl;

    // delete max value of root_a
    max = 0;
    tree_remove_max(root_a, max);

    // print root_a
    std::cout << "root_a with deleted max = " << max << std::endl
              << std::string(80, '-') << std::endl;
    tree_print_debug(root_a);
    std::cout << std::endl;

    // create bst from array 'b' with even size
    const int size_b = 2;
    int b[size_b] = {2, 4};
    std::cout << "b[" << size_b << "] = ";
    for(int i = 0; i < size_b; ++i) std::cout << b[i] << " ";
    std::cout << std::endl;
    root_b = tree_from_sorted_list(b, size_b);

    // print root_b
    std::cout << std::string(80, '-') << std::endl;
    tree_print_debug(root_b);
    std::cout << std::endl;

    // delete max value of root_b
    max = 0;
    tree_remove_max(root_b, max);

    // print root_b
    std::cout << "root_b with deleted max = " << max << std::endl
              << std::string(80, '-') << std::endl;
    tree_print_debug(root_b);
    std::cout << std::endl;

    // create bst from array 'c' with odd size
    const int size_c = 3;
    int c[size_c] = {1, 3, 5};
    std::cout << "c[" << size_c << "] = ";
    for(int i = 0; i < size_c; ++i) std::cout << c[i] << " ";
    std::cout << std::endl;
    root_c = tree_from_sorted_list(c, size_c);

    // print root_c
    std::cout << std::string(80, '-') << std::endl;
    tree_print_debug(root_c);
    std::cout << std::endl;

    // delete max value of root_c
    max = 0;
    tree_remove_max(root_c, max);

    // print root_c
    std::cout << "root_c with deleted max = " << max << std::endl
              << std::string(80, '-') << std::endl;
    tree_print_debug(root_c);
    std::cout << std::endl;

    // create bst from array 'd' with even size
    const int size_d = 4;
    int d[size_d] = {2, 4, 6, 8};
    std::cout << "d[" << size_d << "] = ";
    for(int i = 0; i < size_d; ++i) std::cout << d[i] << " ";
    std::cout << std::endl;
    root_d = tree_from_sorted_list(d, size_d);

    // print root_d
    std::cout << std::string(80, '-') << std::endl;
    tree_print_debug(root_d);
    std::cout << std::endl;

    // create bst from array 'e' with odd size
    const int size_e = 13;
    int e[size_e] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25};
    std::cout << "e[" << size_e << "] = ";
    for(int i = 0; i < size_e; ++i) std::cout << e[i] << " ";
    std::cout << std::endl;
    root_e = tree_from_sorted_list(e, size_e);

    // print root_e
    std::cout << std::string(80, '-') << std::endl;
    tree_print_debug(root_e);
    std::cout << std::endl;

    // delete max value of root_e
    max = 0;
    tree_remove_max(root_e, max);

    // print root_e
    std::cout << "root_e with deleted max = " << max << std::endl
              << std::string(80, '-') << std::endl;
    tree_print_debug(root_e);
    std::cout << std::endl;

    // create bst from array 'f' with even size
    const int size_f = 12;
    int f[size_f] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24};
    std::cout << "f[" << size_f << "] = ";
    for(int i = 0; i < size_f; ++i) std::cout << f[i] << " ";
    std::cout << std::endl;
    root_f = tree_from_sorted_list(f, size_f);

    // print root_f
    std::cout << std::string(80, '-') << std::endl;
    tree_print_debug(root_f);
    std::cout << std::endl;

    // delete max value of root_f
    max = 0;
    tree_remove_max(root_f, max);

    // print root_f
    std::cout << "root_f with deleted max = " << max << std::endl
              << std::string(80, '-') << std::endl;
    tree_print_debug(root_f);
    std::cout << std::endl;

    // add tree
    tree_add(root_e, root_f);

    std::cout << "root_e = root_e + root_f" << max << std::endl
              << std::string(80, '-') << std::endl;
    tree_print_debug(root_e);
    std::cout << std::endl;

    // tree_search
    TreeNode<int> *find = nullptr;
    int find_t = 0;

    // find in e, size odd
    find_t = 1;
    find = tree_search(root_e, find_t);
    if(find)
        std::cout << "Found " << *find << " in root_e" << std::endl;
    else
        std::cout << "Couldn't find " << find_t << " in root_e" << std::endl;

    // find in e, size odd
    find_t = 13;
    find = tree_search(root_e, find_t);
    if(find)
        std::cout << "Found " << *find << " in root_e" << std::endl;
    else
        std::cout << "Couldn't find " << find_t << " in root_e" << std::endl;

    // find in e, size odd
    find_t = 23;
    find = tree_search(root_e, find_t);
    if(find)
        std::cout << "Found " << *find << " in root_e" << std::endl;
    else
        std::cout << "Couldn't find " << find_t << " in root_e" << std::endl;

    // find in e, size odd
    find_t = 27;
    find = tree_search(root_e, find_t);
    if(find)
        std::cout << "Found " << *find << " in root_e" << std::endl;
    else
        std::cout << "Couldn't find " << find_t << " in root_e" << std::endl;

    // find in e, size odd
    find_t = 2;
    find = tree_search(root_e, find_t);
    if(find)
        std::cout << "Found " << *find << " in root_e" << std::endl;
    else
        std::cout << "Couldn't find " << find_t << " in root_e" << std::endl;

    // find in f, size even
    find_t = 2;
    find = tree_search(root_f, find_t);
    if(find)
        std::cout << "Found " << *find << " in root_f" << std::endl;
    else
        std::cout << "Couldn't find " << find_t << " in root_f" << std::endl;

    // find in f, size even
    find_t = 12;
    find = tree_search(root_f, find_t);
    if(find)
        std::cout << "Found " << *find << " in root_f" << std::endl;
    else
        std::cout << "Couldn't find " << find_t << " in root_f" << std::endl;

    // find in f, size even
    find_t = 22;
    find = tree_search(root_f, find_t);
    if(find)
        std::cout << "Found " << *find << " in root_f" << std::endl;
    else
        std::cout << "Couldn't find " << find_t << " in root_f" << std::endl;

    // find in f, size even
    find_t = 26;
    find = tree_search(root_f, find_t);
    if(find)
        std::cout << "Found " << *find << " in root_f" << std::endl;
    else
        std::cout << "Couldn't find " << find_t << " in root_f" << std::endl;

    // find in f, size even
    find_t = 3;
    find = tree_search(root_f, find_t);
    if(find)
        std::cout << "Found " << *find << " in root_f" << std::endl;
    else
        std::cout << "Couldn't find " << find_t << " in root_f" << std::endl;

    // call inorder with direct lambda call
    std::cout << std::endl
              << "Calling inorder with direct lambda call" << std::endl;
    inorder(root_f, [](auto &r) { std::cout << *r << " "; });
    std::cout << std::endl;

    // call preorder with indrect lambda call to tree_print_node
    std::cout << std::endl
              << "Calling preorder with direct lambda call" << std::endl;
    preorder(root_f, [](auto &r) { std::cout << *r << " "; });
    std::cout << std::endl;

    // call postorder with indrect lambda call to tree_print_node
    std::cout << std::endl
              << "Calling postorder with direct lambda call" << std::endl;
    postorder(root_f, [](auto &r) { std::cout << *r << " "; });
    std::cout << std::endl << std::endl;
    ;

    TreeNode<int> *left_left = nullptr, *right_right = nullptr,
                  *left_right = nullptr, *right_left = nullptr;

    // left_left  inserts
    tree_insert(left_left, 60);
    tree_insert(left_left, 70);
    tree_insert(left_left, 40);
    tree_insert(left_left, 50);
    tree_insert(left_left, 20);
    tree_insert(left_left, 30);
    tree_insert(left_left, 10);

    // print left_left
    std::cout << "Unbalanced left_left" << std::endl
              << std::string(80, '-') << std::endl;
    tree_print_debug(left_left);
    std::cout << std::endl;

    left_left = rotate_right(left_left);

    // print left_left
    std::cout << "rotate_right() on left_left" << std::endl
              << std::string(80, '-') << std::endl;
    tree_print_debug(left_left);
    std::cout << std::endl;

    // right_right inserts
    tree_insert(right_right, 40);
    tree_insert(right_right, 30);
    tree_insert(right_right, 60);
    tree_insert(right_right, 50);
    tree_insert(right_right, 80);
    tree_insert(right_right, 70);
    tree_insert(right_right, 90);

    // print right right
    std::cout << "Unbalanced right_right" << std::endl
              << std::string(80, '-') << std::endl;
    tree_print_debug(right_right);
    std::cout << std::endl;

    right_right = rotate_left(right_right);

    // print right right
    std::cout << "rotate_left() on right right" << std::endl
              << std::string(80, '-') << std::endl;
    tree_print_debug(right_right);
    std::cout << std::endl;

    // left_right inserts
    tree_insert(left_right, 60);
    tree_insert(left_right, 70);
    tree_insert(left_right, 20);
    tree_insert(left_right, 10);
    tree_insert(left_right, 40);
    tree_insert(left_right, 30);
    tree_insert(left_right, 50);

    // print left right
    std::cout << "Unbalanced left_right" << std::endl
              << std::string(80, '-') << std::endl;
    tree_print_debug(left_right);
    std::cout << std::endl;

    rotate(left_right);

    // print left right
    std::cout << "rotate() on left right" << std::endl
              << std::string(80, '-') << std::endl;
    tree_print_debug(left_right);
    std::cout << std::endl;

    // right_left inserts
    tree_insert(right_left, 40);
    tree_insert(right_left, 30);
    tree_insert(right_left, 80);
    tree_insert(right_left, 60);
    tree_insert(right_left, 50);
    tree_insert(right_left, 70);
    tree_insert(right_left, 90);

    // print left right
    std::cout << "Unbalanced right_left" << std::endl
              << std::string(80, '-') << std::endl;
    tree_print_debug(right_left);
    std::cout << std::endl;

    rotate(right_left);

    // print left right
    std::cout << "rotate() on right left" << std::endl
              << std::string(80, '-') << std::endl;
    tree_print_debug(right_left);
    std::cout << std::endl;

    // deallocate all roots
    tree_clear(root_a);
    tree_clear(root_b);
    tree_clear(root_c);
    tree_clear(root_d);
    tree_clear(root_e);
    tree_clear(root_f);
    tree_clear(left_left);
    tree_clear(right_right);
    tree_clear(left_right);
    tree_clear(right_left);

    return 0;
}
