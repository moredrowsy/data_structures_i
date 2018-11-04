#include <cassert>                   //asseert()
#include <iomanip>                   // setw()
#include <iostream>                  // stream objects
#include <string>                    // string objects
#include "../include/array_utils.h"  // array utilities
#include "../include/btree.h"        // BTree class
#include "../include/sort.h"         // shuffle()

void test_btree_auto(int sample_size, int tree_size, bool report);
bool test_btree_auto(int tree_size, bool report);
void test_btree_interactive();

int main() {
    // test_btree_interactive();
    test_btree_auto(100, 1000, false);

    return 0;
}

void test_btree_auto(int sample_size, int tree_size, bool report) {
    bool verified = true;

    for(int i = 0; i < sample_size; i++) {
        if(report)
            std::cout << std::string(80, '*') << std::endl
                      << " T E S T:    " << i << std::endl
                      << std::string(80, '*') << std::endl;

        if(!test_btree_auto(tree_size, report)) {
            std::cout << "T E S T :   [" << i << "]    F A I L E D ! ! !"
                      << std::endl;
            verified = false;
            return;
        }
    }

    std::cout << std::string(80, '*') << std::endl
              << std::string(80, '*') << std::endl
              << "             E N D     T E S T: " << sample_size
              << " tests of " << tree_size
              << " items: " << (verified ? "VERIFIED" : "VERIFICATION FAILED")
              << std::endl
              << std::string(80, '*') << std::endl
              << std::string(80, '*') << std::endl;
}

bool test_btree_auto(int tree_size, bool report) {
    using namespace btree;

    const int MAX = 10000;
    assert(tree_size < MAX);

    bool is_inserted = false, is_removed = false;
    BTree<int> bt;
    int test[MAX];
    int original[MAX];
    int deleted[MAX];
    int r_index;

    std::size_t test_size = tree_size;
    std::size_t original_size;
    std::size_t deleted_size = 0;

    // populate test[]
    for(int i = 0; i < tree_size; ++i) test[i] = i;

    // shuffle test[]
    sort::shuffle(test, tree_size);

    // copy test[] to original[]
    array_utils::copy_array(test, tree_size, original, original_size);

    // populate BTree with test[] and assert items are inserted
    for(std::size_t i = 0; i < test_size; ++i) {
        is_inserted = bt.insert(test[i]);
        assert(is_inserted);
    }

    if(report)
        std::cout << std::string(80, '=') << std::endl
                  << "BTree Auto Test" << std::endl
                  << std::string(80, '=') << std::endl
                  << std::endl
                  << bt << std::endl
                  << std::endl;

    for(int i = 0; i < tree_size; i++) {
        // pick item to delete
        r_index = rand() % test_size;

        if(report) {
            std::cout << std::string(80, '=') << std::endl << std::endl;
            std::cout << bt << std::endl;

            for(int i = 0; i < 40; ++i) std::cout << ". ";
            std::cout << std::endl;

            std::cout << "    from: ";
            array_utils::print_array(original, original_size);
            std::cout << "    test: ";
            array_utils::print_array(test, test_size);
            std::cout << " deleted: ";
            array_utils::print_array(deleted, deleted_size);
            std::cout << "REMOVING: [" << test[r_index] << "]" << std::endl;

            std::cout << std::string(80, '=') << std::endl;
            std::cout << std::endl;
        }

        // remove item from BTree and assert is removed
        is_removed = bt.remove(test[r_index]);
        assert(is_removed);

        // remove deleted item from test[] and add to deleted[]
        array_utils::delete_item(test, r_index, test_size,
                                 deleted[deleted_size++]);

        if(!bt.verify()) {
            std::cout << std::setw(6) << i << " I N V A L I D   T R E E"
                      << std::endl;
            std::cout << "Original Array: ";
            array_utils::print_array(original, original_size);
            std::cout << "Deleted Items : ";
            array_utils::print_array(deleted, deleted_size);
            std::cout << std::endl << std::endl << bt << std::endl << std::endl;

            return false;
        }
    }

    if(report) {  // output final BTree structure
        std::cout << std::string(80, '=') << std::endl << std::endl;
        std::cout << bt << std::endl;

        for(int i = 0; i < 40; ++i) std::cout << ". ";
        std::cout << std::endl;

        std::cout << "    from: ";
        array_utils::print_array(original, original_size);
        std::cout << "    test: ";
        array_utils::print_array(test, test_size);
        std::cout << " deleted: ";
        array_utils::print_array(deleted, deleted_size);

        std::cout << std::string(80, '=') << std::endl;
        std::cout << " V A L I D    T R E E" << std::endl;
    }

    return true;
}

void test_btree_interactive() {
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

                std::cout << ">> Find: " << key;

                if(is_found)
                    std::cout << " success.";
                else
                    std::cout << " failed. Does not exist";

                std::cout << std::endl << bt << std::endl;

                break;
            case 'I':
            case 'i':
                std::cin >> key;
                is_inserted = bt.insert(key);

                std::cout << ">> Insert: " << key;

                if(is_inserted)
                    std::cout << " success.";
                else
                    std::cout << " failed. Duplicate?";

                std::cout << std::endl << bt << std::endl;

                break;
            case 'R':
            case 'r':
                key = rand() % MAX_RANGE;
                is_inserted = bt.insert(key);

                std::cout << ">> Random insert: " << key;

                if(is_inserted)
                    std::cout << " success.";
                else
                    std::cout << " failed. Duplicate?";

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
