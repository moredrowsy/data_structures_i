#include <sstream>
#include "../include/bst.h"
#include "../lib/catch.hpp"

SCENARIO("Binary Search Tree Class", "[bst]") {
    using namespace bst;

    std::string outs;
    std::stringstream ss;

    GIVEN("an empty btree with items inserted") {
        // default constructor
        BST<int> btree;

        // test empty btree
        ss.str("");
        ss << btree;
        REQUIRE(ss.str() == "|||\n");

        // insert items
        btree.insert(50);
        btree.insert(70);
        btree.insert(55);
        btree.insert(60);
        btree.insert(40);
        btree.insert(30);
        btree.insert(45);

        // expected output
        outs =
            "          |||\n"
            "     |70|\n"
            "                    |||\n"
            "               |60|\n"
            "                    |||\n"
            "          |55|\n"
            "               |||\n"
            "|50|\n"
            "               |||\n"
            "          |45|\n"
            "               |||\n"
            "     |40|\n"
            "               |||\n"
            "          |30|\n"
            "               |||\n";

        // assert expected output
        ss.str("");
        ss << btree;
        REQUIRE(ss.str() == outs);

        WHEN("searching in btree") {
            bool is_found = false;
            bst_node::TreeNode<int>* found = nullptr;
            int target = -1;

            THEN("of 50 will return true and found value of 50") {
                target = 50;
                is_found = btree.search(target, found);

                REQUIRE(is_found == true);
                REQUIRE(found->_item == target);
            }

            THEN("of 70 will return true and found value of 70") {
                target = 70;
                is_found = btree.search(target, found);

                REQUIRE(is_found == true);
                REQUIRE(found->_item == target);
            }

            THEN("of 40 will return true and found value of 40") {
                target = 40;
                is_found = btree.search(target, found);

                REQUIRE(is_found == true);
                REQUIRE(found->_item == target);
            }

            THEN("of invalid -1 will return false and found is nullptr") {
                target = -1;
                is_found = btree.search(target, found);

                REQUIRE(is_found == false);
                REQUIRE(found == nullptr);
            }

            THEN("of invalid 0 will return false and found is nullptr") {
                target = 0;
                is_found = btree.search(target, found);

                REQUIRE(is_found == false);
                REQUIRE(found == nullptr);
            }

            THEN("of invalid 100 will return false and found is nullptr") {
                target = 100;
                is_found = btree.search(target, found);

                REQUIRE(is_found == false);
                REQUIRE(found == nullptr);
            }
        }

        WHEN("erasing in btree") {
            bool is_erased = false, is_found = false;
            bst_node::TreeNode<int>* found = nullptr;
            int target = -1;

            THEN("of 50 will return true and search will fail") {
                target = 50;

                // assert erased
                is_erased = btree.erase(target);
                REQUIRE(is_erased == true);

                // assert not found
                is_found = btree.search(target, found);
                REQUIRE(is_found == false);
                REQUIRE(found == nullptr);

                // expected output
                outs =
                    "          |||\n"
                    "     |70|\n"
                    "                    |||\n"
                    "               |60|\n"
                    "                    |||\n"
                    "          |55|\n"
                    "               |||\n"
                    "|45|\n"
                    "          |||\n"
                    "     |40|\n"
                    "               |||\n"
                    "          |30|\n"
                    "               |||\n";

                // assert expected output
                ss.str("");
                ss << btree;
                REQUIRE(ss.str() == outs);

                THEN("of 70 will return true and search will fail") {
                    target = 70;

                    // assert erased
                    is_erased = btree.erase(target);
                    REQUIRE(is_erased == true);

                    // assert not found
                    is_found = btree.search(target, found);
                    REQUIRE(is_found == false);
                    REQUIRE(found == nullptr);

                    // expected output
                    outs =
                        "          |||\n"
                        "     |60|\n"
                        "               |||\n"
                        "          |55|\n"
                        "               |||\n"
                        "|45|\n"
                        "          |||\n"
                        "     |40|\n"
                        "               |||\n"
                        "          |30|\n"
                        "               |||\n";

                    // assert expected output
                    ss.str("");
                    ss << btree;
                    REQUIRE(ss.str() == outs);

                    THEN("of 40 will return true and search will fail") {
                        target = 40;

                        // assert erased
                        is_erased = btree.erase(target);
                        REQUIRE(is_erased == true);

                        // assert not found
                        is_found = btree.search(target, found);
                        REQUIRE(is_found == false);
                        REQUIRE(found == nullptr);

                        // expected output
                        outs =
                            "          |||\n"
                            "     |60|\n"
                            "               |||\n"
                            "          |55|\n"
                            "               |||\n"
                            "|45|\n"
                            "          |||\n"
                            "     |30|\n"
                            "          |||\n";

                        // assert expected output
                        ss.str("");
                        ss << btree;
                        REQUIRE(ss.str() == outs);

                        THEN("of 60 will return true and search will fail") {
                            target = 60;

                            // assert erased
                            is_erased = btree.erase(target);
                            REQUIRE(is_erased == true);

                            // assert not found
                            is_found = btree.search(target, found);
                            REQUIRE(is_found == false);
                            REQUIRE(found == nullptr);

                            // expected output
                            outs =
                                "          |||\n"
                                "     |55|\n"
                                "          |||\n"
                                "|45|\n"
                                "          |||\n"
                                "     |30|\n"
                                "          |||\n";

                            // assert expected output
                            ss.str("");
                            ss << btree;
                            REQUIRE(ss.str() == outs);
                        }
                    }
                }
            }

            THEN("of invalid -1 will return false and search will fail") {
                target = -1;

                is_erased = btree.erase(target);
                REQUIRE(is_erased == false);

                is_found = btree.search(target, found);
                REQUIRE(is_found == false);
                REQUIRE(found == nullptr);
            }

            THEN("of invalid 0 will return false and search will fail") {
                target = 0;

                is_erased = btree.erase(target);
                REQUIRE(is_erased == false);

                is_found = btree.search(target, found);
                REQUIRE(is_found == false);
                REQUIRE(found == nullptr);
            }

            THEN("of invalid 100 will return false and search will fail") {
                target = 100;

                is_erased = btree.erase(target);
                REQUIRE(is_erased == false);

                is_found = btree.search(target, found);
                REQUIRE(is_found == false);
                REQUIRE(found == nullptr);
            }
        }

        WHEN("constructing btree2 with btree") {
            // copy constructor
            BST<int> btree2(btree);

            // expected output
            outs =
                "          |||\n"
                "     |70|\n"
                "                    |||\n"
                "               |60|\n"
                "                    |||\n"
                "          |55|\n"
                "               |||\n"
                "|50|\n"
                "               |||\n"
                "          |45|\n"
                "               |||\n"
                "     |40|\n"
                "               |||\n"
                "          |30|\n"
                "               |||\n";

            // assert expected output for btree2
            ss.str("");
            ss << btree2;
            REQUIRE(ss.str() == outs);

            THEN("changing btree2 will not affect btree") {
                btree2.erase(50);
                btree2.erase(70);
                btree2.erase(40);
                btree2.erase(60);

                // expected output
                outs =
                    "          |||\n"
                    "     |55|\n"
                    "          |||\n"
                    "|45|\n"
                    "          |||\n"
                    "     |30|\n"
                    "          |||\n";

                // assert expected output for btree2
                ss.str("");
                ss << btree2;
                REQUIRE(ss.str() == outs);

                // expected output
                outs =
                    "          |||\n"
                    "     |70|\n"
                    "                    |||\n"
                    "               |60|\n"
                    "                    |||\n"
                    "          |55|\n"
                    "               |||\n"
                    "|50|\n"
                    "               |||\n"
                    "          |45|\n"
                    "               |||\n"
                    "     |40|\n"
                    "               |||\n"
                    "          |30|\n"
                    "               |||\n";

                // assert expected output for btree
                ss.str("");
                ss << btree;
                REQUIRE(ss.str() == outs);

                THEN("assignment of btree with btree2") {
                    // copy assignment
                    btree = btree2;

                    // expected output
                    outs =
                        "          |||\n"
                        "     |55|\n"
                        "          |||\n"
                        "|45|\n"
                        "          |||\n"
                        "     |30|\n"
                        "          |||\n";

                    // assert expected output for btree
                    ss.str("");
                    ss << btree;
                    REQUIRE(ss.str() == outs);
                }
            }
        }
    }

    GIVEN("an array of 0 sorted item") {
        const int SIZE = 1;
        int arr[SIZE] = {1};

        WHEN("constructed to btree will give an empty btree") {
            const int SIZE_ZERO = 0;
            // constructor with array of sorted items
            BST<int> btree(arr, SIZE_ZERO);

            // assert expected output for btree
            ss.str("");
            ss << btree;
            REQUIRE(ss.str() == "|||\n");
        }
    }

    GIVEN("an array of 1 sorted item") {
        const int SIZE = 1;
        int arr[SIZE] = {1};

        WHEN("constructed to btree will give a full binary btree") {
            // constructor with array of sorted items
            BST<int> btree(arr, SIZE);

            // expected output
            outs =
                "     |||\n"
                "|1|\n"
                "     |||\n";

            // assert expected output for btree
            ss.str("");
            ss << btree;
            REQUIRE(ss.str() == outs);
        }
    }

    GIVEN("an array of 2 sorted item") {
        const int SIZE = 2;
        int arr[SIZE] = {1, 2};

        WHEN("constructed to btree will give a complete binary btree") {
            // constructor with array of sorted items
            BST<int> btree(arr, SIZE);

            // expected output
            outs =
                "     |||\n"
                "|2|\n"
                "          |||\n"
                "     |1|\n"
                "          |||\n";

            // assert expected output for btree
            ss.str("");
            ss << btree;
            REQUIRE(ss.str() == outs);
        }
    }

    GIVEN("an array of 3 sorted item") {
        const int SIZE = 3;
        int arr[SIZE] = {1, 2, 3};

        WHEN("constructed to btree will give a full binary btree") {
            // constructor with array of sorted items
            BST<int> btree(arr, SIZE);

            // expected output
            outs =
                "          |||\n"
                "     |3|\n"
                "          |||\n"
                "|2|\n"
                "          |||\n"
                "     |1|\n"
                "          |||\n";

            // assert expected output for btree
            ss.str("");
            ss << btree;
            REQUIRE(ss.str() == outs);
        }
    }

    GIVEN("an array of 4 sorted item") {
        const int SIZE = 4;
        int arr[SIZE] = {1, 2, 3, 4};

        WHEN("constructed to btree will give a complete binary btree") {
            // constructor with array of sorted items
            BST<int> btree(arr, SIZE);

            // expected output
            outs =
                "          |||\n"
                "     |4|\n"
                "          |||\n"
                "|3|\n"
                "          |||\n"
                "     |2|\n"
                "               |||\n"
                "          |1|\n"
                "               |||\n";

            // assert expected output for btree
            ss.str("");
            ss << btree;
            REQUIRE(ss.str() == outs);
        }
    }

    GIVEN("an array of 5 sorted item") {
        const int SIZE = 5;
        int arr[SIZE] = {1, 2, 3, 4, 5};

        WHEN("constructed to btree will give a complete binary btree") {
            // constructor with array of sorted items
            BST<int> btree(arr, SIZE);

            // expected output
            outs =
                "          |||\n"
                "     |5|\n"
                "               |||\n"
                "          |4|\n"
                "               |||\n"
                "|3|\n"
                "          |||\n"
                "     |2|\n"
                "               |||\n"
                "          |1|\n"
                "               |||\n";

            // assert expected output for btree
            ss.str("");
            ss << btree;
            REQUIRE(ss.str() == outs);
        }
    }

    GIVEN("an array of 6 sorted item") {
        const int SIZE = 6;
        int arr[SIZE] = {1, 2, 3, 4, 5, 6};

        WHEN("constructed to btree will give a complete binary btree") {
            // constructor with array of sorted items
            BST<int> btree(arr, SIZE);

            // expected output
            outs =
                "          |||\n"
                "     |6|\n"
                "               |||\n"
                "          |5|\n"
                "               |||\n"
                "|4|\n"
                "               |||\n"
                "          |3|\n"
                "               |||\n"
                "     |2|\n"
                "               |||\n"
                "          |1|\n"
                "               |||\n";

            // assert expected output for btree
            ss.str("");
            ss << btree;
            REQUIRE(ss.str() == outs);
        }
    }

    GIVEN("two btree's") {
        const int SIZE_A = 7, SIZE_B = 3;
        int arr_a[SIZE_A] = {30, 40, 45, 50, 55, 60, 70},
            arr_b[SIZE_B] = {1, 2, 3};

        // constructor with array of sorted items
        BST<int> btree_a(arr_a, SIZE_A);
        BST<int> btree_b(arr_b, SIZE_B);

        // expected output for btree
        outs =
            "               |||\n"
            "          |70|\n"
            "               |||\n"
            "     |60|\n"
            "               |||\n"
            "          |55|\n"
            "               |||\n"
            "|50|\n"
            "               |||\n"
            "          |45|\n"
            "               |||\n"
            "     |40|\n"
            "               |||\n"
            "          |30|\n"
            "               |||\n";

        // assert expected output for btree
        ss.str("");
        ss << btree_a;
        REQUIRE(ss.str() == outs);

        // expected output
        outs =
            "          |||\n"
            "     |3|\n"
            "          |||\n"
            "|2|\n"
            "          |||\n"
            "     |1|\n"
            "          |||\n";

        // assert expected output for btree
        ss.str("");
        ss << btree_b;
        REQUIRE(ss.str() == outs);

        WHEN("appending btree_a with btree_b") {
            // compound addition operator of btree_a with btree_b
            btree_a += btree_b;

            THEN("btree_a will have btree_b added") {
                // expected output for btree
                outs =
                    "               |||\n"
                    "          |70|\n"
                    "               |||\n"
                    "     |60|\n"
                    "               |||\n"
                    "          |55|\n"
                    "               |||\n"
                    "|50|\n"
                    "               |||\n"
                    "          |45|\n"
                    "               |||\n"
                    "     |40|\n"
                    "               |||\n"
                    "          |30|\n"
                    "                         |||\n"
                    "                    |3|\n"
                    "                         |||\n"
                    "               |2|\n"
                    "                         |||\n"
                    "                    |1|\n"
                    "                         |||\n";

                // assert expected output for btree
                ss.str("");
                ss << btree_a;
                REQUIRE(ss.str() == outs);
            }
        }
    }
}
