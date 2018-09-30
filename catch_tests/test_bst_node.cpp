#include "../include/bst_node.h"
#include "../lib/catch.hpp"

SCENARIO("Binary Search Tree Node", "[bst_node]") {
    using namespace bst_node;

    GIVEN("a root with nodes inserted with items") {  // test insert nodes
        TreeNode<int>* root = nullptr;

        REQUIRE(root == nullptr);

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
        REQUIRE(root->_left->_item == 40);
        REQUIRE(root->_left->_height == 1);
        REQUIRE(root->_left->_left->_item == 30);
        REQUIRE(root->_left->_left->_height == 0);
        REQUIRE(root->_left->_left->_left == nullptr);
        REQUIRE(root->_left->_left->_right == nullptr);
        REQUIRE(root->_left->_right->_item == 45);
        REQUIRE(root->_left->_right->_height == 0);
        REQUIRE(root->_left->_right->_left == nullptr);
        REQUIRE(root->_left->_right->_right == nullptr);
        REQUIRE(root->_right->_item == 70);
        REQUIRE(root->_right->_height == 2);
        REQUIRE(root->_right->_left->_item == 55);
        REQUIRE(root->_right->_left->_height == 1);
        REQUIRE(root->_right->_right == nullptr);
        REQUIRE(root->_right->_left->_left == nullptr);
        REQUIRE(root->_right->_left->_right->_item == 60);
        REQUIRE(root->_right->_left->_right->_height == 0);
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
                REQUIRE(root->_left->_item == 40);
                REQUIRE(root->_left->_height == 1);
                REQUIRE(root->_left->_left->_item == 30);
                REQUIRE(root->_left->_left->_height == 0);
                REQUIRE(root->_left->_left->_left == nullptr);
                REQUIRE(root->_left->_left->_right == nullptr);
                REQUIRE(root->_left->_right->_item == 45);
                REQUIRE(root->_left->_right->_height == 0);
                REQUIRE(root->_left->_right->_left == nullptr);
                REQUIRE(root->_left->_right->_right == nullptr);
                REQUIRE(root->_right->_item == 60);
                REQUIRE(root->_right->_height == 1);
                REQUIRE(root->_right->_left->_item == 55);
                REQUIRE(root->_right->_left->_height == 0);
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
                REQUIRE(root->_left->_item == 30);
                REQUIRE(root->_left->_height == 1);
                REQUIRE(root->_left->_left == nullptr);
                REQUIRE(root->_left->_right->_item == 45);
                REQUIRE(root->_left->_right->_height == 0);
                REQUIRE(root->_left->_right->_left == nullptr);
                REQUIRE(root->_left->_right->_right == nullptr);
                REQUIRE(root->_right->_item == 70);
                REQUIRE(root->_right->_height == 2);
                REQUIRE(root->_right->_left->_item == 55);
                REQUIRE(root->_right->_left->_height == 1);
                REQUIRE(root->_right->_right == nullptr);
                REQUIRE(root->_right->_left->_left == nullptr);
                REQUIRE(root->_right->_left->_right->_item == 60);
                REQUIRE(root->_right->_left->_right->_height == 0);
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
                REQUIRE(root->_left->_item == 40);
                REQUIRE(root->_left->_height == 1);
                REQUIRE(root->_left->_left->_item == 30);
                REQUIRE(root->_left->_left->_height == 0);
                REQUIRE(root->_left->_left->_left == nullptr);
                REQUIRE(root->_left->_left->_right == nullptr);
                REQUIRE(root->_left->_right->_item == 45);
                REQUIRE(root->_left->_right->_height == 0);
                REQUIRE(root->_left->_right->_left == nullptr);
                REQUIRE(root->_left->_right->_right == nullptr);
                REQUIRE(root->_right->_item == 70);
                REQUIRE(root->_right->_height == 1);
                REQUIRE(root->_right->_left->_item == 55);
                REQUIRE(root->_right->_left->_height == 0);
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
                REQUIRE(root->_left->_item == 40);
                REQUIRE(root->_left->_height == 1);
                REQUIRE(root->_left->_left->_item == 30);
                REQUIRE(root->_left->_left->_height == 0);
                REQUIRE(root->_left->_left->_left == nullptr);
                REQUIRE(root->_left->_left->_right == nullptr);
                REQUIRE(root->_left->_right->_item == 45);
                REQUIRE(root->_left->_right->_height == 0);
                REQUIRE(root->_left->_right->_left == nullptr);
                REQUIRE(root->_left->_right->_right == nullptr);
                REQUIRE(root->_right->_item == 55);
                REQUIRE(root->_right->_height == 1);
                REQUIRE(root->_right->_right->_item == 60);
                REQUIRE(root->_right->_right->_height == 0);
                REQUIRE(root->_right->_right->_left == nullptr);
                REQUIRE(root->_right->_right->_right == nullptr);

                THEN("will return 60 when removing max again") {
                    // test remove max
                    tree_remove_max(root, max);

                    REQUIRE(max == 60);

                    // assert bst has correct order
                    REQUIRE(root->_item == 50);
                    REQUIRE(root->_height == 2);
                    REQUIRE(root->_left->_item == 40);
                    REQUIRE(root->_left->_height == 1);
                    REQUIRE(root->_left->_left->_item == 30);
                    REQUIRE(root->_left->_left->_height == 0);
                    REQUIRE(root->_left->_left->_left == nullptr);
                    REQUIRE(root->_left->_left->_right == nullptr);
                    REQUIRE(root->_left->_right->_item == 45);
                    REQUIRE(root->_left->_right->_height == 0);
                    REQUIRE(root->_left->_right->_left == nullptr);
                    REQUIRE(root->_left->_right->_right == nullptr);
                    REQUIRE(root->_right->_item == 55);
                    REQUIRE(root->_right->_height == 0);
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
            REQUIRE(root->_item == 50);
            REQUIRE(root->_height == 3);
            REQUIRE(root->_left->_item == 40);
            REQUIRE(root->_left->_height == 1);
            REQUIRE(root->_left->_left->_item == 30);
            REQUIRE(root->_left->_left->_height == 0);
            REQUIRE(root->_left->_left->_left == nullptr);
            REQUIRE(root->_left->_left->_right == nullptr);
            REQUIRE(root->_left->_right->_item == 45);
            REQUIRE(root->_left->_right->_height == 0);
            REQUIRE(root->_left->_right->_left == nullptr);
            REQUIRE(root->_left->_right->_right == nullptr);
            REQUIRE(root->_right->_item == 70);
            REQUIRE(root->_right->_height == 2);
            REQUIRE(root->_right->_left->_item == 55);
            REQUIRE(root->_right->_left->_height == 1);
            REQUIRE(root->_right->_right == nullptr);
            REQUIRE(root->_right->_left->_left == nullptr);
            REQUIRE(root->_right->_left->_right->_item == 60);
            REQUIRE(root->_right->_left->_right->_height == 0);
            REQUIRE(root->_right->_left->_right->_left == nullptr);
            REQUIRE(root->_right->_left->_right->_right == nullptr);

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
            REQUIRE(root_a->_left->_item == -1);
            REQUIRE(root_a->_left->_height == 0);
            REQUIRE(root_a->_left->_left == nullptr);
            REQUIRE(root_a->_left->_right == nullptr);
            REQUIRE(root_a->_right->_item == 100);
            REQUIRE(root_a->_right->_height == 2);
            REQUIRE(root_a->_right->_left->_item == 35);
            REQUIRE(root_a->_right->_left->_height == 1);
            REQUIRE(root_a->_right->_left->_right->_item == 65);
            REQUIRE(root_a->_right->_left->_right->_height == 0);

            // test add root_a to root
            tree_add(root, root_a);

            THEN("root will have root_a's items added") {
                REQUIRE(root->_item == 50);
                REQUIRE(root->_height == 4);
                REQUIRE(root->_left->_item == 40);
                REQUIRE(root->_left->_height == 3);
                REQUIRE(root->_left->_left->_item == 30);
                REQUIRE(root->_left->_left->_height == 2);
                REQUIRE(root->_left->_left->_left->_item == 1);
                REQUIRE(root->_left->_left->_left->_height == 1);
                REQUIRE(root->_left->_left->_left->_left->_item == -1);
                REQUIRE(root->_left->_left->_left->_left->_height == 0);
                REQUIRE(root->_left->_left->_left->_right == nullptr);
                REQUIRE(root->_left->_left->_right->_item == 35);
                REQUIRE(root->_left->_left->_right->_height == 0);
                REQUIRE(root->_left->_left->_right->_left == nullptr);
                REQUIRE(root->_left->_left->_right->_right == nullptr);
                REQUIRE(root->_left->_right->_item == 45);
                REQUIRE(root->_left->_right->_height == 0);
                REQUIRE(root->_left->_right->_left == nullptr);
                REQUIRE(root->_left->_right->_right == nullptr);
                REQUIRE(root->_right->_item == 70);
                REQUIRE(root->_right->_height == 3);
                REQUIRE(root->_right->_left->_item == 55);
                REQUIRE(root->_right->_left->_height == 2);
                REQUIRE(root->_right->_right->_item == 100);
                REQUIRE(root->_right->_right->_height == 0);
                REQUIRE(root->_right->_left->_left == nullptr);
                REQUIRE(root->_right->_left->_right->_item == 60);
                REQUIRE(root->_right->_left->_right->_height == 1);
                REQUIRE(root->_right->_left->_right->_left == nullptr);
                REQUIRE(root->_right->_left->_right->_right->_item == 65);
                REQUIRE(root->_right->_left->_right->_right->_height == 0);
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
                REQUIRE(root->_left->_item == 40);
                REQUIRE(root->_left->_height == 1);
                REQUIRE(root->_left->_left->_item == 30);
                REQUIRE(root->_left->_left->_height == 0);
                REQUIRE(root->_left->_left->_left == nullptr);
                REQUIRE(root->_left->_left->_right == nullptr);
                REQUIRE(root->_left->_right->_item == 45);
                REQUIRE(root->_left->_right->_height == 0);
                REQUIRE(root->_left->_right->_left == nullptr);
                REQUIRE(root->_left->_right->_right == nullptr);
                REQUIRE(root->_right->_item == 70);
                REQUIRE(root->_right->_height == 2);
                REQUIRE(root->_right->_left->_item == 55);
                REQUIRE(root->_right->_left->_height == 1);
                REQUIRE(root->_right->_right == nullptr);
                REQUIRE(root->_right->_left->_left == nullptr);
                REQUIRE(root->_right->_left->_right->_item == 60);
                REQUIRE(root->_right->_left->_right->_height == 0);
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
            REQUIRE(root->_left->_item == 1);
            REQUIRE(root->_left->_height == 0);
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
            REQUIRE(root->_left->_item == 1);
            REQUIRE(root->_left->_height == 0);
            REQUIRE(root->_right->_item == 3);
            REQUIRE(root->_right->_height == 0);
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
            REQUIRE(root->_left->_item == 2);
            REQUIRE(root->_left->_height == 1);
            REQUIRE(root->_right->_item == 4);
            REQUIRE(root->_right->_height == 0);
            REQUIRE(root->_left->_left->_item == 1);
            REQUIRE(root->_left->_left->_height == 0);
            REQUIRE(root->_left->_left->_left == nullptr);
            REQUIRE(root->_left->_left->_right == nullptr);
            REQUIRE(root->_left->_right == nullptr);
            REQUIRE(root->_right->_left == nullptr);
            REQUIRE(root->_right->_right == nullptr);
        }

        tree_clear(root);
    }
}
