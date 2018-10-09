/*******************************************************************************
 * AUTHOR        : Thuan Tang
 * ID            : 00991588
 * CLASS         : CS008
 * ASSIGNMENT 08 : AVL Class
 * DESCRIPTION   : This program is an interactive test harness for the AVL
 *      class, which will guarantee a full or complete binary tree.
 ******************************************************************************/
#include <algorithm>         // random_shuffle
#include <cstdlib>           // rand()
#include <ctime>             // time()
#include <iostream>          // io stream objects
#include <string>            // string objects
#include <vector>            // vector objects
#include "../include/avl.h"  // AVL class, balanced binary tree

std::vector<int> gen_rand_vec();
void random_avl_test();

int main() {
    random_avl_test();

    return 0;
}

std::vector<int> gen_rand_vec() {
    // set values to rvector
    std::vector<int> rvector;
    for(int i = -99; i < 100; ++i) rvector.push_back(i);

    // randomize elements in vector
    srand(time(nullptr));
    std::random_shuffle(rvector.begin(), rvector.end());

    return rvector;
}

void random_avl_test() {
    using namespace avl;

    AVL<int> tree;
    bool is_erased = false, is_found = false, is_inserted = false;
    bst_node::TreeNode<int>* found = nullptr;
    char c;
    int i = 0;
    std::vector<int> rvector = gen_rand_vec();

    do {
        std::cout << "[R]andom  [I]nsert  [C]lear  [S]earch  [E]rase      "
                     "e[X]it: ";
        std::cin >> c;

        switch(c) {
            case 'X':
            case 'x':
                return;  // exit function
            case 'C':
            case 'c':
                rvector = gen_rand_vec();

                std::cout << "-- Clearing tree" << std::endl;
                tree.clear();
                std::cout << tree << std::endl;

                break;
            case 'E':
            case 'e':
                std::cout << "-- Erasing: ";
                std::cin >> i;

                is_erased = tree.erase(i);

                if(is_erased)
                    std::cout << "Item erased" << std::endl;
                else
                    std::cout << "Item not in tree" << std::endl;

                std::cout << tree << std::endl;

                break;
            case 'I':
            case 'i':
                std::cout << "-- Inserting: ";
                std::cin >> i;

                tree.insert(i);
                std::cout << tree << std::endl;

                break;
            case 'R':
            case 'r':
                i = rvector.at(rvector.size() - 1);
                rvector.pop_back();

                std::cout << "-- Inserting random: " << i << std::endl;
                is_inserted = tree.insert(i);

                if(!is_inserted)
                    std::cout
                        << "Insertion failed. No more unique random numbers"
                        << std::endl;

                std::cout << tree << std::endl;

                break;
            case 'S':
            case 's':
                std::cout << "-- Searching: ";
                std::cin >> i;

                is_found = tree.search(i, found);

                if(is_found)
                    std::cout << "Item found: " << *found << std::endl;
                else
                    std::cout << "Item not found" << std::endl;

                std::cout << tree << std::endl;

                break;
            default:
                std::cout << "Invalid Choice" << std::endl << std::endl;
        }

        std::cout << std::string(80, '-') << std::endl;

    } while(c != 'X' && c != 'x');
}
