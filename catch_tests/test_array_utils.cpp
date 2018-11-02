#include <algorithm>
#include <cstdlib>
#include "../include/array_utils.h"
#include "../lib/catch.hpp"

using namespace array_utils;

SCENARIO("Array Utilities", "[array_utils]") {
    GIVEN("index of max") {
        WHEN("array of size 1") {
            const int SIZE = 1;
            int array[SIZE] = {1};

            REQUIRE(index_of_max(array, SIZE) == 0);
        }

        WHEN("Array of size 2") {
            const int SIZE = 2;
            int array1[SIZE] = {1, 2};
            int array2[SIZE] = {2, 1};

            REQUIRE(index_of_max(array1, SIZE) == 1);
            REQUIRE(index_of_max(array2, SIZE) == 0);
        }

        WHEN("Array of size 3") {
            const int SIZE = 3;
            int array1[SIZE] = {1, 2, 3};
            int array2[SIZE] = {1, 3, 2};
            int array3[SIZE] = {3, 1, 2};
            int array4[SIZE] = {3, 3, 2};
            int array5[SIZE] = {1, 3, 3};

            REQUIRE(index_of_max(array1, SIZE) == 2);
            REQUIRE(index_of_max(array2, SIZE) == 1);
            REQUIRE(index_of_max(array3, SIZE) == 0);
            REQUIRE(index_of_max(array4, SIZE) == 0);
            REQUIRE(index_of_max(array5, SIZE) == 1);
        }
    }

    GIVEN("ordered insert") {
        //
    }

    GIVEN("first greater or equal") {
        WHEN("array of size 1") {
            const int SIZE = 1;
            int array[SIZE] = {1};
            int entry;

            entry = -1;
            REQUIRE(first_ge(array, SIZE, entry) == 0);

            entry = 1;
            REQUIRE(first_ge(array, SIZE, entry) == 0);

            entry = 2;
            REQUIRE(first_ge(array, SIZE, entry) == 1);
        }

        WHEN("array of size 2") {
            const int SIZE = 2;
            int array[SIZE] = {1, 3};
            int entry;

            entry = -1;
            REQUIRE(first_ge(array, SIZE, entry) == 0);

            entry = 1;
            REQUIRE(first_ge(array, SIZE, entry) == 0);

            entry = 2;
            REQUIRE(first_ge(array, SIZE, entry) == 1);

            entry = 3;
            REQUIRE(first_ge(array, SIZE, entry) == 1);

            entry = 4;
            REQUIRE(first_ge(array, SIZE, entry) == 2);
        }

        WHEN("array of size 3") {
            const int SIZE = 3;
            int array[SIZE] = {1, 3, 5};
            int entry;

            entry = -1;
            REQUIRE(first_ge(array, SIZE, entry) == 0);

            entry = 1;
            REQUIRE(first_ge(array, SIZE, entry) == 0);

            entry = 2;
            REQUIRE(first_ge(array, SIZE, entry) == 1);

            entry = 3;
            REQUIRE(first_ge(array, SIZE, entry) == 1);

            entry = 4;
            REQUIRE(first_ge(array, SIZE, entry) == 2);

            entry = 5;
            REQUIRE(first_ge(array, SIZE, entry) == 2);

            entry = 6;
            REQUIRE(first_ge(array, SIZE, entry) == 3);
        }
    }

    GIVEN("attach item") {
        WHEN("array of size 0") {
            const int SIZE = 1;
            int array[SIZE] = {-1};
            int entry;
            std::size_t array_size = 0;

            entry = 1;
            attach_item(array, array_size, entry);

            REQUIRE(array_size == 1);
            REQUIRE(array[0] == 1);
        }

        WHEN("array of size 1") {
            const int SIZE = 2;
            int array[SIZE] = {-1, -1};
            int entry;
            std::size_t array_size = 1;

            entry = 1;
            attach_item(array, array_size, entry);

            REQUIRE(array_size == 2);
            REQUIRE(array[0] == -1);
            REQUIRE(array[1] == 1);
        }

        WHEN("array of size 2") {
            const int SIZE = 3;
            int array[SIZE] = {-1, -1, -1};
            int entry;
            std::size_t array_size = 2;

            entry = 1;
            attach_item(array, array_size, entry);

            REQUIRE(array_size == 3);
            REQUIRE(array[0] == -1);
            REQUIRE(array[1] == -1);
            REQUIRE(array[2] == 1);
        }
    }

    GIVEN("insert item") {
        WHEN("array of size 0") {
            const int SIZE = 3;
            int array[SIZE] = {-1, -1, -1};
            int entry;
            std::size_t array_size = 0;

            entry = 1;
            insert_item(array, 0, array_size, entry);

            REQUIRE(array_size == 1);
            REQUIRE(array[0] == 1);
            REQUIRE(array[1] == -1);
            REQUIRE(array[2] == -1);
        }

        WHEN("array of size 1") {
            const int SIZE = 3;
            int array[SIZE] = {-1, -2, -3};
            int entry;
            std::size_t array_size = 1;

            entry = 1;
            insert_item(array, 0, array_size, entry);

            REQUIRE(array_size == 2);
            REQUIRE(array[0] == 1);
            REQUIRE(array[1] == -1);
            REQUIRE(array[2] == -3);
        }

        WHEN("array of size 2") {
            const int SIZE = 3;
            int array[SIZE] = {-1, -2, -3};
            int entry;
            std::size_t array_size = 2;

            THEN("@ pos 0") {
                entry = 1;
                insert_item(array, 0, array_size, entry);

                REQUIRE(array_size == 3);
                REQUIRE(array[0] == 1);
                REQUIRE(array[1] == -1);
                REQUIRE(array[2] == -2);
            }

            THEN("@ pos 1") {
                entry = 1;
                insert_item(array, 1, array_size, entry);

                REQUIRE(array_size == 3);
                REQUIRE(array[0] == -1);
                REQUIRE(array[1] == 1);
                REQUIRE(array[2] == -2);
            }
        }

        WHEN("array of size 3") {
            const int SIZE = 4;
            int array[SIZE] = {-1, -2, -3, -4};
            int entry;
            std::size_t array_size = 3;

            THEN("@ pos 0") {
                entry = 1;
                insert_item(array, 0, array_size, entry);

                REQUIRE(array_size == 4);
                REQUIRE(array[0] == 1);
                REQUIRE(array[1] == -1);
                REQUIRE(array[2] == -2);
                REQUIRE(array[3] == -3);
            }

            THEN("@ pos 1") {
                entry = 1;
                insert_item(array, 1, array_size, entry);

                REQUIRE(array_size == 4);
                REQUIRE(array[0] == -1);
                REQUIRE(array[1] == 1);
                REQUIRE(array[2] == -2);
                REQUIRE(array[3] == -3);
            }

            THEN("@ pos 2") {
                entry = 1;
                insert_item(array, 2, array_size, entry);

                REQUIRE(array_size == 4);
                REQUIRE(array[0] == -1);
                REQUIRE(array[1] == -2);
                REQUIRE(array[2] == 1);
                REQUIRE(array[3] == -3);
            }
        }
    }

    GIVEN("detach item") {
        WHEN("array of size 0") {
            const int SIZE = 3;
            int array[SIZE] = {1, 2, 3};
            int entry;
            std::size_t array_size = 0;

            entry = -1;
            detach_item(array, array_size, entry);

            // do nothing on size 0
            REQUIRE(array_size == 0);
            REQUIRE(entry == -1);
        }

        WHEN("array of size 1") {
            const int SIZE = 3;
            int array[SIZE] = {1, 2, 3};
            int entry;
            std::size_t array_size = 1;

            entry = -1;
            detach_item(array, array_size, entry);

            // do nothing on size 0
            REQUIRE(array_size == 0);
            REQUIRE(entry == 1);
        }

        WHEN("array of size 2") {
            const int SIZE = 3;
            int array[SIZE] = {1, 2, 3};
            int entry;
            std::size_t array_size = 2;

            entry = -1;
            detach_item(array, array_size, entry);

            // do nothing on size 0
            REQUIRE(array_size == 1);
            REQUIRE(entry == 2);
        }
    }

    GIVEN("delete item") {
        WHEN("array of size 0") {
            const int SIZE = 3;
            int array[SIZE] = {1, 2, 3};
            int entry;
            std::size_t array_size = 0;

            entry = -1;
            delete_item(array, 0, array_size, entry);

            // do nothing on size 0
            REQUIRE(array_size == 0);
            REQUIRE(entry == -1);
        }

        WHEN("array of size 1") {
            const int SIZE = 3;
            int array[SIZE] = {1, 2, 3};
            int entry;
            std::size_t array_size = 1;

            entry = -1;
            delete_item(array, 0, array_size, entry);

            REQUIRE(array_size == 0);
            REQUIRE(entry == 1);
            REQUIRE(array[0] == 1);
        }

        WHEN("array of size 2") {
            const int SIZE = 3;
            int array[SIZE] = {1, 2, 3};
            int entry;
            std::size_t array_size = 2;

            THEN("@ pos 0") {
                entry = -1;
                delete_item(array, 0, array_size, entry);

                REQUIRE(array_size == 1);
                REQUIRE(entry == 1);
                REQUIRE(array[0] == 2);
            }

            THEN("@ pos 1") {
                entry = -1;
                delete_item(array, 1, array_size, entry);

                REQUIRE(array_size == 1);
                REQUIRE(entry == 2);
                REQUIRE(array[0] == 1);
            }
        }

        WHEN("array of size 3") {
            const int SIZE = 3;
            int array[SIZE] = {1, 2, 3};
            int entry;
            std::size_t array_size = 3;

            THEN("@ pos 0") {
                entry = -1;
                delete_item(array, 0, array_size, entry);

                REQUIRE(array_size == 2);
                REQUIRE(entry == 1);
                REQUIRE(array[0] == 2);
                REQUIRE(array[1] == 3);
            }

            THEN("@ pos 1") {
                entry = -1;
                delete_item(array, 1, array_size, entry);

                REQUIRE(array_size == 2);
                REQUIRE(entry == 2);
                REQUIRE(array[0] == 1);
                REQUIRE(array[1] == 3);
            }

            THEN("@ pos 2") {
                entry = -1;
                delete_item(array, 2, array_size, entry);

                REQUIRE(array_size == 2);
                REQUIRE(entry == 3);
                REQUIRE(array[0] == 1);
                REQUIRE(array[1] == 2);
            }
        }
    }

    GIVEN("merge arrays") {}

    GIVEN("split array") {
        WHEN("dest array size 1") {
            const int SIZE = 10;
            int dest[SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
            std::size_t dest_size = 1;

            THEN("src array size 1") {
                int src[SIZE] = {100};
                std::size_t src_size = 1;

                merge(dest, dest_size, src, src_size);

                REQUIRE(src_size == 0);
                REQUIRE(dest_size == 2);
                REQUIRE(dest[0] == 1);
                REQUIRE(dest[1] == 100);
            }

            THEN("src array size 2") {
                int src[SIZE] = {100, 200};
                std::size_t src_size = 2;

                merge(dest, dest_size, src, src_size);

                REQUIRE(src_size == 0);
                REQUIRE(dest_size == 3);
                REQUIRE(dest[0] == 1);
                REQUIRE(dest[1] == 100);
                REQUIRE(dest[2] == 200);
            }
        }

        WHEN("dest array size 2") {
            const int SIZE = 10;
            int dest[SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
            std::size_t dest_size = 2;

            THEN("src array size 1") {
                int src[SIZE] = {100};
                std::size_t src_size = 1;

                merge(dest, dest_size, src, src_size);

                REQUIRE(src_size == 0);
                REQUIRE(dest_size == 3);
                REQUIRE(dest[0] == 1);
                REQUIRE(dest[1] == 2);
                REQUIRE(dest[2] == 100);
            }

            THEN("src array size 2") {
                int src[SIZE] = {100, 200};
                std::size_t src_size = 2;

                merge(dest, dest_size, src, src_size);

                REQUIRE(src_size == 0);
                REQUIRE(dest_size == 4);
                REQUIRE(dest[0] == 1);
                REQUIRE(dest[1] == 2);
                REQUIRE(dest[2] == 100);
                REQUIRE(dest[3] == 200);
            }

            THEN("src array size 3") {
                int src[SIZE] = {100, 200, 300};
                std::size_t src_size = 3;

                merge(dest, dest_size, src, src_size);

                REQUIRE(src_size == 0);
                REQUIRE(dest_size == 5);
                REQUIRE(dest[0] == 1);
                REQUIRE(dest[1] == 2);
                REQUIRE(dest[2] == 100);
                REQUIRE(dest[3] == 200);
                REQUIRE(dest[4] == 300);
            }
        }
    }

    GIVEN("copy array") {
        WHEN("array is size 1") {
            const int SIZE = 1;
            int src[SIZE] = {1};
            int dest[SIZE] = {-1};
            std::size_t src_size = SIZE;
            std::size_t dest_size = 0;

            copy_array(src, src_size, dest, dest_size);

            REQUIRE(src_size == SIZE);
            REQUIRE(src[0] == 1);

            REQUIRE(dest_size == SIZE);
            REQUIRE(dest[0] == 1);
        }

        WHEN("array is size 2") {
            const int SIZE = 2;
            int src[SIZE] = {1, 2};
            int dest[SIZE] = {-1, -1};
            std::size_t src_size = SIZE;
            std::size_t dest_size = 0;

            copy_array(src, src_size, dest, dest_size);

            REQUIRE(src_size == SIZE);
            REQUIRE(src[0] == 1);
            REQUIRE(src[1] == 2);

            REQUIRE(dest_size == SIZE);
            REQUIRE(dest[0] == 1);
            REQUIRE(dest[1] == 2);
        }

        WHEN("array is size 3") {
            const int SIZE = 3;
            int src[SIZE] = {1, 2, 3};
            int dest[SIZE] = {-1, -1, -1};
            std::size_t src_size = SIZE;
            std::size_t dest_size = 0;

            copy_array(src, src_size, dest, dest_size);

            REQUIRE(src_size == SIZE);
            REQUIRE(src[0] == 1);
            REQUIRE(src[1] == 2);
            REQUIRE(src[2] == 3);

            REQUIRE(dest_size == SIZE);
            REQUIRE(dest[0] == 1);
            REQUIRE(dest[1] == 2);
            REQUIRE(dest[2] == 3);
        }

        WHEN("array is size 4") {
            const int SIZE = 4;
            int src[SIZE] = {1, 2, 3, 4};
            int dest[SIZE] = {-1, -1, -1, -1};
            std::size_t src_size = SIZE;
            std::size_t dest_size = 0;

            copy_array(src, src_size, dest, dest_size);

            REQUIRE(src_size == SIZE);
            REQUIRE(src[0] == 1);
            REQUIRE(src[1] == 2);
            REQUIRE(src[2] == 3);
            REQUIRE(src[3] == 4);

            REQUIRE(dest_size == SIZE);
            REQUIRE(dest[0] == 1);
            REQUIRE(dest[1] == 2);
            REQUIRE(dest[2] == 3);
            REQUIRE(dest[3] == 4);
        }
    }
}