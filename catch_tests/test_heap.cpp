#include <iostream>
#include "../include/heap.h"
#include "../lib/catch.hpp"

SCENARIO("Binary Heap Tree", "[heap]") {
    using namespace heap;

    Heap<int> heap;
    int item = -1000;

    GIVEN("empty heap") {
        THEN("empty() = true, size = 0, cap = 0") {
            REQUIRE(heap.empty() == true);
            REQUIRE(heap.size() == 0);
            REQUIRE(heap.capacity() == 0);
            REQUIRE(heap.root() == nullptr);
        }

        WHEN("inserted w/ ascending items") {
            heap.insert(0);
            REQUIRE(heap.empty() == false);
            REQUIRE(heap.size() == 1);
            REQUIRE(heap.capacity() == 1);
            REQUIRE(heap.root() != nullptr);
            REQUIRE(heap.root()[0] == 0);

            heap.insert(1);
            REQUIRE(heap.empty() == false);
            REQUIRE(heap.size() == 2);
            REQUIRE(heap.capacity() == 2);
            REQUIRE(heap.root() != nullptr);
            REQUIRE(heap.root()[0] == 1);
            REQUIRE(heap.root()[1] == 0);

            heap.insert(2);
            REQUIRE(heap.empty() == false);
            REQUIRE(heap.size() == 3);
            REQUIRE(heap.capacity() == 4);
            REQUIRE(heap.root() != nullptr);
            REQUIRE(heap.root()[0] == 2);
            REQUIRE(heap.root()[1] == 0);
            REQUIRE(heap.root()[2] == 1);

            heap.insert(3);
            REQUIRE(heap.empty() == false);
            REQUIRE(heap.size() == 4);
            REQUIRE(heap.capacity() == 4);
            REQUIRE(heap.root() != nullptr);
            REQUIRE(heap.root()[0] == 3);
            REQUIRE(heap.root()[1] == 2);
            REQUIRE(heap.root()[2] == 1);
            REQUIRE(heap.root()[3] == 0);

            heap.insert(4);
            REQUIRE(heap.empty() == false);
            REQUIRE(heap.size() == 5);
            REQUIRE(heap.capacity() == 8);
            REQUIRE(heap.root() != nullptr);
            REQUIRE(heap.root()[0] == 4);
            REQUIRE(heap.root()[1] == 3);
            REQUIRE(heap.root()[2] == 1);
            REQUIRE(heap.root()[3] == 0);
            REQUIRE(heap.root()[4] == 2);

            heap.insert(5);
            REQUIRE(heap.empty() == false);
            REQUIRE(heap.size() == 6);
            REQUIRE(heap.capacity() == 8);
            REQUIRE(heap.root() != nullptr);
            REQUIRE(heap.root()[0] == 5);
            REQUIRE(heap.root()[1] == 3);
            REQUIRE(heap.root()[2] == 4);
            REQUIRE(heap.root()[3] == 0);
            REQUIRE(heap.root()[4] == 2);
            REQUIRE(heap.root()[5] == 1);

            heap.insert(6);
            REQUIRE(heap.empty() == false);
            REQUIRE(heap.size() == 7);
            REQUIRE(heap.capacity() == 8);
            REQUIRE(heap.root() != nullptr);
            REQUIRE(heap.root()[0] == 6);
            REQUIRE(heap.root()[1] == 3);
            REQUIRE(heap.root()[2] == 5);
            REQUIRE(heap.root()[3] == 0);
            REQUIRE(heap.root()[4] == 2);
            REQUIRE(heap.root()[5] == 1);
            REQUIRE(heap.root()[6] == 4);

            heap.insert(7);
            REQUIRE(heap.empty() == false);
            REQUIRE(heap.size() == 8);
            REQUIRE(heap.capacity() == 8);
            REQUIRE(heap.root() != nullptr);
            REQUIRE(heap.root()[0] == 7);
            REQUIRE(heap.root()[1] == 6);
            REQUIRE(heap.root()[2] == 5);
            REQUIRE(heap.root()[3] == 3);
            REQUIRE(heap.root()[4] == 2);
            REQUIRE(heap.root()[5] == 1);
            REQUIRE(heap.root()[6] == 4);
            REQUIRE(heap.root()[7] == 0);

            THEN("popping each item") {
                item = heap.pop();
                REQUIRE(item == 7);
                REQUIRE(heap.empty() == false);
                REQUIRE(heap.size() == 7);
                REQUIRE(heap.capacity() == 8);
                REQUIRE(heap.root() != nullptr);
                REQUIRE(heap.root()[0] == 6);
                REQUIRE(heap.root()[1] == 3);
                REQUIRE(heap.root()[2] == 5);
                REQUIRE(heap.root()[3] == 0);
                REQUIRE(heap.root()[4] == 2);
                REQUIRE(heap.root()[5] == 1);
                REQUIRE(heap.root()[6] == 4);

                item = heap.pop();
                REQUIRE(item == 6);
                REQUIRE(heap.empty() == false);
                REQUIRE(heap.size() == 6);
                REQUIRE(heap.capacity() == 8);
                REQUIRE(heap.root() != nullptr);
                REQUIRE(heap.root()[0] == 5);
                REQUIRE(heap.root()[1] == 3);
                REQUIRE(heap.root()[2] == 4);
                REQUIRE(heap.root()[3] == 0);
                REQUIRE(heap.root()[4] == 2);
                REQUIRE(heap.root()[5] == 1);

                item = heap.pop();
                REQUIRE(item == 5);
                REQUIRE(heap.empty() == false);
                REQUIRE(heap.size() == 5);
                REQUIRE(heap.capacity() == 8);
                REQUIRE(heap.root() != nullptr);
                REQUIRE(heap.root()[0] == 4);
                REQUIRE(heap.root()[1] == 3);
                REQUIRE(heap.root()[2] == 1);
                REQUIRE(heap.root()[3] == 0);
                REQUIRE(heap.root()[4] == 2);

                item = heap.pop();
                REQUIRE(item == 4);
                REQUIRE(heap.empty() == false);
                REQUIRE(heap.size() == 4);
                REQUIRE(heap.capacity() == 8);
                REQUIRE(heap.root() != nullptr);
                REQUIRE(heap.root()[0] == 3);
                REQUIRE(heap.root()[1] == 2);
                REQUIRE(heap.root()[2] == 1);
                REQUIRE(heap.root()[3] == 0);

                item = heap.pop();
                REQUIRE(item == 3);
                REQUIRE(heap.empty() == false);
                REQUIRE(heap.size() == 3);
                REQUIRE(heap.capacity() == 8);
                REQUIRE(heap.root() != nullptr);
                REQUIRE(heap.root()[0] == 2);
                REQUIRE(heap.root()[1] == 0);
                REQUIRE(heap.root()[2] == 1);

                item = heap.pop();
                REQUIRE(item == 2);
                REQUIRE(heap.empty() == false);
                REQUIRE(heap.size() == 2);
                REQUIRE(heap.capacity() == 8);
                REQUIRE(heap.root() != nullptr);
                REQUIRE(heap.root()[0] == 1);
                REQUIRE(heap.root()[1] == 0);

                item = heap.pop();
                REQUIRE(item == 1);
                REQUIRE(heap.empty() == false);
                REQUIRE(heap.size() == 1);
                REQUIRE(heap.capacity() == 8);
                REQUIRE(heap.root() != nullptr);
                REQUIRE(heap.root()[0] == 0);

                item = heap.pop();
                REQUIRE(item == 0);
                REQUIRE(heap.empty() == true);
                REQUIRE(heap.size() == 0);
                REQUIRE(heap.capacity() == 8);
                REQUIRE(heap.root() != nullptr);
            }

            THEN("copy CTOR: heap_copy has same items as heap") {
                Heap<int> heap_copy(heap);
                REQUIRE(heap_copy.empty() == false);

                const int *root = heap.root(), *root_copy = heap_copy.root();
                REQUIRE(root != root_copy);

                for(unsigned i = 0; i < heap.size(); ++i)
                    REQUIRE(root_copy[i] == root[i]);

                THEN("modifying heap_copy does not affect heap") {
                    heap_copy.clear();
                    REQUIRE(heap_copy.empty() == true);
                    REQUIRE(heap_copy.size() == 0);
                    REQUIRE(heap_copy.capacity() == 8);
                    REQUIRE(heap_copy.root() != nullptr);

                    // assert heap still have original values
                    REQUIRE(heap.empty() == false);
                    REQUIRE(heap.size() == 8);
                    REQUIRE(heap.capacity() == 8);
                    REQUIRE(heap.root() != nullptr);
                    REQUIRE(heap.root()[0] == 7);
                    REQUIRE(heap.root()[1] == 6);
                    REQUIRE(heap.root()[2] == 5);
                    REQUIRE(heap.root()[3] == 3);
                    REQUIRE(heap.root()[4] == 2);
                    REQUIRE(heap.root()[5] == 1);
                    REQUIRE(heap.root()[6] == 4);
                    REQUIRE(heap.root()[7] == 0);
                }
            }

            THEN("assignment op: heap_assign has same items as heap") {
                Heap<int> heap_assign;
                REQUIRE(heap_assign.empty() == true);

                heap_assign = heap;
                REQUIRE(heap_assign.empty() == false);

                const int *root = heap.root(),
                          *root_assign = heap_assign.root();

                for(unsigned i = 0; i < heap.size(); ++i)
                    REQUIRE(root_assign[i] == root[i]);

                THEN("modifying heap_assign does not affect heap") {
                    heap_assign.clear();
                    REQUIRE(heap_assign.empty() == true);
                    REQUIRE(heap_assign.size() == 0);
                    REQUIRE(heap_assign.capacity() == 8);
                    REQUIRE(heap_assign.root() != nullptr);

                    // assert heap still have original values
                    REQUIRE(heap.empty() == false);
                    REQUIRE(heap.size() == 8);
                    REQUIRE(heap.capacity() == 8);
                    REQUIRE(heap.root() != nullptr);
                    REQUIRE(heap.root()[0] == 7);
                    REQUIRE(heap.root()[1] == 6);
                    REQUIRE(heap.root()[2] == 5);
                    REQUIRE(heap.root()[3] == 3);
                    REQUIRE(heap.root()[4] == 2);
                    REQUIRE(heap.root()[5] == 1);
                    REQUIRE(heap.root()[6] == 4);
                    REQUIRE(heap.root()[7] == 0);
                }
            }
        }

        WHEN("inserted w/ descending items") {
            heap.insert(7);
            REQUIRE(heap.empty() == false);
            REQUIRE(heap.size() == 1);
            REQUIRE(heap.capacity() == 1);
            REQUIRE(heap.root() != nullptr);
            REQUIRE(heap.root()[0] == 7);

            heap.insert(6);
            REQUIRE(heap.empty() == false);
            REQUIRE(heap.size() == 2);
            REQUIRE(heap.capacity() == 2);
            REQUIRE(heap.root() != nullptr);
            REQUIRE(heap.root()[0] == 7);
            REQUIRE(heap.root()[1] == 6);

            heap.insert(5);
            REQUIRE(heap.empty() == false);
            REQUIRE(heap.size() == 3);
            REQUIRE(heap.capacity() == 4);
            REQUIRE(heap.root() != nullptr);
            REQUIRE(heap.root()[0] == 7);
            REQUIRE(heap.root()[1] == 6);
            REQUIRE(heap.root()[2] == 5);

            heap.insert(4);
            REQUIRE(heap.empty() == false);
            REQUIRE(heap.size() == 4);
            REQUIRE(heap.capacity() == 4);
            REQUIRE(heap.root() != nullptr);
            REQUIRE(heap.root()[0] == 7);
            REQUIRE(heap.root()[1] == 6);
            REQUIRE(heap.root()[2] == 5);
            REQUIRE(heap.root()[3] == 4);

            heap.insert(3);
            REQUIRE(heap.empty() == false);
            REQUIRE(heap.size() == 5);
            REQUIRE(heap.capacity() == 8);
            REQUIRE(heap.root() != nullptr);
            REQUIRE(heap.root()[0] == 7);
            REQUIRE(heap.root()[1] == 6);
            REQUIRE(heap.root()[2] == 5);
            REQUIRE(heap.root()[3] == 4);
            REQUIRE(heap.root()[4] == 3);

            heap.insert(2);
            REQUIRE(heap.empty() == false);
            REQUIRE(heap.size() == 6);
            REQUIRE(heap.capacity() == 8);
            REQUIRE(heap.root() != nullptr);
            REQUIRE(heap.root()[0] == 7);
            REQUIRE(heap.root()[1] == 6);
            REQUIRE(heap.root()[2] == 5);
            REQUIRE(heap.root()[3] == 4);
            REQUIRE(heap.root()[4] == 3);
            REQUIRE(heap.root()[5] == 2);

            heap.insert(1);
            REQUIRE(heap.empty() == false);
            REQUIRE(heap.size() == 7);
            REQUIRE(heap.capacity() == 8);
            REQUIRE(heap.root() != nullptr);
            REQUIRE(heap.root() != nullptr);
            REQUIRE(heap.root()[0] == 7);
            REQUIRE(heap.root()[1] == 6);
            REQUIRE(heap.root()[2] == 5);
            REQUIRE(heap.root()[3] == 4);
            REQUIRE(heap.root()[4] == 3);
            REQUIRE(heap.root()[5] == 2);
            REQUIRE(heap.root()[6] == 1);

            heap.insert(0);
            REQUIRE(heap.empty() == false);
            REQUIRE(heap.size() == 8);
            REQUIRE(heap.capacity() == 8);
            REQUIRE(heap.root() != nullptr);
            REQUIRE(heap.root()[0] == 7);
            REQUIRE(heap.root()[1] == 6);
            REQUIRE(heap.root()[2] == 5);
            REQUIRE(heap.root()[3] == 4);
            REQUIRE(heap.root()[4] == 3);
            REQUIRE(heap.root()[5] == 2);
            REQUIRE(heap.root()[6] == 1);
            REQUIRE(heap.root()[7] == 0);
        }

        WHEN("inserted w/ ascending list") {
            const int SIZE = 8;
            int list[SIZE] = {0, 1, 2, 3, 4, 5, 6, 7};
            Heap<int> heap_from_list(list, SIZE);

            REQUIRE(heap_from_list.empty() == false);
            REQUIRE(heap_from_list.size() == 8);
            REQUIRE(heap_from_list.capacity() == 8);
            REQUIRE(heap_from_list.root() != nullptr);
            REQUIRE(heap_from_list.root()[0] == 7);
            REQUIRE(heap_from_list.root()[1] == 6);
            REQUIRE(heap_from_list.root()[2] == 5);
            REQUIRE(heap_from_list.root()[3] == 3);
            REQUIRE(heap_from_list.root()[4] == 2);
            REQUIRE(heap_from_list.root()[5] == 1);
            REQUIRE(heap_from_list.root()[6] == 4);
            REQUIRE(heap_from_list.root()[7] == 0);
        }
    }
}
