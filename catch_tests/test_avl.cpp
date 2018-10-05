#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "../include/avl.h"
#include "../lib/catch.hpp"

template <typename T>  // check node for within balance limit
void assert_balance_limit(const bst_node::TreeNode<T>* root);

// check root is greater than left but less than right
template <typename T>
void check_node_order(const bst_node::TreeNode<T>*& root);

SCENARIO("Adelson-Velsky and Landis Class", "[avl]") {
    using namespace avl;

    bool is_found = false, is_erased = false, is_inserted = false;
    AVL<int> avl;
    const int SORTED_SIZE = 199;
    int sorted_items[SORTED_SIZE];
    const bst_node::TreeNode<int>* root = nullptr;
    bst_node::TreeNode<int>* found = nullptr;
    std::vector<int> ascending_items, descending_items, random_items;

    // populate sorted array
    int index = 0;
    for(int i = -99; i <= 99; ++i) {
        sorted_items[index] = i;
        ++index;
    }

    // populate vector with ascending items
    for(int i = 1; i <= 200; ++i) ascending_items.push_back(i);

    // populate vector with descending items
    for(int i = -1; i >= -200; --i) descending_items.push_back(i);

    // populate random_items and perform STL random_shuffle
    random_items = ascending_items;
    srand(time(nullptr));
    std::random_shuffle(random_items.begin(), random_items.end());

    REQUIRE(ascending_items.size());
    REQUIRE(descending_items.size());
    REQUIRE(random_items.size());

    GIVEN("insertion: w/ ascending items, root's balance is within limits") {
        // insert ascending items from vector to avl
        for(int i : ascending_items) {
            is_inserted = avl.insert(i);
            REQUIRE(is_inserted == true);

            // access avl's root
            root = avl.root();
            REQUIRE(root != nullptr);

            // assert all nodes are in order and within balance limits
            bst_node::preorder(root, assert_balance_limit<int>);
            bst_node::preorder(root, check_node_order<int>);
        }

        THEN("unique items are tested with fail reinsertions") {
            // assert reinserting items will fail
            for(int i : ascending_items) {
                is_inserted = avl.insert(i);
                REQUIRE(is_inserted == false);
            }
        }
    }

    GIVEN("insertion: w/ descending items, root's balance is within limits") {
        // insert descending items from vector to avl
        for(int i : descending_items) {
            is_inserted = avl.insert(i);
            REQUIRE(is_inserted == true);

            // access avl's root
            root = avl.root();
            REQUIRE(root != nullptr);

            // assert all nodes are in order and within balance limits
            bst_node::preorder(root, assert_balance_limit<int>);
            bst_node::preorder(root, check_node_order<int>);
        }

        THEN("unique items are tested with fail reinsertions") {
            // assert reinserting items will fail
            for(int i : descending_items) {
                is_inserted = avl.insert(i);
                REQUIRE(is_inserted == false);
            }
        }
    }

    GIVEN("insertion: w/ random items, root's balance is within limits") {
        // insert random items from vector to avl
        for(int i : random_items) {
            is_inserted = avl.insert(i);
            REQUIRE(is_inserted == true);

            // access avl's root
            root = avl.root();
            REQUIRE(root != nullptr);

            // assert all nodes are in order and within balance limits
            bst_node::preorder(root, assert_balance_limit<int>);
            bst_node::preorder(root, check_node_order<int>);
        }

        THEN("unique items are tested with fail reinsertions") {
            // assert reinserting items will fail
            for(int i : random_items) {
                is_inserted = avl.insert(i);
                REQUIRE(is_inserted == false);
            }
        }
    }

    // testing searching, clearing and erasing items in AVL
    GIVEN("avl inserted with ascending items") {
        // insert ascending items from vector to avl
        for(int i : ascending_items) {
            is_inserted = avl.insert(i);
            REQUIRE(is_inserted == true);
        }

        THEN("searching: with 'valid' ascending items are found") {
            // assert all previously inserted items are found in avl
            for(int i : ascending_items) {
                is_found = avl.search(i, found);

                REQUIRE(is_found == true);
                REQUIRE(found->_item == i);
            }
        }

        THEN("searching: with 'invalid' descending items are NOT found") {
            // assert all items that are not inserted are NOT found in avl
            for(int i : descending_items) {
                is_found = avl.search(i, found);

                REQUIRE(is_found == false);
                REQUIRE(found == nullptr);
            }
        }

        THEN("clearing: all items are removed and items are NOT found") {
            avl.clear();

            // access avl's root
            root = avl.root();

            REQUIRE(root == nullptr);

            // searching
            for(int i : ascending_items) {
                is_found = avl.search(i, found);

                REQUIRE(is_found == false);
                REQUIRE(found == nullptr);
            }
        }

        WHEN(
            "erasing: all 'valid' items are moved and all nodes at every "
            "erasure is within balance limits") {
            // erase all items in avl
            for(int i : ascending_items) {
                is_erased = avl.erase(i);
                REQUIRE(is_erased == true);

                // access avl's root
                root = avl.root();

                // assert all nodes are in order and within balance limits
                bst_node::preorder(root, assert_balance_limit<int>);
                bst_node::preorder(root, check_node_order<int>);
            }

            THEN("searching: items are NOT found") {
                for(int i : ascending_items) {
                    is_found = avl.search(i, found);

                    REQUIRE(is_found == false);
                    REQUIRE(found == nullptr);
                }
            }
        }

        THEN("erasing: all 'invalid' items fail to remove") {
            // erase all items in avl
            for(int i : descending_items) {
                is_erased = avl.erase(i);
                REQUIRE(is_erased == false);
            }
        }
    }

    GIVEN("AVL's overloaded constructor with sorted_items") {
        // overloaded ALV constructor
        AVL<int> avl_sorted_items(sorted_items, SORTED_SIZE);

        THEN("all nodes are within balance limits") {
            // access avl's root
            root = avl_sorted_items.root();
            REQUIRE(root != nullptr);

            // assert all nodes are in order and within balance limits
            bst_node::preorder(root, assert_balance_limit<int>);
            bst_node::preorder(root, check_node_order<int>);
        }
    }

    GIVEN(
        "AVL's copy constructor and assignment op: avl is inserted with "
        "ascending items") {
        // insert ascending items from vector to avl
        for(int i : ascending_items) {
            is_inserted = avl.insert(i);
            REQUIRE(is_inserted == true);
        }

        WHEN("avl_copy(avl) and avl_assign = avl") {
            AVL<int> avl_copy(avl);
            AVL<int> avl_assign;
            avl_assign = avl;

            THEN("all items in avl are in avl_copy and avl_assign") {
                for(int i : ascending_items) {
                    // assert for avl_copy
                    is_found = avl_copy.search(i, found);
                    REQUIRE(is_found == true);
                    REQUIRE(found->_item == i);

                    // assert for avl_assign
                    is_found = avl_assign.search(i, found);
                    REQUIRE(is_found == true);
                    REQUIRE(found->_item == i);
                }
            }

            THEN(
                "all nodes in avl_copy and avl_assign are within balance "
                "limits") {
                // access avl_copy's root
                root = avl_copy.root();
                REQUIRE(root != nullptr);

                // assert all nodes are in order and within balance limits
                bst_node::preorder(root, assert_balance_limit<int>);
                bst_node::preorder(root, check_node_order<int>);

                // access avl_assign's root
                root = avl_assign.root();
                REQUIRE(root != nullptr);

                // assert all nodes are in order and within balance limits
                bst_node::preorder(root, assert_balance_limit<int>);
                bst_node::preorder(root, check_node_order<int>);
            }

            THEN("avl_copy and avl_assign is unique via avl modifications") {
                // add descending items to avl
                for(int i : descending_items) {
                    is_inserted = avl.insert(i);
                    REQUIRE(is_inserted == true);
                }

                // searching for items added to AVL are not in avl_copy
                for(int i : descending_items) {
                    // assert for avl_copy
                    is_found = avl_copy.search(i, found);
                    REQUIRE(is_found == false);
                    REQUIRE(found == nullptr);

                    // assert for avl_assign
                    is_found = avl_assign.search(i, found);
                    REQUIRE(is_found == false);
                    REQUIRE(found == nullptr);
                }
            }

            THEN("avl_copy is unique from avl via avl deletions") {
                // clearing avl will not clear avl_copy
                avl.clear();

                // assert avl_copy still have items
                for(int i : ascending_items) {
                    // assert for avl_copy
                    is_found = avl_copy.search(i, found);
                    REQUIRE(is_found == true);
                    REQUIRE(found->_item == i);

                    // assert for avl_assign
                    is_found = avl_assign.search(i, found);
                    REQUIRE(is_found == true);
                    REQUIRE(found->_item == i);
                }
            }
        }
    }

    GIVEN("addition compound op: avl ascending items (with set 1)") {
        // insert ascending items from vector to avl
        for(int i : ascending_items) {
            is_inserted = avl.insert(i);
            REQUIRE(is_inserted == true);

            // access avl's root
            root = avl.root();
            REQUIRE(root != nullptr);

            // assert all nodes are in order and within balance limits
            bst_node::preorder(root, assert_balance_limit<int>);
            bst_node::preorder(root, check_node_order<int>);
        }

        THEN(
            "adding avl_ascending (with set 2) to avl, avl_ascending's nodes "
            "are all within balance limits") {
            AVL<int> avl_ascending;
            std::vector<int> ascending_items2;

            // populate vector with ascending items
            for(int i = 201; i <= 400; ++i) ascending_items2.push_back(i);

            for(int i : ascending_items2) {
                is_inserted = avl_ascending.insert(i);
                REQUIRE(is_inserted == true);

                // access avl's root
                root = avl_ascending.root();
                REQUIRE(root != nullptr);

                // assert all nodes are in order and within balance limits
                bst_node::preorder(root, assert_balance_limit<int>);
                bst_node::preorder(root, check_node_order<int>);
            }
        }
    }

    GIVEN("addition compound op: avl descending items (with set 1)") {
        // insert ascending items from vector to avl
        for(int i : descending_items) {
            is_inserted = avl.insert(i);
            REQUIRE(is_inserted == true);

            // access avl's root
            root = avl.root();
            REQUIRE(root != nullptr);

            // assert all nodes are in order and within balance limits
            bst_node::preorder(root, assert_balance_limit<int>);
            bst_node::preorder(root, check_node_order<int>);
        }

        THEN(
            "adding avl_descending (with set 2) to avl, avl_descending's nodes "
            "are all within balance limits") {
            AVL<int> avl_descending;
            std::vector<int> descending_items2;

            // populate vector with ascending items
            for(int i = -201; i <= -400; --i) descending_items2.push_back(i);

            for(int i : descending_items2) {
                is_inserted = avl_descending.insert(i);
                REQUIRE(is_inserted == true);

                // access avl's root
                root = avl_descending.root();
                REQUIRE(root != nullptr);

                // assert all nodes are in order and within balance limits
                bst_node::preorder(root, assert_balance_limit<int>);
                bst_node::preorder(root, check_node_order<int>);
            }
        }
    }

    GIVEN("addition compound op: avl random items (with set 1)") {
        // insert random items (from ascending set) from vector to avl
        for(int i : random_items) {
            is_inserted = avl.insert(i);
            REQUIRE(is_inserted == true);

            // access avl's root
            root = avl.root();
            REQUIRE(root != nullptr);

            // assert all nodes are in order and within balance limits
            bst_node::preorder(root, assert_balance_limit<int>);
            bst_node::preorder(root, check_node_order<int>);
        }

        THEN(
            "adding avl_random (with set 2) to avl, avl_random's nodes "
            "are all within balance limits") {
            AVL<int> avl_random;
            std::vector<int> random_items2;

            // populate vector with descending items and do random_shuffle
            for(int i = -1; i <= -200; --i) random_items2.push_back(i);
            std::random_shuffle(random_items2.begin(), random_items2.end());

            for(int i : random_items2) {
                is_inserted = avl_random.insert(i);
                REQUIRE(is_inserted == true);

                // access avl's root
                root = avl_random.root();
                REQUIRE(root != nullptr);

                // assert all nodes are in order and within balance limits
                bst_node::preorder(root, assert_balance_limit<int>);
                bst_node::preorder(root, check_node_order<int>);
            }
        }
    }
}

template <typename T>
void assert_balance_limit(const bst_node::TreeNode<T>* root) {
    if(root) {
        bool is_within_limit = false;
        int factor = 2;

        factor = root->balance_factor();
        is_within_limit = factor >= -1 && factor <= 1;
        REQUIRE(is_within_limit);
    }
}

template <typename T>
void check_node_order(const bst_node::TreeNode<T>*& root) {
    if(root) {
        if(root->_left) REQUIRE(root->_item > root->_left->_item);
        if(root->_right) REQUIRE(root->_item < root->_right->_item);
    }
}
