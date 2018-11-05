#include <cassert>                   // asseert()
#include <cstdlib>                   // rand(), srand()
#include <iomanip>                   // setw()
#include <iostream>                  // stream objects
#include <string>                    // string objects
#include "../include/array_utils.h"  // array utilities
#include "../include/btree.h"        // BTree class
#include "../include/sort.h"         // shuffle()

void test_btree_auto(int sample_size, int tree_size, bool report);
bool test_btree_auto(int tree_size, bool report);
bool test_btree_big_three();
bool test_btree_insert();
bool test_btree_remove();
void test_btree_interactive();

int main() {
    srand(0);

    test_btree_interactive();
    test_btree_big_three();
    test_btree_insert();
    test_btree_remove();
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
              << "             E N D     T E S T : " << sample_size
              << " tests of " << tree_size
              << " items : " << (verified ? "VERIFIED" : "VERIFICATION FAILED")
              << std::endl
              << std::string(80, '*') << std::endl
              << std::string(80, '*') << std::endl;
}

bool test_btree_auto(int tree_size, bool report) {
    const int MAX = 10000;
    assert(tree_size < MAX);

    btree::BTree<int> bt;
    int original[MAX];
    int test[MAX];
    int deleted[MAX];
    int r;

    std::size_t original_size = tree_size;
    std::size_t test_size = 0;
    std::size_t deleted_size = 0;

    // populate original[]
    for(std::size_t i = 0; i < original_size; ++i) original[i] = i;

    // shuffle original[]
    sort::shuffle(original, original_size);

    // copy original[] to test[]
    array_utils::copy_array(original, original_size, test, test_size);

    // populate BTree with test[] and assert items are inserted
    for(std::size_t i = 0; i < test_size; ++i) bt.insert(test[i]);

    if(report)
        std::cout << std::string(80, '=') << std::endl
                  << "BTree Auto Test" << std::endl
                  << std::string(80, '=') << std::endl
                  << std::endl
                  << bt << std::endl
                  << std::endl;

    for(int i = 0; i < tree_size; i++) {
        // pick item to delete
        r = rand() % test_size;

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
            std::cout << "REMOVING: [" << test[r] << "]" << std::endl;

            std::cout << std::string(80, '=') << std::endl;
            std::cout << std::endl;
        }

        // remove item from BTree and assert is removed
        bt.remove(test[r]);

        // remove deleted item from test[] and add to deleted[]
        array_utils::delete_item(test, r, test_size, deleted[deleted_size++]);

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
        std::cout << "V A L I D    T R E E" << std::endl;
    }

    return true;
}

bool test_btree_big_three() {
    const int MAX = 1000;
    int set1[MAX], set2[MAX];
    btree::BTree<int> bt1, bt2;

    // populate set1 and set2
    for(int i = 0; i < MAX; ++i) {
        set1[i] = i + 1;
        set2[i] = (i + 1) * -1;
    }

    // insert set1 and set2 to bt1 and bt2 respectively
    for(int i = 0; i < MAX; ++i) {
        bt1.insert(set1[i]);
        bt2.insert(set2[i]);
    }

    // test copy CTOR
    btree::BTree<int> bt_test(bt1);

    // verify bt_test contains set1 and BTree structure is valid
    for(int i = 0; i < MAX; ++i) {
        if(!bt_test.contains(set1[i]) || !bt_test.verify()) {
            std::cout << "C O N T A I N S  [" << set1[i] << "]  F A I L E D"
                      << std::endl;
            return false;
        }

        // modify bt_test after verifying set1[i] exists
        if(i < MAX / 2) bt_test.remove(set1[i]);
    }

    // verify bt1 is not modified
    for(int i = 0; i < MAX; ++i)
        if(!bt1.contains(set1[i]) || !bt1.verify()) {
            std::cout << "C O N T A I N S  [" << set1[i] << "]  F A I L E D"
                      << std::endl;
            return false;
        }

    // test assignment op
    bt_test = bt2;

    // verify bt_test contains set2 but not set 1 and BTree structure is valid
    for(int i = 0; i < MAX; ++i) {
        if(bt_test.contains(set1[i]) || !bt_test.contains(set2[i]) ||
           !bt_test.verify()) {
            std::cout << "C O N T A I N S  [" << set2[i] << "]  F A I L E D"
                      << std::endl;
            return false;
        }

        // modify bt_test after verifying set2[i] exists
        if(i < MAX / 2) bt_test.remove(set2[i]);
    }

    // verify bt2 is not modified
    for(int i = 0; i < MAX; ++i)
        if(!bt2.contains(set2[i]) || !bt2.verify()) {
            std::cout << "C O N T A I N S  [" << set2[i] << "]  F A I L E D"
                      << std::endl;
            return false;
        }

    return true;
}

bool test_btree_insert() {
    const int MAX = 1000, sample_size = 100;
    btree::BTree<int> bt;
    int test[MAX];
    int find;

    std::size_t test_size = MAX;

    // populate test[]
    for(std::size_t i = 0; i < test_size; ++i) test[i] = i;

    for(int i = 0; i < sample_size; ++i) {
        // shuffle test[]
        sort::shuffle(test, test_size);

        // populate BTree with test[] and return false if fails
        for(std::size_t i = 0; i < test_size; ++i)
            if(!bt.insert(test[i]) || !bt.verify()) {
                std::cout << "I N S E R T I O N  [" << i << "]  F A I L E D"
                          << std::endl;
                return false;
            }

        for(std::size_t i = 0; i < MAX; ++i) {
            // remove last item from test[] to verify it is contained in BTree
            find = test[--test_size];

            if(!bt.contains(find)) {
                std::cout << "C O N T A I N S  [" << find << "]  F A I L E D"
                          << std::endl;
                return false;
            }
        }

        // reset BTree and test variables
        bt.clear();
        test_size = MAX;
    }

    return true;
}

bool test_btree_remove() {
    const int MAX = 1000, sample_size = 100;
    btree::BTree<int> bt;
    int original[MAX];
    int test[MAX];
    int r;

    std::size_t original_size = MAX;
    std::size_t test_size = 0;

    // populate test[]
    for(std::size_t i = 0; i < original_size; ++i) original[i] = i;

    for(int i = 0; i < sample_size; ++i) {
        // copy original[] to test[]
        array_utils::copy_array(original, original_size, test, test_size);

        // shuffle test[]
        sort::shuffle(test, test_size);

        // populate BTree with test[]
        for(std::size_t i = 0; i < test_size; ++i) bt.insert(test[i]);

        for(std::size_t i = 0; i < original_size; ++i) {
            // pick item to delete
            r = rand() % test_size;

            if(!bt.remove(test[r]) || !bt.verify() || bt.contains(test[r])) {
                std::cout << "R E M O V E  [" << test[r] << "]  F A I L E D"
                          << std::endl;
                return false;
            }

            // remove deleted item from test[]
            array_utils::delete_item(test, r, test_size);
        }

        // reset BTree and test variables
        bt.clear();
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
