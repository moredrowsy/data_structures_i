#include <algorithm>          // std::sort()
#include <array>              // std::array
#include <cstdlib>            // rand(), srand()
#include <ctime>              // time()
#include "../include/heap.h"  // Heap Class
#include "../lib/catch.hpp"   // catch2 test framework

SCENARIO("Binary Heap Tree", "[heap]") {
    using namespace heap;

    bool is_inserted = false;
    Heap<int> heap;
    int item = -1000;

    GIVEN("empty heap") {
        THEN("empty() = true, size = 0, cap = 0") {
            REQUIRE(heap.empty() == true);
            REQUIRE(heap.size() == 0);
            REQUIRE(heap.capacity() == 0);
            REQUIRE(heap.items() == nullptr);
        }

        WHEN("insertion: w/ ascending items") {
            is_inserted = heap.insert(0);
            REQUIRE(is_inserted == true);
            REQUIRE(heap.empty() == false);
            REQUIRE(heap.size() == 1);
            REQUIRE(heap.capacity() == 1);
            REQUIRE(heap.items() != nullptr);
            REQUIRE(heap.items()[0] == 0);

            is_inserted = heap.insert(1);
            REQUIRE(is_inserted == true);
            REQUIRE(heap.empty() == false);
            REQUIRE(heap.size() == 2);
            REQUIRE(heap.capacity() == 2);
            REQUIRE(heap.items() != nullptr);
            REQUIRE(heap.items()[0] == 1);
            REQUIRE(heap.items()[1] == 0);

            is_inserted = heap.insert(2);
            REQUIRE(is_inserted == true);
            REQUIRE(heap.empty() == false);
            REQUIRE(heap.size() == 3);
            REQUIRE(heap.capacity() == 4);
            REQUIRE(heap.items() != nullptr);
            REQUIRE(heap.items()[0] == 2);
            REQUIRE(heap.items()[1] == 0);
            REQUIRE(heap.items()[2] == 1);

            is_inserted = heap.insert(3);
            REQUIRE(is_inserted == true);
            REQUIRE(heap.empty() == false);
            REQUIRE(heap.size() == 4);
            REQUIRE(heap.capacity() == 4);
            REQUIRE(heap.items() != nullptr);
            REQUIRE(heap.items()[0] == 3);
            REQUIRE(heap.items()[1] == 2);
            REQUIRE(heap.items()[2] == 1);
            REQUIRE(heap.items()[3] == 0);

            is_inserted = heap.insert(4);
            REQUIRE(is_inserted == true);
            REQUIRE(heap.empty() == false);
            REQUIRE(heap.size() == 5);
            REQUIRE(heap.capacity() == 8);
            REQUIRE(heap.items() != nullptr);
            REQUIRE(heap.items()[0] == 4);
            REQUIRE(heap.items()[1] == 3);
            REQUIRE(heap.items()[2] == 1);
            REQUIRE(heap.items()[3] == 0);
            REQUIRE(heap.items()[4] == 2);

            is_inserted = heap.insert(5);
            REQUIRE(is_inserted == true);
            REQUIRE(heap.empty() == false);
            REQUIRE(heap.size() == 6);
            REQUIRE(heap.capacity() == 8);
            REQUIRE(heap.items() != nullptr);
            REQUIRE(heap.items()[0] == 5);
            REQUIRE(heap.items()[1] == 3);
            REQUIRE(heap.items()[2] == 4);
            REQUIRE(heap.items()[3] == 0);
            REQUIRE(heap.items()[4] == 2);
            REQUIRE(heap.items()[5] == 1);

            is_inserted = heap.insert(6);
            REQUIRE(is_inserted == true);
            REQUIRE(heap.empty() == false);
            REQUIRE(heap.size() == 7);
            REQUIRE(heap.capacity() == 8);
            REQUIRE(heap.items() != nullptr);
            REQUIRE(heap.items()[0] == 6);
            REQUIRE(heap.items()[1] == 3);
            REQUIRE(heap.items()[2] == 5);
            REQUIRE(heap.items()[3] == 0);
            REQUIRE(heap.items()[4] == 2);
            REQUIRE(heap.items()[5] == 1);
            REQUIRE(heap.items()[6] == 4);

            is_inserted = heap.insert(7);
            REQUIRE(is_inserted == true);
            REQUIRE(heap.empty() == false);
            REQUIRE(heap.size() == 8);
            REQUIRE(heap.capacity() == 8);
            REQUIRE(heap.items() != nullptr);
            REQUIRE(heap.items()[0] == 7);
            REQUIRE(heap.items()[1] == 6);
            REQUIRE(heap.items()[2] == 5);
            REQUIRE(heap.items()[3] == 3);
            REQUIRE(heap.items()[4] == 2);
            REQUIRE(heap.items()[5] == 1);
            REQUIRE(heap.items()[6] == 4);
            REQUIRE(heap.items()[7] == 0);

            is_inserted = heap.insert(7);
            REQUIRE(is_inserted == true);
            REQUIRE(heap.empty() == false);
            REQUIRE(heap.size() == 9);
            REQUIRE(heap.capacity() == 16);
            REQUIRE(heap.items() != nullptr);
            REQUIRE(heap.items()[0] == 7);
            REQUIRE(heap.items()[1] == 7);
            REQUIRE(heap.items()[2] == 5);
            REQUIRE(heap.items()[3] == 6);
            REQUIRE(heap.items()[4] == 2);
            REQUIRE(heap.items()[5] == 1);
            REQUIRE(heap.items()[6] == 4);
            REQUIRE(heap.items()[7] == 0);
            REQUIRE(heap.items()[8] == 3);

            // validate heap structure
            REQUIRE(heap.validate() == true);

            THEN("popping each item") {
                item = heap.pop();
                REQUIRE(item == 7);
                REQUIRE(heap.empty() == false);
                REQUIRE(heap.size() == 8);
                REQUIRE(heap.capacity() == 16);
                REQUIRE(heap.items() != nullptr);
                REQUIRE(heap.items()[0] == 7);
                REQUIRE(heap.items()[1] == 6);
                REQUIRE(heap.items()[2] == 5);
                REQUIRE(heap.items()[3] == 3);
                REQUIRE(heap.items()[4] == 2);
                REQUIRE(heap.items()[5] == 1);
                REQUIRE(heap.items()[6] == 4);
                REQUIRE(heap.items()[7] == 0);

                item = heap.pop();
                REQUIRE(item == 7);
                REQUIRE(heap.empty() == false);
                REQUIRE(heap.size() == 7);
                REQUIRE(heap.capacity() == 16);
                REQUIRE(heap.items() != nullptr);
                REQUIRE(heap.items()[0] == 6);
                REQUIRE(heap.items()[1] == 3);
                REQUIRE(heap.items()[2] == 5);
                REQUIRE(heap.items()[3] == 0);
                REQUIRE(heap.items()[4] == 2);
                REQUIRE(heap.items()[5] == 1);
                REQUIRE(heap.items()[6] == 4);

                item = heap.pop();
                REQUIRE(item == 6);
                REQUIRE(heap.empty() == false);
                REQUIRE(heap.size() == 6);
                REQUIRE(heap.capacity() == 16);
                REQUIRE(heap.items() != nullptr);
                REQUIRE(heap.items()[0] == 5);
                REQUIRE(heap.items()[1] == 3);
                REQUIRE(heap.items()[2] == 4);
                REQUIRE(heap.items()[3] == 0);
                REQUIRE(heap.items()[4] == 2);
                REQUIRE(heap.items()[5] == 1);

                item = heap.pop();
                REQUIRE(item == 5);
                REQUIRE(heap.empty() == false);
                REQUIRE(heap.size() == 5);
                REQUIRE(heap.capacity() == 16);
                REQUIRE(heap.items() != nullptr);
                REQUIRE(heap.items()[0] == 4);
                REQUIRE(heap.items()[1] == 3);
                REQUIRE(heap.items()[2] == 1);
                REQUIRE(heap.items()[3] == 0);
                REQUIRE(heap.items()[4] == 2);

                item = heap.pop();
                REQUIRE(item == 4);
                REQUIRE(heap.empty() == false);
                REQUIRE(heap.size() == 4);
                REQUIRE(heap.capacity() == 16);
                REQUIRE(heap.items() != nullptr);
                REQUIRE(heap.items()[0] == 3);
                REQUIRE(heap.items()[1] == 2);
                REQUIRE(heap.items()[2] == 1);
                REQUIRE(heap.items()[3] == 0);

                item = heap.pop();
                REQUIRE(item == 3);
                REQUIRE(heap.empty() == false);
                REQUIRE(heap.size() == 3);
                REQUIRE(heap.capacity() == 16);
                REQUIRE(heap.items() != nullptr);
                REQUIRE(heap.items()[0] == 2);
                REQUIRE(heap.items()[1] == 0);
                REQUIRE(heap.items()[2] == 1);

                item = heap.pop();
                REQUIRE(item == 2);
                REQUIRE(heap.empty() == false);
                REQUIRE(heap.size() == 2);
                REQUIRE(heap.capacity() == 16);
                REQUIRE(heap.items() != nullptr);
                REQUIRE(heap.items()[0] == 1);
                REQUIRE(heap.items()[1] == 0);

                item = heap.pop();
                REQUIRE(item == 1);
                REQUIRE(heap.empty() == false);
                REQUIRE(heap.size() == 1);
                REQUIRE(heap.capacity() == 16);
                REQUIRE(heap.items() != nullptr);
                REQUIRE(heap.items()[0] == 0);

                item = heap.pop();
                REQUIRE(item == 0);
                REQUIRE(heap.empty() == true);
                REQUIRE(heap.size() == 0);
                REQUIRE(heap.capacity() == 16);
                REQUIRE(heap.items() != nullptr);
            }

            THEN("copy CTOR: heap_copy has same items as heap") {
                Heap<int> heap_copy(heap);
                REQUIRE(heap_copy.empty() == false);

                const int *root = heap.items(), *root_copy = heap_copy.items();
                REQUIRE(root != root_copy);

                for(unsigned i = 0; i < heap.size(); ++i)
                    REQUIRE(root_copy[i] == root[i]);

                THEN("modifying heap_copy does not affect heap") {
                    heap_copy.clear();
                    REQUIRE(heap_copy.empty() == true);
                    REQUIRE(heap_copy.size() == 0);
                    REQUIRE(heap_copy.capacity() == 16);
                    REQUIRE(heap_copy.items() != nullptr);

                    // assert heap still have original values
                    REQUIRE(heap.empty() == false);
                    REQUIRE(heap.size() == 9);
                    REQUIRE(heap.capacity() == 16);
                    REQUIRE(heap.items() != nullptr);
                    REQUIRE(heap.items()[0] == 7);
                    REQUIRE(heap.items()[1] == 7);
                    REQUIRE(heap.items()[2] == 5);
                    REQUIRE(heap.items()[3] == 6);
                    REQUIRE(heap.items()[4] == 2);
                    REQUIRE(heap.items()[5] == 1);
                    REQUIRE(heap.items()[6] == 4);
                    REQUIRE(heap.items()[7] == 0);
                    REQUIRE(heap.items()[8] == 3);
                }
            }

            THEN("assignment op: heap_assign has same items as heap") {
                Heap<int> heap_assign;
                REQUIRE(heap_assign.empty() == true);

                heap_assign = heap;
                REQUIRE(heap_assign.empty() == false);

                const int *root = heap.items(),
                          *root_assign = heap_assign.items();

                for(unsigned i = 0; i < heap.size(); ++i)
                    REQUIRE(root_assign[i] == root[i]);

                THEN("modifying heap_assign does not affect heap") {
                    heap_assign.clear();
                    REQUIRE(heap_assign.empty() == true);
                    REQUIRE(heap_assign.size() == 0);
                    REQUIRE(heap_assign.capacity() == 16);
                    REQUIRE(heap_assign.items() != nullptr);

                    // assert heap still have original values
                    REQUIRE(heap.empty() == false);
                    REQUIRE(heap.size() == 9);
                    REQUIRE(heap.capacity() == 16);
                    REQUIRE(heap.items() != nullptr);
                    REQUIRE(heap.items()[0] == 7);
                    REQUIRE(heap.items()[1] == 7);
                    REQUIRE(heap.items()[2] == 5);
                    REQUIRE(heap.items()[3] == 6);
                    REQUIRE(heap.items()[4] == 2);
                    REQUIRE(heap.items()[5] == 1);
                    REQUIRE(heap.items()[6] == 4);
                    REQUIRE(heap.items()[7] == 0);
                    REQUIRE(heap.items()[8] == 3);
                }
            }
        }

        WHEN("insertion: w/ descending items") {
            is_inserted = heap.insert(7);
            REQUIRE(is_inserted == true);
            REQUIRE(heap.empty() == false);
            REQUIRE(heap.size() == 1);
            REQUIRE(heap.capacity() == 1);
            REQUIRE(heap.items() != nullptr);
            REQUIRE(heap.items()[0] == 7);

            is_inserted = heap.insert(6);
            REQUIRE(is_inserted == true);
            REQUIRE(heap.empty() == false);
            REQUIRE(heap.size() == 2);
            REQUIRE(heap.capacity() == 2);
            REQUIRE(heap.items() != nullptr);
            REQUIRE(heap.items()[0] == 7);
            REQUIRE(heap.items()[1] == 6);

            is_inserted = heap.insert(5);
            REQUIRE(is_inserted == true);
            REQUIRE(heap.empty() == false);
            REQUIRE(heap.size() == 3);
            REQUIRE(heap.capacity() == 4);
            REQUIRE(heap.items() != nullptr);
            REQUIRE(heap.items()[0] == 7);
            REQUIRE(heap.items()[1] == 6);
            REQUIRE(heap.items()[2] == 5);

            is_inserted = heap.insert(4);
            REQUIRE(is_inserted == true);
            REQUIRE(heap.empty() == false);
            REQUIRE(heap.size() == 4);
            REQUIRE(heap.capacity() == 4);
            REQUIRE(heap.items() != nullptr);
            REQUIRE(heap.items()[0] == 7);
            REQUIRE(heap.items()[1] == 6);
            REQUIRE(heap.items()[2] == 5);
            REQUIRE(heap.items()[3] == 4);

            is_inserted = heap.insert(3);
            REQUIRE(is_inserted == true);
            REQUIRE(heap.empty() == false);
            REQUIRE(heap.size() == 5);
            REQUIRE(heap.capacity() == 8);
            REQUIRE(heap.items() != nullptr);
            REQUIRE(heap.items()[0] == 7);
            REQUIRE(heap.items()[1] == 6);
            REQUIRE(heap.items()[2] == 5);
            REQUIRE(heap.items()[3] == 4);
            REQUIRE(heap.items()[4] == 3);

            is_inserted = heap.insert(2);
            REQUIRE(is_inserted == true);
            REQUIRE(heap.empty() == false);
            REQUIRE(heap.size() == 6);
            REQUIRE(heap.capacity() == 8);
            REQUIRE(heap.items() != nullptr);
            REQUIRE(heap.items()[0] == 7);
            REQUIRE(heap.items()[1] == 6);
            REQUIRE(heap.items()[2] == 5);
            REQUIRE(heap.items()[3] == 4);
            REQUIRE(heap.items()[4] == 3);
            REQUIRE(heap.items()[5] == 2);

            is_inserted = heap.insert(1);
            REQUIRE(is_inserted == true);
            REQUIRE(heap.empty() == false);
            REQUIRE(heap.size() == 7);
            REQUIRE(heap.capacity() == 8);
            REQUIRE(heap.items() != nullptr);
            REQUIRE(heap.items() != nullptr);
            REQUIRE(heap.items()[0] == 7);
            REQUIRE(heap.items()[1] == 6);
            REQUIRE(heap.items()[2] == 5);
            REQUIRE(heap.items()[3] == 4);
            REQUIRE(heap.items()[4] == 3);
            REQUIRE(heap.items()[5] == 2);
            REQUIRE(heap.items()[6] == 1);

            is_inserted = heap.insert(0);
            REQUIRE(is_inserted == true);
            REQUIRE(heap.empty() == false);
            REQUIRE(heap.size() == 8);
            REQUIRE(heap.capacity() == 8);
            REQUIRE(heap.items() != nullptr);
            REQUIRE(heap.items()[0] == 7);
            REQUIRE(heap.items()[1] == 6);
            REQUIRE(heap.items()[2] == 5);
            REQUIRE(heap.items()[3] == 4);
            REQUIRE(heap.items()[4] == 3);
            REQUIRE(heap.items()[5] == 2);
            REQUIRE(heap.items()[6] == 1);
            REQUIRE(heap.items()[7] == 0);
        }

        WHEN("insertion: w/ ascending list") {
            const int SIZE = 8;
            int list[SIZE] = {0, 1, 2, 3, 4, 5, 6, 7};
            Heap<int> heap_from_list(list, SIZE);

            REQUIRE(heap_from_list.empty() == false);
            REQUIRE(heap_from_list.size() == 8);
            REQUIRE(heap_from_list.capacity() == 8);
            REQUIRE(heap_from_list.items() != nullptr);
            REQUIRE(heap_from_list.items()[0] == 7);
            REQUIRE(heap_from_list.items()[1] == 6);
            REQUIRE(heap_from_list.items()[2] == 5);
            REQUIRE(heap_from_list.items()[3] == 3);
            REQUIRE(heap_from_list.items()[4] == 2);
            REQUIRE(heap_from_list.items()[5] == 1);
            REQUIRE(heap_from_list.items()[6] == 4);
            REQUIRE(heap_from_list.items()[7] == 0);
        }

        WHEN("insertion: w/ random items") {
            bool is_inserted = false;
            int item = 0;
            const int SIZE = 100;
            std::array<int, SIZE> random_items;
            srand(time(nullptr));

            // repeat ENTIRE inserts/pops of 100 random items for 100 iterations
            for(int i = 0; i < 100; ++i) {
                // populate random_items with random numbers
                for(auto &a : random_items) a = rand() % 100;

                // add random items to heap
                for(unsigned i = 0; i < SIZE; ++i) {
                    // two arguments insert
                    is_inserted = heap.insert(random_items[i]);
                    REQUIRE(is_inserted == true);
                    REQUIRE(heap.empty() == false);
                    REQUIRE(heap.size() == i + 1);
                    REQUIRE(heap.validate() == true);
                }

                // sort random_items to prepare for pop assertions
                std::sort(random_items.begin(), random_items.end());

                // assert popping will return max item
                for(unsigned i = SIZE; i > 0; --i) {
                    item = heap.pop();
                    REQUIRE(heap.size() == i - 1);
                    REQUIRE(item == random_items[i - 1]);
                    REQUIRE(heap.validate() == true);
                }

                heap.clear();
                REQUIRE(heap.size() == 0);
            }
        }
    }
}
