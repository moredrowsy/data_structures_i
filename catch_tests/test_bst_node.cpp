#include <algorithm>  // std::shuffle
#include <ctime>      // std::time
#include <random>     // std::default_random_engine
#include <vector>     // std::vector
#include "../include/bst_node.h"
#include "../lib/catch.hpp"

// check node for within balance limit and node is greater than left but less
// than right
template <typename T>
void assert_b_limits_and_order(bst_node::TreeNode<T>* root);

SCENARIO("Binary Search Tree Node", "[bst_node]") {
    using namespace bst_node;

    GIVEN("a root with nodes inserted with items") {  // test insert nodes
        TreeNode<int>* root = nullptr;

        // insert nodes
        tree_insert(root, 50);
        tree_insert(root, 70);
        tree_insert(root, 55);
        tree_insert(root, 60);
        tree_insert(root, 40);
        tree_insert(root, 30);
        tree_insert(root, 45);

        REQUIRE(root != nullptr);

        // assert nodes are inserted in the correct bst order and height
        REQUIRE(root->_item == 50);
        REQUIRE(root->_height == 3);
        REQUIRE(root->balance_factor() == 1);
        REQUIRE(root->_left->_item == 40);
        REQUIRE(root->_left->_height == 1);
        REQUIRE(root->_left->balance_factor() == 0);
        REQUIRE(root->_left->_left->_item == 30);
        REQUIRE(root->_left->_left->_height == 0);
        REQUIRE(root->_left->_left->balance_factor() == 0);
        REQUIRE(root->_left->_left->_left == nullptr);
        REQUIRE(root->_left->_left->_right == nullptr);
        REQUIRE(root->_left->_right->_item == 45);
        REQUIRE(root->_left->_right->_height == 0);
        REQUIRE(root->_left->_right->balance_factor() == 0);
        REQUIRE(root->_left->_right->_left == nullptr);
        REQUIRE(root->_left->_right->_right == nullptr);
        REQUIRE(root->_right->_item == 70);
        REQUIRE(root->_right->_height == 2);
        REQUIRE(root->_right->balance_factor() == -2);
        REQUIRE(root->_right->_left->_item == 55);
        REQUIRE(root->_right->_left->_height == 1);
        REQUIRE(root->_right->_left->balance_factor() == 1);
        REQUIRE(root->_right->_right == nullptr);
        REQUIRE(root->_right->_left->_left == nullptr);
        REQUIRE(root->_right->_left->_right->_item == 60);
        REQUIRE(root->_right->_left->_right->_height == 0);
        REQUIRE(root->_right->_left->_right->balance_factor() == 0);
        REQUIRE(root->_right->_left->_right->_left == nullptr);
        REQUIRE(root->_right->_left->_right->_right == nullptr);

        WHEN("searching bst") {
            bool is_found = false;
            TreeNode<int>* found = nullptr;

            THEN("of 50 will return a non-nullptr and/or or true") {
                REQUIRE(is_found == false);
                REQUIRE(root->_item == 50);

                // test tree_search with pointer return
                found = tree_search(root, 50);

                REQUIRE(found != nullptr);
                REQUIRE(found->_item == 50);

                found = nullptr;

                // test tree_search with boolean return
                is_found = tree_search(root, 50, found);

                REQUIRE(found != nullptr);
                REQUIRE(is_found == true);
                REQUIRE(found->_item == 50);
            }

            THEN("of 70 will return a non-nullptr and/or or true") {
                REQUIRE(is_found == false);
                REQUIRE(root->_right->_item == 70);

                // test tree_search with pointer return
                found = tree_search(root, 70);

                REQUIRE(found != nullptr);
                REQUIRE(found->_item == 70);

                found = nullptr;

                // test tree_search with boolean return
                is_found = tree_search(root, 70, found);

                REQUIRE(found != nullptr);
                REQUIRE(is_found == true);
                REQUIRE(found->_item == 70);
            }

            THEN("of 40 will return a non-nullptr and/or true") {
                REQUIRE(is_found == false);
                REQUIRE(root->_left->_item == 40);

                // test tree_search with pointer return
                found = tree_search(root, 40);

                REQUIRE(found != nullptr);
                REQUIRE(found->_item == 40);

                found = nullptr;

                // test tree_search with boolean return
                is_found = tree_search(root, 40, found);

                REQUIRE(found != nullptr);
                REQUIRE(is_found == true);
                REQUIRE(found->_item == 40);
            }

            THEN("of invalid items will return a nullptr and/or false") {
                REQUIRE(is_found == false);

                // test tree_search with pointer return
                found = tree_search(root, -1);
                REQUIRE(found == nullptr);

                found = tree_search(root, 0);
                REQUIRE(found == nullptr);

                found = tree_search(root, 100);
                REQUIRE(found == nullptr);

                // test tree_search with boolean return
                is_found = tree_search(root, -1, found);
                REQUIRE(found == nullptr);
                REQUIRE(is_found == false);

                is_found = tree_search(root, 0, found);
                REQUIRE(found == nullptr);
                REQUIRE(is_found == false);

                is_found = tree_search(root, 100, found);
                REQUIRE(found == nullptr);
                REQUIRE(is_found == false);
            }
        }

        WHEN("is cleared will return root with nullptr") {
            // test deallocation of bst nodes
            tree_clear(root);

            REQUIRE(root == nullptr);
        }

        WHEN("erasing a node in bst") {
            bool is_erased = false, is_found = false;
            TreeNode<int>* found = nullptr;

            THEN("of 50 will return true and search will fail") {
                REQUIRE(is_erased == false);
                REQUIRE(root != nullptr);
                REQUIRE(root->_item == 50);

                // test erase
                is_erased = tree_erase(root, 50);

                REQUIRE(is_erased == true);
                REQUIRE(root != nullptr);
                REQUIRE(root->_item == 45);

                // assert that 50 is not found
                is_found = tree_search(root, 50, found);
                REQUIRE(is_found == false);
                REQUIRE(found == nullptr);

                // assert bst has correct order and height
                REQUIRE(root->_item == 45);
                REQUIRE(root->_height == 3);
                REQUIRE(root->balance_factor() == 1);
                REQUIRE(root->_left->_item == 40);
                REQUIRE(root->_left->_height == 1);
                REQUIRE(root->_left->balance_factor() == -1);
                REQUIRE(root->_left->_left->_item == 30);
                REQUIRE(root->_left->_left->_height == 0);
                REQUIRE(root->_left->_left->balance_factor() == 0);
                REQUIRE(root->_left->_left->_left == nullptr);
                REQUIRE(root->_left->_left->_right == nullptr);
                REQUIRE(root->_left->_right == nullptr);
                REQUIRE(root->_right->_item == 70);
                REQUIRE(root->_right->_height == 2);
                REQUIRE(root->_right->balance_factor() == -2);
                REQUIRE(root->_right->_left->_item == 55);
                REQUIRE(root->_right->_left->_height == 1);
                REQUIRE(root->_right->_left->balance_factor() == 1);
                REQUIRE(root->_right->_left->_left == nullptr);
                REQUIRE(root->_right->_left->_right->_item == 60);
                REQUIRE(root->_right->_left->_right->_height == 0);
                REQUIRE(root->_right->_left->_right->balance_factor() == 0);
                REQUIRE(root->_right->_left->_right->_left == nullptr);
                REQUIRE(root->_right->_left->_right->_right == nullptr);
                REQUIRE(root->_right->_right == nullptr);
            }

            THEN("of 70 will return true and search will fail") {
                REQUIRE(is_erased == false);
                REQUIRE(root->_right != nullptr);
                REQUIRE(root->_right->_item == 70);

                // test erase
                is_erased = tree_erase(root, 70);

                REQUIRE(is_erased == true);
                REQUIRE(root->_right != nullptr);
                REQUIRE(root->_right->_item == 60);

                // assert that 70 is not found
                is_found = tree_search(root, 70, found);
                REQUIRE(is_found == false);
                REQUIRE(found == nullptr);

                // assert bst has correct order and height
                REQUIRE(root->_item == 50);
                REQUIRE(root->_height == 2);
                REQUIRE(root->balance_factor() == 0);
                REQUIRE(root->_left->_item == 40);
                REQUIRE(root->_left->_height == 1);
                REQUIRE(root->_left->balance_factor() == 0);
                REQUIRE(root->_left->_left->_item == 30);
                REQUIRE(root->_left->_left->_height == 0);
                REQUIRE(root->_left->_left->balance_factor() == 0);
                REQUIRE(root->_left->_left->_left == nullptr);
                REQUIRE(root->_left->_left->_right == nullptr);
                REQUIRE(root->_left->_right->_item == 45);
                REQUIRE(root->_left->_right->_height == 0);
                REQUIRE(root->_left->_right->balance_factor() == 0);
                REQUIRE(root->_left->_right->_left == nullptr);
                REQUIRE(root->_left->_right->_right == nullptr);
                REQUIRE(root->_right->_item == 60);
                REQUIRE(root->_right->_height == 1);
                REQUIRE(root->_right->balance_factor() == -1);
                REQUIRE(root->_right->_left->_item == 55);
                REQUIRE(root->_right->_left->_height == 0);
                REQUIRE(root->_right->_left->balance_factor() == 0);
                REQUIRE(root->_right->_right == nullptr);
            }

            THEN("of 40 will return true and search will fail") {
                REQUIRE(is_erased == false);
                REQUIRE(root->_left != nullptr);
                REQUIRE(root->_left->_item == 40);

                // test erase
                is_erased = tree_erase(root, 40);

                REQUIRE(is_erased == true);
                REQUIRE(root->_left != nullptr);
                REQUIRE(root->_left->_item == 30);

                // assert that 40 is not found
                is_found = tree_search(root, 40, found);
                REQUIRE(is_found == false);
                REQUIRE(found == nullptr);

                // assert bst has correct order and height
                REQUIRE(root->_item == 50);
                REQUIRE(root->_height == 3);
                REQUIRE(root->balance_factor() == 1);
                REQUIRE(root->_left->_item == 30);
                REQUIRE(root->_left->_height == 1);
                REQUIRE(root->_left->balance_factor() == 1);
                REQUIRE(root->_left->_left == nullptr);
                REQUIRE(root->_left->_right->_item == 45);
                REQUIRE(root->_left->_right->_height == 0);
                REQUIRE(root->_left->_right->balance_factor() == 0);
                REQUIRE(root->_left->_right->_left == nullptr);
                REQUIRE(root->_left->_right->_right == nullptr);
                REQUIRE(root->_right->_item == 70);
                REQUIRE(root->_right->_height == 2);
                REQUIRE(root->_right->balance_factor() == -2);
                REQUIRE(root->_right->_left->_item == 55);
                REQUIRE(root->_right->_left->_height == 1);
                REQUIRE(root->_right->_left->balance_factor() == 1);
                REQUIRE(root->_right->_right == nullptr);
                REQUIRE(root->_right->_left->_left == nullptr);
                REQUIRE(root->_right->_left->_right->_item == 60);
                REQUIRE(root->_right->_left->_right->_height == 0);
                REQUIRE(root->_right->_left->_right->balance_factor() == 0);
                REQUIRE(root->_right->_left->_right->_left == nullptr);
                REQUIRE(root->_right->_left->_right->_right == nullptr);
            }

            THEN("of 60 will return true and search will fail") {
                REQUIRE(is_erased == false);
                REQUIRE(root->_right->_left->_right != nullptr);
                REQUIRE(root->_right->_left->_right->_item == 60);

                // test erase
                is_erased = tree_erase(root, 60);

                REQUIRE(is_erased == true);
                REQUIRE(root->_right->_left->_right == nullptr);

                // assert that 60 is not found
                is_found = tree_search(root, 60, found);
                REQUIRE(is_found == false);
                REQUIRE(found == nullptr);

                // assert bst has correct order
                REQUIRE(root->_item == 50);
                REQUIRE(root->_height == 2);
                REQUIRE(root->balance_factor() == 0);
                REQUIRE(root->_left->_item == 40);
                REQUIRE(root->_left->_height == 1);
                REQUIRE(root->_left->balance_factor() == 0);
                REQUIRE(root->_left->_left->_item == 30);
                REQUIRE(root->_left->_left->_height == 0);
                REQUIRE(root->_left->_left->balance_factor() == 0);
                REQUIRE(root->_left->_left->_left == nullptr);
                REQUIRE(root->_left->_left->_right == nullptr);
                REQUIRE(root->_left->_right->_item == 45);
                REQUIRE(root->_left->_right->_height == 0);
                REQUIRE(root->_left->_right->balance_factor() == 0);
                REQUIRE(root->_left->_right->_left == nullptr);
                REQUIRE(root->_left->_right->_right == nullptr);
                REQUIRE(root->_right->_item == 70);
                REQUIRE(root->_right->_height == 1);
                REQUIRE(root->_right->balance_factor() == -1);
                REQUIRE(root->_right->_left->_item == 55);
                REQUIRE(root->_right->_left->_height == 0);
                REQUIRE(root->_right->_left->balance_factor() == 0);
                REQUIRE(root->_right->_right == nullptr);
                REQUIRE(root->_right->_left->_left == nullptr);
                REQUIRE(root->_right->_left->_right == nullptr);
            }

            THEN("of invalid items will return false") {
                REQUIRE(is_erased == false);

                // test erase
                is_erased = tree_erase(root, -1);
                REQUIRE(is_erased == false);

                // test erase
                is_erased = tree_erase(root, 1);
                REQUIRE(is_erased == false);

                // test erase
                is_erased = tree_erase(root, 2);
                REQUIRE(is_erased == false);
            }
        }

        WHEN("removing max") {
            bool is_found = false;
            TreeNode<int>* found = nullptr;
            int max = -1;

            THEN("will return 70, point to left subtree and search will fail") {
                REQUIRE(root->_right != nullptr);
                REQUIRE(root->_right->_item == 70);

                // test remove max
                tree_remove_max(root, max);

                REQUIRE(max == 70);

                // assert that 70 is not found
                is_found = tree_search(root, 70, found);
                REQUIRE(is_found == false);
                REQUIRE(found == nullptr);

                // assert bst has correct order
                REQUIRE(root->_item == 50);
                REQUIRE(root->_height == 2);
                REQUIRE(root->balance_factor() == 0);
                REQUIRE(root->_left->_item == 40);
                REQUIRE(root->_left->_height == 1);
                REQUIRE(root->_left->balance_factor() == 0);
                REQUIRE(root->_left->_left->_item == 30);
                REQUIRE(root->_left->_left->_height == 0);
                REQUIRE(root->_left->_left->balance_factor() == 0);
                REQUIRE(root->_left->_left->_left == nullptr);
                REQUIRE(root->_left->_left->_right == nullptr);
                REQUIRE(root->_left->_right->_item == 45);
                REQUIRE(root->_left->_right->_height == 0);
                REQUIRE(root->_left->_right->balance_factor() == 0);
                REQUIRE(root->_left->_right->_left == nullptr);
                REQUIRE(root->_left->_right->_right == nullptr);
                REQUIRE(root->_right->_item == 55);
                REQUIRE(root->_right->_height == 1);
                REQUIRE(root->_right->balance_factor() == 1);
                REQUIRE(root->_right->_right->_item == 60);
                REQUIRE(root->_right->_right->_height == 0);
                REQUIRE(root->_right->_right->balance_factor() == 0);
                REQUIRE(root->_right->_right->_left == nullptr);
                REQUIRE(root->_right->_right->_right == nullptr);

                THEN("will return 60 when removing max again") {
                    // test remove max
                    tree_remove_max(root, max);

                    REQUIRE(max == 60);

                    // assert bst has correct order
                    REQUIRE(root->_item == 50);
                    REQUIRE(root->_height == 2);
                    REQUIRE(root->balance_factor() == -1);
                    REQUIRE(root->_left->_item == 40);
                    REQUIRE(root->_left->_height == 1);
                    REQUIRE(root->_left->balance_factor() == 0);
                    REQUIRE(root->_left->_left->_item == 30);
                    REQUIRE(root->_left->_left->_height == 0);
                    REQUIRE(root->_left->_left->balance_factor() == 0);
                    REQUIRE(root->_left->_left->_left == nullptr);
                    REQUIRE(root->_left->_left->_right == nullptr);
                    REQUIRE(root->_left->_right->_item == 45);
                    REQUIRE(root->_left->_right->_height == 0);
                    REQUIRE(root->_left->_right->balance_factor() == 0);
                    REQUIRE(root->_left->_right->_left == nullptr);
                    REQUIRE(root->_left->_right->_right == nullptr);
                    REQUIRE(root->_right->_item == 55);
                    REQUIRE(root->_right->_height == 0);
                    REQUIRE(root->_right->balance_factor() == 0);
                    REQUIRE(root->_right->_right == nullptr);
                    REQUIRE(root->_right->_left == nullptr);
                }
            }
        }

        WHEN("copying bst from root to root_a") {
            TreeNode<int>* root_a = nullptr;

            root_a = tree_copy(root);

            REQUIRE(root_a != nullptr);

            // assert root_a has items of root
            REQUIRE(root_a->_item == 50);
            REQUIRE(root_a->_height == 3);
            REQUIRE(root_a->balance_factor() == 1);
            REQUIRE(root_a->_left->_item == 40);
            REQUIRE(root_a->_left->_height == 1);
            REQUIRE(root_a->_left->balance_factor() == 0);
            REQUIRE(root_a->_left->_left->_item == 30);
            REQUIRE(root_a->_left->_left->_height == 0);
            REQUIRE(root_a->_left->_left->balance_factor() == 0);
            REQUIRE(root_a->_left->_left->_left == nullptr);
            REQUIRE(root_a->_left->_left->_right == nullptr);
            REQUIRE(root_a->_left->_right->_item == 45);
            REQUIRE(root_a->_left->_right->_height == 0);
            REQUIRE(root_a->_left->_right->balance_factor() == 0);
            REQUIRE(root_a->_left->_right->_left == nullptr);
            REQUIRE(root_a->_left->_right->_right == nullptr);
            REQUIRE(root_a->_right->_item == 70);
            REQUIRE(root_a->_right->_height == 2);
            REQUIRE(root_a->_right->balance_factor() == -2);
            REQUIRE(root_a->_right->_left->_item == 55);
            REQUIRE(root_a->_right->_left->_height == 1);
            REQUIRE(root_a->_right->_left->balance_factor() == 1);
            REQUIRE(root_a->_right->_right == nullptr);
            REQUIRE(root_a->_right->_left->_left == nullptr);
            REQUIRE(root_a->_right->_left->_right->_item == 60);
            REQUIRE(root_a->_right->_left->_right->_height == 0);
            REQUIRE(root_a->_right->_left->_right->balance_factor() == 0);
            REQUIRE(root_a->_right->_left->_right->_left == nullptr);
            REQUIRE(root_a->_right->_left->_right->_right == nullptr);

            THEN("changing items in root_a will have no effect on root") {
                // change items
                root_a->_item = 51;
                root_a->_left->_item = 41;
                root_a->_right->_item = 71;

                // assert changes to root_a are correct
                REQUIRE(root_a->_item == 51);
                REQUIRE(root_a->_left->_item == 41);
                REQUIRE(root_a->_right->_item == 71);

                // assert changes to original root are NOT changed
                REQUIRE(root->_item == 50);
                REQUIRE(root->_left->_item == 40);
                REQUIRE(root->_right->_item == 70);

                // deallocate root_a
                tree_clear(root_a);

                REQUIRE(root_a == nullptr);
                REQUIRE(root != nullptr);
            }

            tree_clear(root_a);
        }

        WHEN("adding a non-empty root_a to root") {
            TreeNode<int>* root_a = nullptr;

            tree_insert(root_a, 1);
            tree_insert(root_a, 100);
            tree_insert(root_a, 35);
            tree_insert(root_a, 65);
            tree_insert(root_a, -1);

            // assert root_a has correct items
            REQUIRE(root_a->_item == 1);
            REQUIRE(root_a->_height == 3);
            REQUIRE(root_a->balance_factor() == 2);
            REQUIRE(root_a->_left->_item == -1);
            REQUIRE(root_a->_left->_height == 0);
            REQUIRE(root_a->_left->balance_factor() == 0);
            REQUIRE(root_a->_left->_left == nullptr);
            REQUIRE(root_a->_left->_right == nullptr);
            REQUIRE(root_a->_right->_item == 100);
            REQUIRE(root_a->_right->_height == 2);
            REQUIRE(root_a->_right->balance_factor() == -2);
            REQUIRE(root_a->_right->_left->_item == 35);
            REQUIRE(root_a->_right->_left->_height == 1);
            REQUIRE(root_a->_right->_left->balance_factor() == 1);
            REQUIRE(root_a->_right->_left->_right->_item == 65);
            REQUIRE(root_a->_right->_left->_right->_height == 0);
            REQUIRE(root_a->_right->_left->_right->balance_factor() == 0);

            // test add root_a to root
            tree_add(root, root_a);

            THEN("root will have root_a's items added") {
                REQUIRE(root->_item == 50);
                REQUIRE(root->_height == 4);
                REQUIRE(root->balance_factor() == 0);
                REQUIRE(root->_left->_item == 40);
                REQUIRE(root->_left->_height == 3);
                REQUIRE(root->_left->balance_factor() == -2);
                REQUIRE(root->_left->_left->_item == 30);
                REQUIRE(root->_left->_left->_height == 2);
                REQUIRE(root->_left->_left->balance_factor() == -1);
                REQUIRE(root->_left->_left->_left->_item == 1);
                REQUIRE(root->_left->_left->_left->_height == 1);
                REQUIRE(root->_left->_left->_left->balance_factor() == -1);
                REQUIRE(root->_left->_left->_left->_left->_item == -1);
                REQUIRE(root->_left->_left->_left->_left->_height == 0);
                REQUIRE(root->_left->_left->_left->_left->balance_factor() ==
                        0);
                REQUIRE(root->_left->_left->_left->_right == nullptr);
                REQUIRE(root->_left->_left->_right->_item == 35);
                REQUIRE(root->_left->_left->_right->_height == 0);
                REQUIRE(root->_left->_left->_right->balance_factor() == 0);
                REQUIRE(root->_left->_left->_right->_left == nullptr);
                REQUIRE(root->_left->_left->_right->_right == nullptr);
                REQUIRE(root->_left->_right->_item == 45);
                REQUIRE(root->_left->_right->_height == 0);
                REQUIRE(root->_left->_right->balance_factor() == 0);
                REQUIRE(root->_left->_right->_left == nullptr);
                REQUIRE(root->_left->_right->_right == nullptr);
                REQUIRE(root->_right->_item == 70);
                REQUIRE(root->_right->_height == 3);
                REQUIRE(root->_right->balance_factor() == -2);
                REQUIRE(root->_right->_left->_item == 55);
                REQUIRE(root->_right->_left->_height == 2);
                REQUIRE(root->_right->_left->balance_factor() == 2);
                REQUIRE(root->_right->_right->_item == 100);
                REQUIRE(root->_right->_right->_height == 0);
                REQUIRE(root->_right->_right->balance_factor() == 0);
                REQUIRE(root->_right->_left->_left == nullptr);
                REQUIRE(root->_right->_left->_right->_item == 60);
                REQUIRE(root->_right->_left->_right->_height == 1);
                REQUIRE(root->_right->_left->_right->balance_factor() == 1);
                REQUIRE(root->_right->_left->_right->_left == nullptr);
                REQUIRE(root->_right->_left->_right->_right->_item == 65);
                REQUIRE(root->_right->_left->_right->_right->_height == 0);
                REQUIRE(root->_right->_left->_right->_right->balance_factor() ==
                        0);
            }

            tree_clear(root_a);
        }

        WHEN("adding root to empty root_a") {
            TreeNode<int>* root_a = nullptr;

            // test add root to empty root_a
            tree_add(root_a, root);

            REQUIRE(root_a != nullptr);

            THEN("root_a will have root's items") {
                // root_a has same items in same order of root
                REQUIRE(root->_item == 50);
                REQUIRE(root->_height == 3);
                REQUIRE(root->balance_factor() == 1);
                REQUIRE(root->_left->_item == 40);
                REQUIRE(root->_left->_height == 1);
                REQUIRE(root->_left->balance_factor() == 0);
                REQUIRE(root->_left->_left->_item == 30);
                REQUIRE(root->_left->_left->_height == 0);
                REQUIRE(root->_left->_left->balance_factor() == 0);
                REQUIRE(root->_left->_left->_left == nullptr);
                REQUIRE(root->_left->_left->_right == nullptr);
                REQUIRE(root->_left->_right->_item == 45);
                REQUIRE(root->_left->_right->_height == 0);
                REQUIRE(root->_left->_right->balance_factor() == 0);
                REQUIRE(root->_left->_right->_left == nullptr);
                REQUIRE(root->_left->_right->_right == nullptr);
                REQUIRE(root->_right->_item == 70);
                REQUIRE(root->_right->_height == 2);
                REQUIRE(root->_right->balance_factor() == -2);
                REQUIRE(root->_right->_left->_item == 55);
                REQUIRE(root->_right->_left->_height == 1);
                REQUIRE(root->_right->_left->balance_factor() == 1);
                REQUIRE(root->_right->_right == nullptr);
                REQUIRE(root->_right->_left->_left == nullptr);
                REQUIRE(root->_right->_left->_right->_item == 60);
                REQUIRE(root->_right->_left->_right->_height == 0);
                REQUIRE(root->_right->_left->_right->balance_factor() == 0);
                REQUIRE(root->_right->_left->_right->_left == nullptr);
                REQUIRE(root->_right->_left->_right->_right == nullptr);
            }

            tree_clear(root_a);
        }

        // deallocate nodes
        tree_clear(root);
        REQUIRE(root == nullptr);
    }

    GIVEN("array of sorted items, a *TreeNode is returned") {
        TreeNode<int>* root;
        const int SIZE_EMPTY = 0, SIZE_A = 1, SIZE_B = 2, SIZE_C = 3,
                  SIZE_D = 4;
        int array_empty[SIZE_A],
            array_a[SIZE_A] = {1}, array_b[SIZE_B] = {1, 2},
            array_c[SIZE_C] = {1, 2, 3}, array_D[SIZE_D] = {1, 2, 3, 4};

        WHEN("size 0 array will return a nullptr") {
            root = tree_from_sorted_list(array_empty, SIZE_EMPTY);
            REQUIRE(root == nullptr);
        }

        WHEN("size 1 array will add 1 item to Complete Binary tree root") {
            root = tree_from_sorted_list(array_a, SIZE_A);
            REQUIRE(root != nullptr);

            // assert that item is added
            REQUIRE(root->_item == 1);
            REQUIRE(root->_height == 0);
            REQUIRE(root->balance_factor() == 0);
            REQUIRE(root->_left == nullptr);
            REQUIRE(root->_right == nullptr);

            tree_clear(root);
        }

        WHEN("size 2 array will add 2 items to Complete Binary tree root") {
            root = tree_from_sorted_list(array_b, SIZE_B);
            REQUIRE(root != nullptr);

            // assert that items are added
            REQUIRE(root->_item == 2);
            REQUIRE(root->_height == 1);
            REQUIRE(root->balance_factor() == -1);
            REQUIRE(root->_left->_item == 1);
            REQUIRE(root->_left->_height == 0);
            REQUIRE(root->_left->balance_factor() == 0);
            REQUIRE(root->_right == nullptr);
            REQUIRE(root->_left->_left == nullptr);
            REQUIRE(root->_left->_right == nullptr);

            tree_clear(root);
        }

        WHEN("size 3 array will add 3 items to Complete Binary tree root") {
            root = tree_from_sorted_list(array_c, SIZE_C);
            REQUIRE(root != nullptr);

            // assert that items are added
            REQUIRE(root->_item == 2);
            REQUIRE(root->_height == 1);
            REQUIRE(root->balance_factor() == 0);
            REQUIRE(root->_left->_item == 1);
            REQUIRE(root->_left->_height == 0);
            REQUIRE(root->_left->balance_factor() == 0);
            REQUIRE(root->_right->_item == 3);
            REQUIRE(root->_right->_height == 0);
            REQUIRE(root->_right->balance_factor() == 0);
            REQUIRE(root->_left->_left == nullptr);
            REQUIRE(root->_left->_right == nullptr);
            REQUIRE(root->_right->_left == nullptr);
            REQUIRE(root->_right->_right == nullptr);
        }

        WHEN("size 4 array will add 4 items to Complete Binary tree root") {
            root = tree_from_sorted_list(array_D, SIZE_D);
            REQUIRE(root != nullptr);

            // assert that items are added
            REQUIRE(root->_item == 3);
            REQUIRE(root->_height == 2);
            REQUIRE(root->balance_factor() == -1);
            REQUIRE(root->_left->_item == 2);
            REQUIRE(root->_left->_height == 1);
            REQUIRE(root->_left->balance_factor() == -1);
            REQUIRE(root->_right->_item == 4);
            REQUIRE(root->_right->_height == 0);
            REQUIRE(root->_right->balance_factor() == 0);
            REQUIRE(root->_left->_left->_item == 1);
            REQUIRE(root->_left->_left->_height == 0);
            REQUIRE(root->_left->_left->balance_factor() == 0);
            REQUIRE(root->_left->_left->_left == nullptr);
            REQUIRE(root->_left->_left->_right == nullptr);
            REQUIRE(root->_left->_right == nullptr);
            REQUIRE(root->_right->_left == nullptr);
            REQUIRE(root->_right->_right == nullptr);
        }

        tree_clear(root);
    }

    GIVEN("a root with unbalanced nodes, case 1: left, left") {
        TreeNode<int>* left_left = nullptr;

        // left_left  inserts
        tree_insert(left_left, 60);
        tree_insert(left_left, 70);
        tree_insert(left_left, 40);
        tree_insert(left_left, 50);
        tree_insert(left_left, 20);
        tree_insert(left_left, 30);
        tree_insert(left_left, 10);

        REQUIRE(left_left->_item == 60);
        REQUIRE(left_left->_height == 3);
        REQUIRE(left_left->balance_factor() == -2);
        REQUIRE(left_left->_left->_item == 40);
        REQUIRE(left_left->_left->_height == 2);
        REQUIRE(left_left->_left->balance_factor() == -1);
        REQUIRE(left_left->_left->_left->_item == 20);
        REQUIRE(left_left->_left->_left->_height == 1);
        REQUIRE(left_left->_left->_left->balance_factor() == 0);
        REQUIRE(left_left->_left->_right->_item == 50);
        REQUIRE(left_left->_left->_right->_height == 0);
        REQUIRE(left_left->_left->_right->balance_factor() == 0);
        REQUIRE(left_left->_left->_left->_left->_item == 10);
        REQUIRE(left_left->_left->_left->_left->_height == 0);
        REQUIRE(left_left->_left->_left->_left->balance_factor() == 0);
        REQUIRE(left_left->_left->_left->_right->_item == 30);
        REQUIRE(left_left->_left->_left->_right->_height == 0);
        REQUIRE(left_left->_left->_left->_right->balance_factor() == 0);
        REQUIRE(left_left->_right->_item == 70);
        REQUIRE(left_left->_right->_height == 0);
        REQUIRE(left_left->_right->balance_factor() == 0);

        THEN("rotate right() will produce full/complete binary tree") {
            left_left = rotate_right(left_left);
            REQUIRE(left_left->_item == 40);
            REQUIRE(left_left->_height == 2);
            REQUIRE(left_left->balance_factor() == 0);
            REQUIRE(left_left->_left->_item == 20);
            REQUIRE(left_left->_left->_height == 1);
            REQUIRE(left_left->_left->balance_factor() == 0);
            REQUIRE(left_left->_left->_left->_item == 10);
            REQUIRE(left_left->_left->_left->_height == 0);
            REQUIRE(left_left->_left->_left->balance_factor() == 0);
            REQUIRE(left_left->_left->_right->_item == 30);
            REQUIRE(left_left->_left->_right->_height == 0);
            REQUIRE(left_left->_left->_right->balance_factor() == 0);
            REQUIRE(left_left->_right->_item == 60);
            REQUIRE(left_left->_right->_height == 1);
            REQUIRE(left_left->_right->balance_factor() == 0);
            REQUIRE(left_left->_right->_left->_item == 50);
            REQUIRE(left_left->_right->_left->_height == 0);
            REQUIRE(left_left->_right->_left->balance_factor() == 0);
            REQUIRE(left_left->_right->_right->_item == 70);
            REQUIRE(left_left->_right->_right->_height == 0);
            REQUIRE(left_left->_right->_right->balance_factor() == 0);
        }

        tree_clear(left_left);
    }

    GIVEN("a root with unbalanced nodes, case 2: right, right") {
        TreeNode<int>* right_right = nullptr;

        // right_right  inserts
        tree_insert(right_right, 40);
        tree_insert(right_right, 30);
        tree_insert(right_right, 60);
        tree_insert(right_right, 50);
        tree_insert(right_right, 80);
        tree_insert(right_right, 70);
        tree_insert(right_right, 90);

        REQUIRE(right_right->_item == 40);
        REQUIRE(right_right->_height == 3);
        REQUIRE(right_right->balance_factor() == 2);
        REQUIRE(right_right->_left->_item == 30);
        REQUIRE(right_right->_left->_height == 0);
        REQUIRE(right_right->_left->balance_factor() == 0);
        REQUIRE(right_right->_right->_item == 60);
        REQUIRE(right_right->_right->_height == 2);
        REQUIRE(right_right->_right->balance_factor() == 1);
        REQUIRE(right_right->_right->_left->_item == 50);
        REQUIRE(right_right->_right->_left->_height == 0);
        REQUIRE(right_right->_right->_left->balance_factor() == 0);
        REQUIRE(right_right->_right->_right->_item == 80);
        REQUIRE(right_right->_right->_right->_height == 1);
        REQUIRE(right_right->_right->_right->balance_factor() == 0);
        REQUIRE(right_right->_right->_right->_left->_item == 70);
        REQUIRE(right_right->_right->_right->_left->_height == 0);
        REQUIRE(right_right->_right->_right->_left->balance_factor() == 0);
        REQUIRE(right_right->_right->_right->_right->_item == 90);
        REQUIRE(right_right->_right->_right->_right->_height == 0);
        REQUIRE(right_right->_right->_right->_right->balance_factor() == 0);

        THEN("rotate left() will produce full/complete binary tree") {
            right_right = rotate_left(right_right);

            REQUIRE(right_right->_item == 60);
            REQUIRE(right_right->_height == 2);
            REQUIRE(right_right->balance_factor() == 0);
            REQUIRE(right_right->_left->_item == 40);
            REQUIRE(right_right->_left->_height == 1);
            REQUIRE(right_right->_left->balance_factor() == 0);
            REQUIRE(right_right->_left->_left->_item == 30);
            REQUIRE(right_right->_left->_left->_height == 0);
            REQUIRE(right_right->_left->_left->balance_factor() == 0);
            REQUIRE(right_right->_left->_right->_item == 50);
            REQUIRE(right_right->_left->_right->_height == 0);
            REQUIRE(right_right->_left->_right->balance_factor() == 0);
            REQUIRE(right_right->_right->_item == 80);
            REQUIRE(right_right->_right->_height == 1);
            REQUIRE(right_right->_right->balance_factor() == 0);
            REQUIRE(right_right->_right->_left->_item == 70);
            REQUIRE(right_right->_right->_left->_height == 0);
            REQUIRE(right_right->_right->_left->balance_factor() == 0);
            REQUIRE(right_right->_right->_right->_item == 90);
            REQUIRE(right_right->_right->_right->_height == 0);
            REQUIRE(right_right->_right->_right->balance_factor() == 0);
        }

        tree_clear(right_right);
    }

    GIVEN("a root with unbalanced nodes, case 3: left, right") {
        TreeNode<int>* left_right = nullptr;

        // left_right inserts
        tree_insert(left_right, 60);
        tree_insert(left_right, 70);
        tree_insert(left_right, 20);
        tree_insert(left_right, 10);
        tree_insert(left_right, 40);
        tree_insert(left_right, 30);
        tree_insert(left_right, 50);

        REQUIRE(left_right->_item == 60);
        REQUIRE(left_right->_height == 3);
        REQUIRE(left_right->balance_factor() == -2);
        REQUIRE(left_right->_left->_item == 20);
        REQUIRE(left_right->_left->_height == 2);
        REQUIRE(left_right->_left->balance_factor() == 1);
        REQUIRE(left_right->_left->_left->_item == 10);
        REQUIRE(left_right->_left->_left->_height == 0);
        REQUIRE(left_right->_left->_left->balance_factor() == 0);
        REQUIRE(left_right->_left->_right->_item == 40);
        REQUIRE(left_right->_left->_right->_height == 1);
        REQUIRE(left_right->_left->_right->balance_factor() == 0);
        REQUIRE(left_right->_left->_right->_left->_item == 30);
        REQUIRE(left_right->_left->_right->_left->_height == 0);
        REQUIRE(left_right->_left->_right->_left->balance_factor() == 0);
        REQUIRE(left_right->_left->_right->_right->_item == 50);
        REQUIRE(left_right->_left->_right->_right->_height == 0);
        REQUIRE(left_right->_left->_right->_right->balance_factor() == 0);
        REQUIRE(left_right->_right->_item == 70);
        REQUIRE(left_right->_right->_height == 0);
        REQUIRE(left_right->_right->balance_factor() == 0);

        THEN("rotate() will produce full/complete binary tree") {
            left_right = rotate(left_right);

            REQUIRE(left_right->_item == 40);
            REQUIRE(left_right->_height == 2);
            REQUIRE(left_right->balance_factor() == 0);
            REQUIRE(left_right->_left->_item == 20);
            REQUIRE(left_right->_left->_height == 1);
            REQUIRE(left_right->_left->balance_factor() == 0);
            REQUIRE(left_right->_left->_left->_item == 10);
            REQUIRE(left_right->_left->_left->_height == 0);
            REQUIRE(left_right->_left->_left->balance_factor() == 0);
            REQUIRE(left_right->_left->_right->_item == 30);
            REQUIRE(left_right->_left->_right->_height == 0);
            REQUIRE(left_right->_left->_right->balance_factor() == 0);
            REQUIRE(left_right->_right->_item == 60);
            REQUIRE(left_right->_right->_height == 1);
            REQUIRE(left_right->_right->balance_factor() == 0);
            REQUIRE(left_right->_right->_left->_item == 50);
            REQUIRE(left_right->_right->_left->_height == 0);
            REQUIRE(left_right->_right->_left->balance_factor() == 0);
            REQUIRE(left_right->_right->_right->_item == 70);
            REQUIRE(left_right->_right->_right->_height == 0);
            REQUIRE(left_right->_right->_right->balance_factor() == 0);
        }

        tree_clear(left_right);
    }

    GIVEN("a root with unbalanced nodes, case 3: right, left") {
        TreeNode<int>* right_left = nullptr;

        // right_left inserts
        tree_insert(right_left, 40);
        tree_insert(right_left, 30);
        tree_insert(right_left, 80);
        tree_insert(right_left, 60);
        tree_insert(right_left, 50);
        tree_insert(right_left, 70);
        tree_insert(right_left, 90);

        REQUIRE(right_left->_item == 40);
        REQUIRE(right_left->_height == 3);
        REQUIRE(right_left->balance_factor() == 2);
        REQUIRE(right_left->_left->_item == 30);
        REQUIRE(right_left->_left->_height == 0);
        REQUIRE(right_left->_left->balance_factor() == 0);
        REQUIRE(right_left->_right->_item == 80);
        REQUIRE(right_left->_right->_height == 2);
        REQUIRE(right_left->_right->balance_factor() == -1);
        REQUIRE(right_left->_right->_left->_item == 60);
        REQUIRE(right_left->_right->_left->_height == 1);
        REQUIRE(right_left->_right->_left->balance_factor() == 0);
        REQUIRE(right_left->_right->_left->_left->_item == 50);
        REQUIRE(right_left->_right->_left->_left->_height == 0);
        REQUIRE(right_left->_right->_left->_left->balance_factor() == 0);
        REQUIRE(right_left->_right->_left->_right->_item == 70);
        REQUIRE(right_left->_right->_left->_right->_height == 0);
        REQUIRE(right_left->_right->_left->_right->balance_factor() == 0);
        REQUIRE(right_left->_right->_right->_item == 90);
        REQUIRE(right_left->_right->_right->_height == 0);
        REQUIRE(right_left->_right->_right->balance_factor() == 0);

        THEN("rotate() will produce full/complete binary tree") {
            right_left = rotate(right_left);

            REQUIRE(right_left->_item == 60);
            REQUIRE(right_left->_height == 2);
            REQUIRE(right_left->balance_factor() == 0);
            REQUIRE(right_left->_left->_item == 40);
            REQUIRE(right_left->_left->_height == 1);
            REQUIRE(right_left->_left->balance_factor() == 0);
            REQUIRE(right_left->_left->_left->_item == 30);
            REQUIRE(right_left->_left->_left->_height == 0);
            REQUIRE(right_left->_left->_left->balance_factor() == 0);
            REQUIRE(right_left->_left->_right->_item == 50);
            REQUIRE(right_left->_left->_right->_height == 0);
            REQUIRE(right_left->_left->_right->balance_factor() == 0);
            REQUIRE(right_left->_right->_item == 80);
            REQUIRE(right_left->_right->_height == 1);
            REQUIRE(right_left->_right->balance_factor() == 0);
            REQUIRE(right_left->_right->_left->_item == 70);
            REQUIRE(right_left->_right->_left->_height == 0);
            REQUIRE(right_left->_right->_left->balance_factor() == 0);
            REQUIRE(right_left->_right->_right->_item == 90);
            REQUIRE(right_left->_right->_right->_height == 0);
            REQUIRE(right_left->_right->_right->balance_factor() == 0);
        }

        tree_clear(right_left);
    }

    GIVEN("an empty node") {
        bool is_erased = false, is_inserted = false;
        TreeNode<int>* root = nullptr;

        WHEN(
            "inserting balanced nodes in ascending order, root's balance "
            "factor will not exceed limit [-1,1]") {
            const int items[] = {-9, -8, -7, -6, -5, -4, -3, -2, -1, 0,
                                 1,  2,  3,  4,  5,  6,  7,  8,  9};

            for(int i : items) {
                is_inserted = tree_insert(root, i, true);
                REQUIRE(is_inserted == true);

                // assert all nodes are in order and within balance limits
                preorder(root, assert_b_limits_and_order<int>);
            }

            THEN(
                "erasing balanced nodes, root's balance factor will not exceed "
                "limit") {
                for(int i : items) {
                    is_erased = tree_erase(root, i, true);

                    if(root) {
                        REQUIRE(is_erased == true);

                        // assert all nodes are in order and within balance
                        // limits
                        preorder(root, assert_b_limits_and_order<int>);
                    }
                }
            }
        }

        WHEN(
            "inserting balanced nodes in descending order, root's balance "
            "factor will not exceed limit [-1,1]") {
            const int items[] = {9,  8,  7,  6,  5,  4,  3,  2,  1, 0,
                                 -1, -2, -3, -4, -5, -6, -7, -8, -9};

            for(int i : items) {
                is_inserted = tree_insert(root, i, true);
                REQUIRE(is_inserted == true);

                // assert all nodes are in order and within balance limits
                preorder(root, assert_b_limits_and_order<int>);
            }

            THEN(
                "erasing balanced nodes, root's balance factor will not exceed "
                "limit") {
                for(int i : items) {
                    is_erased = tree_erase(root, i, true);

                    if(root) {
                        REQUIRE(is_erased == true);

                        // assert all nodes are in order and within balance
                        // limits
                        preorder(root, assert_b_limits_and_order<int>);
                    }
                }
            }
        }

        WHEN(
            "inserting balanced nodes in random order, root's balance "
            "factor will not exceed limit [-1,1]") {
            // fill vector with items from -99 to 99
            std::vector<int> items;
            for(int i = -99; i <= 99; ++i) {
                items.push_back(i);
            }

            std::shuffle(items.begin(), items.end(),
                         std::default_random_engine(time(nullptr)));

            for(int i : items) {
                is_inserted = tree_insert(root, i, true);
                REQUIRE(is_inserted == true);

                // assert all nodes are in order and within balance limits
                preorder(root, assert_b_limits_and_order<int>);
            }

            THEN(
                "erasing balanced nodes, root's balance factor will not exceed "
                "limit") {
                for(int i : items) {
                    is_erased = tree_erase(root, i, true);

                    if(root) {
                        REQUIRE(is_erased == true);

                        // assert all nodes are in order and within balance
                        // limits
                        preorder(root, assert_b_limits_and_order<int>);
                    }
                }
            }

            THEN(
                "copying root to root_a with balanced == true, root_a's "
                "balance factor will not exceed limit [-1, 1]") {
                TreeNode<int>* root_a = nullptr;

                root_a = tree_copy(root, true);

                REQUIRE(root_a != nullptr);

                // assert all nodes are in order and within balance limits
                preorder(root_a, assert_b_limits_and_order<int>);

                tree_clear(root_a);
            }

            THEN(
                "adding items in root_a to root, root's balance factor will "
                "not exceed limit [-1, 1]") {
                TreeNode<int>* root_a = nullptr;

                // fill vector with items from -200 to -100
                std::vector<int> items_lower_bound;
                for(int i = -200; i <= -100; ++i) {
                    items_lower_bound.push_back(i);
                }

                // fill vector with items from 100 to 200
                std::vector<int> items_upper_bound;
                for(int i = 100; i <= 200; ++i) {
                    items_upper_bound.push_back(i);
                }

                // fill vector with items from lower bound and upper bound
                std::vector<int> items_a;
                for(int i : items_lower_bound) {
                    items_a.push_back(i);
                }
                for(int i : items_upper_bound) {
                    items_a.push_back(i);
                }

                std::shuffle(items_a.begin(), items_a.end(),
                             std::default_random_engine(time(nullptr)));

                // insert items_a to root_a and assert root_a is within limits
                for(int i : items_a) {
                    is_inserted = tree_insert(root_a, i, true);
                    REQUIRE(is_inserted == true);

                    // assert all nodes are in order and within balance
                    // limits
                    preorder(root_a, assert_b_limits_and_order<int>);
                }

                // add root_a to root
                tree_add(root, root_a, true);

                // assert all nodes are in order and within balance
                // limits
                preorder(root, assert_b_limits_and_order<int>);

                tree_clear(root_a);
            }
        }

        tree_clear(root);
    }
}

template <typename T>
void assert_b_limits_and_order(bst_node::TreeNode<T>* root) {
    if(root) {
        bool is_within_limit = false;
        int factor = 2;

        factor = root->balance_factor();
        is_within_limit = factor >= -1 && factor <= 1;
        REQUIRE(is_within_limit);

        if(root->_left) REQUIRE(root->_item > root->_left->_item);
        if(root->_right) REQUIRE(root->_item < root->_right->_item);
    }
}
