#include <algorithm>
#include <cstdlib>
#include "../include/array_utils.h"
#include "../lib/catch.hpp"

using namespace array_utils;

SCENARIO("Array Utilities", "[array_utils]") {
    GIVEN("Array of size 1") {
        const int SIZE = 1;
        int array[SIZE] = {1};

        REQUIRE(index_of_max(array, SIZE) == 0);
    }

    GIVEN("Array of size 2") {
        const int SIZE = 2;
        int array1[SIZE] = {1, 2};
        int array2[SIZE] = {2, 1};

        REQUIRE(index_of_max(array1, SIZE) == 1);
        REQUIRE(index_of_max(array2, SIZE) == 0);
    }

    GIVEN("Array of size 1") {
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

    GIVEN("split array") {
        WHEN("array size 2") {
            const int SIZE = 1;
            int src[SIZE] = {1};
            int dest[SIZE] = {-1};
            std::size_t src_size = SIZE;
            std::size_t dest_size = 0;

            split(src, src_size, dest, dest_size);

            REQUIRE(src_size == 1);
            REQUIRE(src[0] == 1);

            REQUIRE(dest_size == 0);
        }

        WHEN("array size 2") {
            const int SIZE = 2;
            int src[SIZE] = {1, 2};
            int dest[SIZE] = {-1, -1};
            std::size_t src_size = SIZE;
            std::size_t dest_size = 0;

            split(src, src_size, dest, dest_size);

            REQUIRE(src_size == 1);
            REQUIRE(src[0] == 1);

            REQUIRE(dest_size == 1);
            REQUIRE(dest[0] == 2);
        }

        WHEN("array size 3") {
            const int SIZE = 3;
            int src[SIZE] = {1, 2, 3};
            int dest[SIZE] = {-1, -1, -1};
            std::size_t src_size = SIZE;
            std::size_t dest_size = 0;

            split(src, src_size, dest, dest_size);

            REQUIRE(src_size == 2);
            REQUIRE(src[0] == 1);
            REQUIRE(src[1] == 2);

            REQUIRE(dest_size == 1);
            REQUIRE(dest[0] == 3);
        }

        WHEN("array size 4") {
            const int SIZE = 4;
            int src[SIZE] = {1, 2, 3, 4};
            int dest[SIZE] = {-1, -1, -1, -1};
            std::size_t src_size = SIZE;
            std::size_t dest_size = 0;

            split(src, src_size, dest, dest_size);

            REQUIRE(src_size == 2);
            REQUIRE(src[0] == 1);
            REQUIRE(src[1] == 2);

            REQUIRE(dest_size == 2);
            REQUIRE(dest[0] == 3);
            REQUIRE(dest[1] == 4);
        }

        WHEN("array size 5") {
            const int SIZE = 5;
            int src[SIZE] = {1, 2, 3, 4, 5};
            int dest[SIZE] = {-1, -1, -1, -1, -1};
            std::size_t src_size = SIZE;
            std::size_t dest_size = 0;

            split(src, src_size, dest, dest_size);

            REQUIRE(src_size == 3);
            REQUIRE(src[0] == 1);
            REQUIRE(src[1] == 2);
            REQUIRE(src[2] == 3);

            REQUIRE(dest_size == 2);
            REQUIRE(dest[0] == 4);
            REQUIRE(dest[1] == 5);
        }
    }
}
