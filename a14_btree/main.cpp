#include <iostream>
#include "../include/btree.h"

void test_btree__interactive();

int main() {
    test_btree__interactive();

    return 0;
}

void test_btree__interactive() {
    using namespace btree;

    std::cout << std::string(80, '-') << std::endl
              << "INTERACTIVE TESTS: BTree" << std::endl
              << std::string(80, '-') << std::endl;

    const unsigned MAX_RANGE = 100;
    BTree<int> bt;
    bool is_inserted = false, is_found = false, is_removed = false;
    int key;
    char c;

    srand(time(nullptr));

    do {
        std::cout << "[S]ize()  [R]andom  [I]nsert  [D]elete  [F]ind  [C]lear"
                     "     e[X]it: ";
        std::cin >> c;

        switch(c) {
            case 'X':
            case 'x':
                return;  // exit function
            case 'C':
            case 'c':
                bt.clear();
                std::cout << ">> Clear: " << std::endl << bt << std::endl;

                break;
            case 'D':
            case 'd':
                std::cin >> key;
                is_removed = bt.remove(key);

                std::cout << ">> Deletion: " << key;

                if(is_removed)
                    std::cout << " removed";
                else
                    std::cout << " does not exist";

                std::cout << std::endl << bt << std::endl;

                break;
            case 'F':
            case 'f':
                std::cin >> key;
                is_found = bt.find(key);

                std::cout << ">> Find ";

                if(is_found)
                    std::cout << "success: " << key;
                else
                    std::cout << "failed: " << key << " does not exist";

                std::cout << std::endl << bt << std::endl;

                break;
            case 'I':
            case 'i':
                std::cin >> key;
                is_inserted = bt.insert(key);

                std::cout << ">> Insert ";

                if(is_inserted)
                    std::cout << ": " << key;
                else
                    std::cout << "failed: duplicate?";

                std::cout << std::endl << bt << std::endl;

                break;
            case 'R':
            case 'r':
                key = rand() % MAX_RANGE;
                is_inserted = bt.insert(key);

                std::cout << ">> Random insert ";

                if(is_inserted)
                    std::cout << ": " << key;
                else
                    std::cout << "failed: duplicate?";

                std::cout << std::endl << bt << std::endl;

                break;
            case 'S':
            case 's':
                std::cout << ">> Size: " << bt.size() << std::endl << std::endl;
                break;
            default:
                std::cout << "Invalid Choice" << std::endl << std::endl;
        }
    } while(c != 'X' && c != 'x');
}
