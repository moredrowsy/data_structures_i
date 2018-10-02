#include <cstdlib>  // rand()
#include <ctime>    // time()
#include <iostream>
#include "../include/bst.h"

void call_choice(bst::BST<int> &btree, char &c);
void menu(char &c);
void random_bst_test();

int main() {
    random_bst_test();

    return 0;
}

void menu(char &c) {
    std::cout << "[R]andom  [I]nsert  [C]clear  [S]earch  [E]rase      "
                 "e[X]it: ";
    std::cin >> c;
}

void random_bst_test() {
    using namespace bst;

    BST<int> btree;
    char c;

    do {
        menu(c);                // display menu and get user choice
        call_choice(btree, c);  // execute choice

    } while(c != 'X' && c != 'x');
}
void call_choice(bst::BST<int> &btree, char &c) {
    bool is_erased = false, is_found = false;
    bst_node::TreeNode<int> *found = nullptr;
    int i = 0;

    switch(c) {
        case 'C':
        case 'c':
            std::cout << "-- Clearing tree" << std::endl;
            btree.clear();

            break;
        case 'E':
        case 'e':

            std::cout << "-- Erasing: ";
            std::cin >> i;

            is_erased = btree.erase(i);

            if(is_erased)
                std::cout << "Item erased: " << std::endl;
            else
                std::cout << "Item not in tree." << std::endl;
            break;
        case 'I':
        case 'i':
            std::cout << "-- Inserting: ";
            std::cin >> i;

            btree.insert(i);

            break;

        case 'R':
        case 'r':
            srand(time(nullptr));
            i = rand() % 100;

            std::cout << "-- Inserting random: " << i << std::endl;
            btree.insert(i);

            break;

        case 'S':
        case 's':
            std::cout << "-- Searching: ";
            std::cin >> i;

            is_found = btree.search(i, found);

            if(is_found)
                std::cout << "Item found: " << *found << std::endl;
            else
                std::cout << "Item not found." << std::endl;

            break;

        default:
            break;
    }

    if(c != 'X' && c != 'x') std::cout << btree << std::endl;
}
