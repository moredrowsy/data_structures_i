#include <cstdlib>                   // srand(), rand()
#include <iostream>                  // stream objects
#include "../include/array_utils.h"  // array utilities
#include "../include/bpt_map.h"      // BPTree's Map/MMap class
#include "../include/bptree.h"       // BPTree class
#include "../include/pair.h"         // Pair/MPair class
#include "../include/sort.h"         // shuffle()

void test_bptree_auto(int sample_size, int tree_size, bool report);
bool test_bptree_auto(int tree_size, bool report);
bool test_bptree_big_three();
bool test_bptree_insert();
bool test_bptree_remove();
void test_bptree_interactive();

bool test_map_and_mmap_auto();
void test_map_interactive();
void test_mmap_interactive();

int main() {
    srand(0);

    test_bptree_interactive();
    test_map_interactive();
    test_mmap_interactive();

    test_bptree_big_three();
    test_bptree_insert();
    test_bptree_remove();
    test_bptree_auto(100, 1000, false);
    test_map_and_mmap_auto();

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
    bptree::BPTree<int> bpt1(false, 2), bpt2(false, 2);

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

    if(!is_passed)
        std::cout << "B I G  T H R E E  F A I L E D" << std::endl;
    else
        std::cout << "B+TREE TESTS: BIG THREE PASSED" << std::endl;

    return is_passed;
}

bool test_bptree_insert() {
    bool is_passed = true;
    const int MAX = 1000, sample_size = 100;
    bptree::BPTree<int> bpt(false, 2);
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

    if(!is_passed)
        std::cout << "I N S E R T I O N  F A I L E D" << std::endl;
    else
        std::cout << "B+TREE TESTS: INSERTIONS PASSED" << std::endl;

    return is_passed;
}

bool test_bptree_remove() {
    bool is_passed = true;
    const int MAX = 1000, sample_size = 100;
    bptree::BPTree<int> bpt(false, 2);
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

        if(!bpt.verify()) {
            is_passed = false;
            break;
        }

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
        bpt.clear();
    }

    if(!is_passed)
        std::cout << "R E M O V E D  F A I L E D" << std::endl;
    else
        std::cout << "B+TREE TESTS: REMOVALS PASSED" << std::endl;

    return is_passed;
}

void test_bptree_interactive() {
    using namespace bptree;

    std::cout << std::string(80, '-') << std::endl
              << "INTERACTIVE TESTS: B+Tree" << std::endl
              << std::string(80, '-') << std::endl;

    const unsigned MAX_RANGE = 100;
    bool is_inserted = false, is_removed = false, is_print = false;
    BPTree<int> bpt;
    BPTree<int>::Iterator is_found, it;
    int key;
    char c;

    srand(time(nullptr));

    do {
        std::cout << "[S]ize()  [R]andom  [I]nsert  [D]elete  [F]ind [C]lear"
                     "     e[X]it: ";
        std::cin >> c;

        switch(c) {
            case 'X':
            case 'x':
                is_print = false;
                return;  // exit function
            case 'C':
            case 'c':
                bpt.clear();
                std::cout << ">> Clear: " << std::endl;
                is_print = true;

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
                std::cout << std::endl;
                is_print = true;

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
                std::cout << std::endl;
                is_print = true;

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
                std::cout << std::endl;
                is_print = true;

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
                std::cout << std::endl;
                is_print = true;

                break;
            case 'S':
            case 's':
                std::cout << ">> Size: " << bpt.size() << std::endl
                          << std::endl;
                is_print = false;

                break;
            default:
                std::cout << "Invalid Choice" << std::endl << std::endl;
                is_print = false;
        }

        if(is_print) {
            std::cout << std::endl << bpt << std::endl;

            std::cout << "Iterators: ";
            it = bpt.begin();
            while(it) std::cout << *it++ << ' ';
            std::cout << std::endl;

            std::cout << "Verify: " << bpt.verify() << std::endl << std::endl;
        }
    } while(c != 'X' && c != 'x');
}

bool test_map_and_mmap_auto() {
    using namespace bpt_map;
    using namespace pair;

    const int MAX = 1000;

    Map<int, int> map;
    MMap<int, int> mmap;
    int keys[MAX];
    int values[MAX];

    // populate keys/values
    for(int i = 0; i < MAX; ++i) {
        keys[i] = i;
        values[i] = i;
    }

    // insert keys/values to map and mmap
    for(int i = 0; i < MAX; ++i) {
        map[keys[i]] = values[i];
        mmap[keys[i]] += values[i];

        if(!map.verify() || !mmap.verify()) {
            std::cout << "I N S E R T I O N  F A I L E D" << std::endl;
            return false;
        }

        if(!map.contains(Pair<const int, int>(keys[i], values[i])) ||
           !mmap.contains(keys[i])) {
            std::cout << "I N S E R T  C O N T A I N S  F A I L E D"
                      << std::endl;
            return false;
        }
    }

    // remove keys from map and mmap
    for(int i = 0; i < MAX; ++i) {
        map.erase(keys[i]);
        mmap.erase(keys[i]);

        if(!map.verify() || !mmap.verify()) {
            std::cout << "E R A S E  F A I L E D" << std::endl;
            return false;
        }

        if(map.contains(Pair<const int, int>(keys[i], values[i])) ||
           mmap.contains(keys[i])) {
            std::cout << "E R A S E D  C O N T A I N S  F A I L E D"
                      << std::endl;
            return false;
        }
    }

    std::cout << "MAP/MMAP TESTS: AUTO PASSED" << std::endl;

    return true;
}

void test_map_interactive() {
    using namespace bpt_map;
    typedef pair::Pair<std::string, std::string> Pair;

    std::cout << std::string(80, '-') << std::endl
              << "INTERACTIVE TESTS: Map" << std::endl
              << std::string(80, '-') << std::endl;

    const std::size_t SIZE = 24;
    Map<std::string, std::string> map;
    Map<std::string, std::string>::Iterator it;
    bool is_found = false, is_removed = false, is_print = false;
    char c;
    std::string key, value;

    std::string keys[SIZE] = {"Aleph",  "Beth", "Gimel", "Daleth", "He",
                              "Zayin",  "Heth", "Teth",  "Yodh",   "Kaph",
                              "Lamedh", "Mem",  "Nun",   "Samekh", "Ayin",
                              "Pe",     "Res",  "Sin",   "Taw",    "Waw",
                              "Sade",   "Qoph", "Koppa", "Sampi"};

    std::string values[SIZE] = {"Alpha",  "Beta", "Gamma", "Delta", "Epsilon",
                                "Zeta",   "Eta",  "Theta", "Iota",  "Kappa",
                                "Lambda", "Mu",   "Nu",    "Xi",    "Omicron",
                                "Pi",     "Rho",  "Sigma", "Tau",   "Upsilon",
                                "Phi",    "Chi",  "Psi",   "Omega"};

    srand(time(nullptr));

    do {
        std::cout << std::string(80, '-') << std::endl;
        std::cout << "[S]ize()  [R]andom  [I]nsert  [D]elete  [F]ind [C]lear"
                     "     e[X]it: ";
        std::cin >> c;
        std::cout << std::string(80, '-') << std::endl;

        switch(c) {
            case 'X':
            case 'x':
                is_print = false;
                return;  // exit function
            case 'C':
            case 'c':
                map.clear();
                std::cout << ">> Clear: " << std::endl;
                is_print = true;

                break;
            case 'D':
            case 'd':
                std::cin >> key;
                is_removed = map.erase(key);

                std::cout << ">> Deletion: " << key;

                if(is_removed)
                    std::cout << " removed";
                else
                    std::cout << " does not exist";
                std::cout << std::endl;
                is_print = true;

                break;
            case 'F':
            case 'f':
                std::cin >> key;
                is_found = map.contains(key);

                std::cout << ">> Find: " << key;

                if(is_found)
                    std::cout << " success.";
                else
                    std::cout << " failed. Does not exist";
                std::cout << std::endl;
                is_print = true;

                break;
            case 'I':
            case 'i':
                std::cin >> key >> value;
                map[key] = value;

                std::cout << ">> Insert: " << Pair(key, value) << std::endl;
                is_print = true;

                break;
            case 'R':
            case 'r':
                key = keys[rand() % SIZE];
                value = values[rand() % SIZE];
                map[key] = value;

                std::cout << ">> Random insert: " << Pair(key, value)
                          << std::endl;
                is_print = true;

                break;
            case 'S':
            case 's':
                std::cout << ">> Size: " << map.size() << std::endl;
                is_print = false;

                break;
            default:
                std::cout << "Invalid Choice" << std::endl << std::endl;
                is_print = false;
        }

        if(is_print) {
            std::cout << std::endl;
            map.print_debug();
            std::cout << std::endl;

            std::cout << "Iterators: ";
            it = map.begin();
            while(it) std::cout << "<< " << *it++ << " >> ";
            std::cout << std::endl;

            std::cout << "Verify: " << map.verify() << std::endl << std::endl;
        }
    } while(c != 'X' && c != 'x');
}

void test_mmap_interactive() {
    using namespace bpt_map;
    typedef pair::MPair<std::string, std::string> Pair;

    std::cout << std::string(80, '-') << std::endl
              << "INTERACTIVE TESTS: Multi-Map" << std::endl
              << std::string(80, '-') << std::endl;

    const std::size_t SIZE = 24;
    MMap<std::string, std::string> mmap;
    MMap<std::string, std::string>::Iterator it;
    bool is_found = false, is_removed = false, is_print = false;
    char c;
    std::string key, value;

    std::string keys[SIZE] = {"Aleph",  "Beth", "Gimel", "Daleth", "He",
                              "Zayin",  "Heth", "Teth",  "Yodh",   "Kaph",
                              "Lamedh", "Mem",  "Nun",   "Samekh", "Ayin",
                              "Pe",     "Res",  "Sin",   "Taw",    "Waw",
                              "Sade",   "Qoph", "Koppa", "Sampi"};

    std::string values[SIZE] = {"Alpha",  "Beta", "Gamma", "Delta", "Epsilon",
                                "Zeta",   "Eta",  "Theta", "Iota",  "Kappa",
                                "Lambda", "Mu",   "Nu",    "Xi",    "Omicron",
                                "Pi",     "Rho",  "Sigma", "Tau",   "Upsilon",
                                "Phi",    "Chi",  "Psi",   "Omega"};

    srand(time(nullptr));

    do {
        std::cout << std::string(80, '-') << std::endl;
        std::cout << "[S]ize()  [R]andom  [I]nsert  [D]elete  [F]ind [C]lear"
                     "     e[X]it: ";
        std::cin >> c;
        std::cout << std::string(80, '-') << std::endl;

        switch(c) {
            case 'X':
            case 'x':
                is_print = false;
                return;  // exit function
            case 'C':
            case 'c':
                mmap.clear();
                std::cout << ">> Clear: " << std::endl << mmap << std::endl;
                is_print = true;

                break;
            case 'D':
            case 'd':
                std::cin >> key;
                is_removed = mmap.erase(key);

                std::cout << ">> Deletion: " << key;

                if(is_removed)
                    std::cout << " removed";
                else
                    std::cout << " does not exist";
                std::cout << std::endl;
                is_print = true;

                break;
            case 'F':
            case 'f':
                std::cin >> key;
                is_found = mmap.contains(key);

                std::cout << ">> Find: " << key;

                if(is_found)
                    std::cout << " success.";
                else
                    std::cout << " failed. Does not exist";
                std::cout << std::endl;
                is_print = true;

                break;
            case 'I':
            case 'i':
                std::cin >> key >> value;
                mmap[key] += value;

                std::cout << ">> Insert: " << Pair(key, value) << std::endl;
                is_print = true;

                break;
            case 'R':
            case 'r':
                key = keys[rand() % SIZE];
                value = values[rand() % SIZE];
                mmap[key] += value;

                std::cout << ">> Random insert: " << Pair(key, value)
                          << std::endl;
                is_print = true;

                break;
            case 'S':
            case 's':
                std::cout << ">> Size: " << mmap.size() << std::endl
                          << std::endl;
                is_print = false;

                break;
            default:
                std::cout << "Invalid Choice" << std::endl << std::endl;
                is_print = false;
        }

        if(is_print) {
            std::cout << std::endl;
            mmap.print_debug();
            std::cout << std::endl;

            std::cout << "Iterators: ";
            it = mmap.begin();
            while(it) {
                std::cout << "<< " << it->key << " : " << *it->value << " >> ";
                ++it;
            }
            std::cout << std::endl;

            std::cout << "Verify: " << mmap.verify() << std::endl << std::endl;
        }
    } while(c != 'X' && c != 'x');
}
