#include <cstdlib>  // srand(), rand()
#include <ctime>    // std::time
#include <vector>   // std::vector
#include "../include/chained_avl_hash.h"
#include "../include/chained_list_hash.h"
#include "../include/double_hash.h"
#include "../include/open_hash.h"
#include "../include/record.h"
#include "../lib/catch.hpp"

SCENARIO("Open Hash", "[open_hash]") {
    using Record = record::Record<int>;
    using OpenHash = open_hash::OpenHash<Record>;
    using DoubleHash = double_hash::DoubleHash<Record>;
    using ChainedAVLHash = chained_avl_hash::ChainedAVLHash<Record>;
    using ChainedListHash = chained_list_hash::ChainedListHash<Record>;

    bool is_found = false;
    const int MAX_RANGE = 10001, sample_size = 1000,
              iterations = sample_size / 2;
    OpenHash* ohash = new OpenHash[sample_size];
    DoubleHash* dhash = new DoubleHash[sample_size];
    ChainedAVLHash* avl_hash = new ChainedAVLHash[sample_size];
    ChainedListHash* cl_hash = new ChainedListHash[sample_size];
    std::vector<Record>* list = new std::vector<Record>[sample_size];
    Record result;

    // add random values to array of vector of Records
    for(unsigned i = 0; i < sample_size; ++i) {
        for(unsigned j = 0; j < iterations; ++j)
            list[i].push_back(Record(rand() % MAX_RANGE, rand() % MAX_RANGE));

        assert(list[i].size() == iterations);
    }

    GIVEN("Hash objects") {
        // inserts and search of inserted
        for(unsigned i = 0; i < sample_size; ++i) {
            // ensure all inserted items are found
            for(auto& a : list[i]) {
                ohash[i].insert(a);
                dhash[i].insert(a);
                avl_hash[i].insert(a);
                cl_hash[i].insert(a);
                is_found = false;
                is_found = ohash[i].find(a._key, result);
                REQUIRE(is_found == true);
                REQUIRE(a == result);

                is_found = false;
                is_found = dhash[i].find(a._key, result);
                REQUIRE(is_found == true);
                REQUIRE(a == result);

                is_found = false;
                is_found = avl_hash[i].find(a._key, result);
                REQUIRE(is_found == true);
                REQUIRE(a == result);

                is_found = false;
                is_found = cl_hash[i].find(a._key, result);
                REQUIRE(is_found == true);
                REQUIRE(a == result);
            }
        }

        // search invalid keys
        for(unsigned i = 0; i < sample_size; ++i) {
            // ensure that all searches are FALSE
            for(int j = -100; j > -350; --j) {
                is_found = false;
                is_found = ohash[i].find(j, result);
                REQUIRE(is_found == false);

                is_found = false;
                is_found = dhash[i].find(j, result);
                REQUIRE(is_found == false);

                is_found = false;
                is_found = avl_hash[i].find(j, result);
                REQUIRE(is_found == false);

                is_found = false;
                is_found = cl_hash[i].find(j, result);
                REQUIRE(is_found == false);
            }
        }

        // remove inserted keys
        for(unsigned i = 0; i < sample_size; ++i) {
            // ensure removed keys are not found
            for(auto& a : list[i]) {
                ohash[i].remove(a._key);
                is_found = ohash[i].find(a._key, result);
                REQUIRE(is_found == false);

                dhash[i].remove(a._key);
                is_found = dhash[i].find(a._key, result);
                REQUIRE(is_found == false);

                avl_hash[i].remove(a._key);
                is_found = avl_hash[i].find(a._key, result);
                REQUIRE(is_found == false);

                cl_hash[i].remove(a._key);
                is_found = cl_hash[i].find(a._key, result);
                REQUIRE(is_found == false);
            }
        }
    }

    delete[] ohash;
    delete[] dhash;
    delete[] avl_hash;
    delete[] cl_hash;
    delete[] list;
}