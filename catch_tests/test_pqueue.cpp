#include <algorithm>            // std::shuffle()
#include <array>                // std::array
#include <ctime>                // time()
#include <initializer_list>     // initializer list object for CTOR
#include <random>               // std::default_random_engine()
#include <string>               // string objects
#include "../include/pqueue.h"  // Info struct and PQueue class
#include "../lib/catch.hpp"     // catch2 test framework

SCENARIO("Priority Queue", "[pqueue]") {
    using namespace pqueue;

    bool is_inserted = false;
    Info<std::string> info;
    std::string item;
    PQueue<std::string> pqueue;

    // prepare list
    const int SIZE = 20;
    std::string items[SIZE] = {
        "Ares",     "Ares", "Boreas",  "Cronus",     "Dionysus",
        "Eos",      "Eos",  "Freya",   "Gaia",       "Hermes",
        "Hermes",   "Iris", "Jupiter", "Kappa",      "Lupe",
        "Morpheus", "Nike", "Oceanus", "Persephone", "Persephone"};
    int priorities[SIZE] = {1, 1, 2,  3,  4,  5,  5,  6,  7,  8,
                            8, 9, 10, 11, 12, 13, 14, 15, 16, 16};

    GIVEN("insertion: w/ ascending items") {
        for(unsigned i = 0; i < SIZE; ++i) {
            // two arguments insert
            is_inserted = pqueue.insert(items[i], priorities[i]);
            REQUIRE(is_inserted == true);
            REQUIRE(pqueue.empty() == false);
            REQUIRE(pqueue.size() == i + 1);
            REQUIRE(pqueue.validate() == true);
        }

        THEN("pop will return the highest priorities item") {
            for(unsigned i = SIZE; i > 0; --i) {
                item = pqueue.pop();
                REQUIRE(pqueue.size() == i - 1);
                REQUIRE(item == items[i - 1]);
                REQUIRE(pqueue.validate() == true);
            }
        }
    }

    GIVEN("insertion: w/ descending items") {
        for(unsigned i = SIZE; i > 0; --i) {
            // one argument insert
            is_inserted = pqueue.insert(
                Info<std::string>(items[i - 1], priorities[i - 1]));
            REQUIRE(is_inserted == true);
            REQUIRE(pqueue.empty() == false);
            REQUIRE(pqueue.size() == SIZE - i + 1);
            REQUIRE(pqueue.validate() == true);
        }

        THEN("pop will return the highest priorities item") {
            for(unsigned i = SIZE; i > 0; --i) {
                item = pqueue.pop();
                REQUIRE(pqueue.size() == i - 1);
                REQUIRE(item == items[i - 1]);
                REQUIRE(pqueue.validate() == true);
            }
        }
    }

    GIVEN("insertion: w/ random items") {
        std::array<Info<std::string>, SIZE> random_items;
        Info<std::string> info;

        // populate random_items
        for(unsigned i = 0; i < SIZE; ++i) {
            info._item = items[i];
            info._priority = priorities[i];
            random_items[i] = info;
        }

        // repeat ENTIRE inserts/pops of random items for 100 iterations
        for(int i = 0; i < 100; ++i) {
            // shuffle random_items
            std::shuffle(random_items.begin(), random_items.end(),
                         std::default_random_engine(time(nullptr)));

            // insert every item in random_items
            for(unsigned i = 0; i < SIZE; ++i) {
                is_inserted = pqueue.insert(random_items[i]);
                REQUIRE(is_inserted == true);
                REQUIRE(pqueue.empty() == false);
                REQUIRE(pqueue.size() == i + 1);
                REQUIRE(pqueue.validate() == true);
            }

            // assert pop will return the highest priorities item
            for(unsigned i = SIZE; i > 0; --i) {
                item = pqueue.pop();
                REQUIRE(pqueue.size() == i - 1);
                REQUIRE(item == items[i - 1]);
                REQUIRE(pqueue.validate() == true);
            }

            pqueue.clear();
            REQUIRE(pqueue.size() == 0);
        }
    }

    GIVEN("CTOR w/ Info item") {
        // one argument constructor
        PQueue<std::string> pq_item(Info<std::string>(items[0], priorities[0]));

        REQUIRE(pq_item.empty() == false);
        REQUIRE(pq_item.size() == 1);
        REQUIRE(pq_item.validate() == true);

        THEN("pop will return the highest priorities item") {
            item = pq_item.pop();
            REQUIRE(pq_item.size() == 0);
            REQUIRE(item == items[0]);
            REQUIRE(pq_item.validate() == true);
        }
    }

    GIVEN("CTOR w/ T item and int priorities") {
        // two arguments constructor
        PQueue<std::string> pq_item(items[0], priorities[0]);

        REQUIRE(pq_item.empty() == false);
        REQUIRE(pq_item.size() == 1);
        REQUIRE(pq_item.validate() == true);

        THEN("pop will return the highest priorities item") {
            item = pq_item.pop();
            REQUIRE(pq_item.size() == 0);
            REQUIRE(item == items[0]);
            REQUIRE(pq_item.validate() == true);
        }
    }

    GIVEN("CTOR w/ Info list") {
        Info<std::string> ascending_list[SIZE];

        // create Info list
        for(unsigned i = 0; i < SIZE; ++i)
            ascending_list[i] = Info<std::string>(items[i], priorities[i]);

        // list constructor
        PQueue<std::string> pq_list(ascending_list, SIZE);

        REQUIRE(pq_list.empty() == false);
        REQUIRE(pq_list.size() == SIZE);
        REQUIRE(pq_list.validate() == true);

        THEN("pop will return the highest priorities item") {
            for(unsigned i = SIZE; i > 0; --i) {
                item = pq_list.pop();
                REQUIRE(pq_list.size() == i - 1);
                REQUIRE(item == items[i - 1]);
                REQUIRE(pq_list.validate() == true);
            }
        }
    }

    GIVEN("CTOR: w/ initializer list") {
        typedef Info<std::string> Info_s;

        PQueue<std::string> pqueue_list = {Info_s("One", 1), Info_s("Two", 2),
                                           Info_s("Three", 3),
                                           Info_s("Four", 4)};

        REQUIRE(pqueue_list.empty() == false);
        REQUIRE(pqueue_list.size() == 4);
        REQUIRE(pqueue_list.validate() == true);
    }
}
