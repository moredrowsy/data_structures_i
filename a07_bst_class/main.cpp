#include <iostream>
#include "../include/bst.h"

int main() {
    using namespace bst;

    // create empty btree1 and insert with values
    BST<int> btree1;

    std::cout << "Default constructor: printing empty btree1:" << std::endl;
    std::cout << btree1 << std::endl;

    btree1.insert(50);
    btree1.insert(70);
    btree1.insert(55);
    btree1.insert(60);
    btree1.insert(40);
    btree1.insert(30);
    btree1.insert(45);

    std::cout << std::endl;
    std::cout << "Printing btree1 with inserted values:" << std::endl;
    std::cout << btree1 << std::endl;

    // copy constructor btree2 with btree1
    BST<int> btree2(btree1);

    std::cout << std::endl;
    std::cout << "Copy constructor: printing btree2(btree1):" << std::endl;
    std::cout << btree2 << std::endl;

    btree2.insert(100);
    btree2.insert(-1);

    std::cout << std::endl;
    std::cout << "Printing btree1 after modifying btree2:" << std::endl;
    std::cout << btree1 << std::endl;

    std::cout << std::endl;
    std::cout << "Printing modified btree2:" << std::endl;
    std::cout << btree2 << std::endl;

    // assignment operator
    btree2 = btree1;

    std::cout << std::endl;
    std::cout << "Assignment operator btree2 = btree1:" << std::endl;
    std::cout << btree2 << std::endl;

    // search begins here
    bool is_search = false;
    int target = 0;
    bst_node::TreeNode<int> *found = nullptr;

    // search 50
    target = 50;
    is_search = btree1.search(target, found);
    std::cout << std::endl;

    if(is_search) {
        std::cout << "Search successful for target " << target
                  << " and found = " << *found << std::endl;
    } else {
        std::cout << "Search for target " << target << " failed" << std::endl;
    }

    // search 70
    target = 70;
    is_search = btree1.search(target, found);
    std::cout << std::endl;

    if(is_search) {
        std::cout << "Search successful for target " << target
                  << " and found = " << *found << std::endl;
    } else {
        std::cout << "Search for target " << target << " failed" << std::endl;
    }

    // search 30
    target = 30;
    is_search = btree1.search(target, found);
    std::cout << std::endl;

    if(is_search) {
        std::cout << "Search successful for target " << target
                  << " and found = " << *found << std::endl;
    } else {
        std::cout << "Search for target " << target << " failed" << std::endl;
    }

    // search invalid 0
    target = 0;
    is_search = btree1.search(target, found);
    std::cout << std::endl;

    if(is_search) {
        std::cout << "Search successful for target " << target
                  << " and found = " << *found << std::endl;
    } else {
        std::cout << "Search for target " << target << " failed" << std::endl;
    }

    // erase here
    bool is_erased = false;

    // erase 50
    target = 50;
    is_erased = btree1.erase(target);

    std::cout << std::endl;

    if(is_erased)
        std::cout << "Erasing successful for target " << target << std::endl;
    else
        std::cout << "Erasing target " << target << " failed" << std::endl;

    std::cout << "Printing btree1 after erasing " << target << ":" << std::endl;
    std::cout << btree1 << std::endl;

    // erase 55
    target = 55;
    is_erased = btree1.erase(target);

    std::cout << std::endl;

    if(is_erased)
        std::cout << "Erasing successful for target " << target << std::endl;
    else
        std::cout << "Erasing target " << target << " failed" << std::endl;

    std::cout << "Printing btree1 after erasing " << target << ":" << std::endl;
    std::cout << btree1 << std::endl;

    // erase 70
    target = 70;
    is_erased = btree1.erase(target);

    std::cout << std::endl;

    if(is_erased)
        std::cout << "Erasing successful for target " << target << std::endl;
    else
        std::cout << "Erasing target " << target << " failed" << std::endl;

    std::cout << "Printing btree1 after erasing " << target << ":" << std::endl;
    std::cout << btree1 << std::endl;

    // erase 30
    target = 30;
    is_erased = btree1.erase(target);

    std::cout << std::endl;

    if(is_erased)
        std::cout << "Erasing successful for target " << target << std::endl;
    else
        std::cout << "Erasing target " << target << " failed" << std::endl;

    std::cout << "Printing btree1 after erasing " << target << ":" << std::endl;
    std::cout << btree1 << std::endl;

    return 0;
}
