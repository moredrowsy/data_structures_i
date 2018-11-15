#include <iostream>             // stream objects
#include "../include/bptree.h"  // BPTree class

void test_bptree_auto(int sample_size, int tree_size, bool report);
bool test_bptree_auto(int tree_size, bool report);
bool test_bptree_big_three();
bool test_bptree_insert();
bool test_bptree_remove();
void test_bptree_interactive();

int main() {
    srand(0);

    test_bptree_interactive();
    test_bptree_big_three();
    test_bptree_insert();
    test_bptree_remove();
    test_bptree_auto(100, 1000, false);

    return 0;
}

void test_bptree_auto(int sample_size, int tree_size, bool report) {
    bool verified = true;

    for(int i = 0; i < sample_size; i++) {
        if(report)
            std::cout << std::string(80, '*') << std::endl
                      << " T E S T:    " << i << std::endl
                      << std::string(80, '*') << std::endl;

        if(!test_bptree_auto(tree_size, report)) {
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

bool test_bptree_auto(int tree_size, bool report) {
    const int MAX = 10000;
    assert(tree_size < MAX);

    bptree::BPTree<int> bpt;
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

    // populate bptree with test[] and assert items are inserted
    for(std::size_t i = 0; i < test_size; ++i) bpt.insert(test[i]);

    if(report)
        std::cout << std::string(80, '=') << std::endl
                  << "bptree Auto Test" << std::endl
                  << std::string(80, '=') << std::endl
                  << std::endl
                  << bpt << std::endl
                  << std::endl;

    for(int i = 0; i < tree_size; i++) {
        // pick item to delete
        r = rand() % test_size;

        if(report) {
            std::cout << std::string(80, '=') << std::endl << std::endl;
            std::cout << bpt << std::endl;

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

        // remove item from bptree and assert is removed
        bpt.remove(test[r]);

        // remove deleted item from test[] and add to deleted[]
        array_utils::delete_item(test, r, test_size, deleted[deleted_size++]);

        if(!bpt.verify()) {
            std::cout << std::setw(6) << i << " I N V A L I D   T R E E"
                      << std::endl;
            std::cout << "Original Array: ";
            array_utils::print_array(original, original_size);
            std::cout << "Deleted Items : ";
            array_utils::print_array(deleted, deleted_size);
            std::cout << std::endl
                      << std::endl
                      << bpt << std::endl
                      << std::endl;

            return false;
        }
    }

    if(report) {  // output final bptree structure
        std::cout << std::string(80, '=') << std::endl << std::endl;
        std::cout << bpt << std::endl;

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

bool test_bptree_big_three() {
    bool is_passed = true;
    const int MAX = 1000;
    int set1[MAX], set2[MAX];
    bptree::BPTree<int> bpt1, bpt2;

    // populate set1 and set2
    for(int i = 0; i < MAX; ++i) {
        set1[i] = i + 1;
        set2[i] = (i + 1) * -1;
    }

    // insert set1 and set2 to bpt1 and bpt2 respectively
    for(int i = 0; i < MAX; ++i) {
        bpt1.insert(set1[i]);
        bpt2.insert(set2[i]);
    }

    // test copy CTOR
    bptree::BPTree<int> bpt_test(bpt1);

    // verify bpt_test contains set1 and bptree structure is valid
    for(int i = 0; i < MAX; ++i) {
        if(!bpt_test.contains(set1[i]) || !bpt_test.verify()) {
            is_passed = false;
            break;
        }
    }

    if(bpt_test.size() != MAX) is_passed = false;

    // modify bpt_test
    for(int i = 0; i < MAX / 2; ++i) {
        if(!bpt_test.remove(set1[i])) {
            is_passed = false;
            break;
        }
    }

    // verify bpt1 is not modified
    for(int i = 0; i < MAX; ++i)
        if(!bpt1.contains(set1[i]) || !bpt1.verify()) {
            is_passed = false;
            break;
        }

    // test assignment op
    bpt_test = bpt2;

    // verify bpt_test contains set2 but not set 1 and bptree structure is valid
    for(int i = 0; i < MAX; ++i) {
        if(bpt_test.contains(set1[i]) || !bpt_test.contains(set2[i]) ||
           !bpt_test.verify()) {
            is_passed = false;
            break;
        }
    }

    if(bpt_test.size() != MAX) is_passed = false;

    // modify bpt_test
    for(int i = 0; i < MAX / 2; ++i) {
        if(!bpt_test.remove(set2[i])) {
            is_passed = false;
            break;
        }
    }

    // verify bpt2 is not modified
    for(int i = 0; i < MAX; ++i)
        if(!bpt2.contains(set2[i]) || !bpt2.verify()) {
            is_passed = false;
            break;
        }

    if(!is_passed) std::cout << "B I G  T H R E E  F A I L E D" << std::endl;

    return is_passed;
}

bool test_bptree_insert() {
    bool is_passed = true;
    const int MAX = 1000, sample_size = 100;
    bptree::BPTree<int> bpt;
    int test[MAX];
    int find;

    std::size_t test_size = MAX;

    // populate test[]
    for(std::size_t i = 0; i < test_size; ++i) test[i] = i;

    for(int i = 0; i < sample_size; ++i) {
        // shuffle test[]
        sort::shuffle(test, test_size);

        // populate bptree with test[] and return false if fails
        for(std::size_t i = 0; i < test_size; ++i)
            if(!bpt.insert(test[i]) || !bpt.verify()) {
                is_passed = false;
                break;
            }

        for(std::size_t i = 0; i < MAX; ++i) {
            // remove last item from test[] to verify it is contained in bptree
            find = test[--test_size];

            if(!bpt.contains(find)) {
                is_passed = false;
                break;
            }
        }

        if(bpt.size() != MAX) is_passed = false;

        // reset bptree and test variables
        bpt.clear();
        test_size = MAX;
    }

    if(!is_passed) std::cout << "I N S E R T I O N  F A I L E D" << std::endl;

    return is_passed;
}

bool test_bptree_remove() {
    bool is_passed = true;
    const int MAX = 1000, sample_size = 100;
    bptree::BPTree<int> bpt;
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

        // populate BPTree with test[]
        for(std::size_t i = 0; i < test_size; ++i) bpt.insert(test[i]);

        for(std::size_t i = 0; i < original_size; ++i) {
            // pick item to delete
            r = rand() % test_size;

            if(!bpt.remove(test[r]) || !bpt.verify() || bpt.contains(test[r])) {
                is_passed = false;
                break;
            }

            // remove deleted item from test[]
            array_utils::delete_item(test, r, test_size);
        }

        if(bpt.size()) is_passed = false;
    }

    if(!is_passed) std::cout << "R E M O V E D  F A I L E D" << std::endl;

    return is_passed;
}

void test_bptree_interactive() {
    using namespace bptree;

    std::cout << std::string(80, '-') << std::endl
              << "INTERACTIVE TESTS: bptree" << std::endl
              << std::string(80, '-') << std::endl;

    const unsigned MAX_RANGE = 100;
    bool is_inserted = false, is_removed = false;
    BPTree<int> bpt;
    BPTree<int>::Iterator is_found, it;
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
                bpt.clear();
                std::cout << ">> Clear: " << std::endl << bpt << std::endl;

                std::cout << "Iterators: ";
                it = bpt.begin();
                while(it) std::cout << *it++ << ' ';
                std::cout << std::endl;

                std::cout << "Verify: " << bpt.verify() << std::endl;

                break;
            case 'D':
            case 'd':
                std::cin >> key;
                is_removed = bpt.remove(key);

                std::cout << ">> Deletion: " << key;

                if(is_removed)
                    std::cout << " removed";
                else
                    std::cout << " does not exist";

                std::cout << std::endl << bpt << std::endl;

                std::cout << "Iterators: ";
                it = bpt.begin();
                while(it) std::cout << *it++ << ' ';
                std::cout << std::endl;

                std::cout << "Verify: " << bpt.verify() << std::endl;

                break;
            case 'F':
            case 'f':
                std::cin >> key;
                is_found = bpt.find(key);

                std::cout << ">> Find: ";

                if(is_found)
                    std::cout << *is_found << " success.";
                else
                    std::cout << key << " failed. Does not exist";

                std::cout << std::endl << bpt << std::endl;

                std::cout << "Iterators: ";
                it = bpt.begin();
                while(it) std::cout << *it++ << ' ';
                std::cout << std::endl;

                break;
            case 'I':
            case 'i':
                std::cin >> key;
                is_inserted = bpt.insert(key);

                std::cout << ">> Insert: " << key;

                if(is_inserted)
                    std::cout << " success.";
                else
                    std::cout << " failed. Duplicate?";

                std::cout << std::endl << bpt << std::endl;

                std::cout << "Iterators: ";
                it = bpt.begin();
                while(it) std::cout << *it++ << ' ';
                std::cout << std::endl;

                std::cout << "Verify: " << bpt.verify() << std::endl;

                break;
            case 'R':
            case 'r':
                key = rand() % MAX_RANGE;
                is_inserted = bpt.insert(key);

                std::cout << ">> Random insert: " << key;

                if(is_inserted)
                    std::cout << " success.";
                else
                    std::cout << " failed. Duplicate?";

                std::cout << std::endl << bpt << std::endl;

                std::cout << "Iterators: ";
                it = bpt.begin();
                while(it) std::cout << *it++ << ' ';
                std::cout << std::endl;

                std::cout << "Verify: " << bpt.verify() << std::endl;

                break;
            case 'S':
            case 's':
                std::cout << ">> Size: " << bpt.size() << std::endl
                          << std::endl;
                break;
            default:
                std::cout << "Invalid Choice" << std::endl << std::endl;
        }
    } while(c != 'X' && c != 'x');
}
