#include <algorithm>
#include <array>
#include <ctime>
#include <random>
#include <string>
#include "../include/pqueue.h"
#include "../lib/catch.hpp"

SCENARIO("Priority Queue", "[pqueue]") {
    using namespace pqueue;

    bool is_inserted = false;
    Info<std::string> info;
    std::string item;
    PQueue<std::string> pqueue;

    // prepare list
    const int SIZE = 11;
    std::string names[SIZE] = {"Ares",     "Ares",   "Boreas", "Cronus",
                               "Dionysus", "Eos",    "Eos",    "Freya",
                               "Gaia",     "Hermes", "Hermes"};
    int priority[SIZE] = {1, 1, 2, 3, 4, 5, 5, 6, 7, 8, 8};

    GIVEN("insertion: w/ ascending items") {
        for(unsigned i = 0; i < SIZE; ++i) {
            // two arguments insert
            is_inserted = pqueue.insert(names[i], priority[i]);
            REQUIRE(is_inserted == true);
            REQUIRE(pqueue.empty() == false);
            REQUIRE(pqueue.size() == i + 1);
            REQUIRE(pqueue.validate() == true);
        }

        THEN("pop will return the highest priority item") {
            for(unsigned i = SIZE; i > 0; --i) {
                item = pqueue.pop();
                REQUIRE(pqueue.size() == i - 1);
                REQUIRE(item == names[i - 1]);
                REQUIRE(pqueue.validate() == true);
            }
        }
    }

    GIVEN("insertion: w/ descending items") {
        for(unsigned i = SIZE; i > 0; --i) {
            // one argument insert
            is_inserted =
                pqueue.insert(Info<std::string>(names[i - 1], priority[i - 1]));
            REQUIRE(is_inserted == true);
            REQUIRE(pqueue.empty() == false);
            REQUIRE(pqueue.size() == SIZE - i + 1);
            REQUIRE(pqueue.validate() == true);
        }

        THEN("pop will return the highest priority item") {
            for(unsigned i = SIZE; i > 0; --i) {
                item = pqueue.pop();
                REQUIRE(pqueue.size() == i - 1);
                REQUIRE(item == names[i - 1]);
                REQUIRE(pqueue.validate() == true);
            }
        }
    }

    GIVEN("insertion: w/ random items") {
        std::array<Info<std::string>, SIZE> random_items;
        Info<std::string> info;

        // populate random_items
        for(unsigned i = 0; i < SIZE; ++i) {
            info._item = names[i];
            info._priority = priority[i];
            random_items[i] = info;
        }

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

        THEN("pop will return the highest priority item") {
            for(unsigned i = SIZE; i > 0; --i) {
                item = pqueue.pop();
                REQUIRE(pqueue.size() == i - 1);
                REQUIRE(item == names[i - 1]);
                REQUIRE(pqueue.validate() == true);
            }
        }
    }

    GIVEN("CTOR w/ Info item") {
        // one argument constructor
        PQueue<std::string> pq_item(Info<std::string>(names[0], priority[0]));

        REQUIRE(pq_item.empty() == false);
        REQUIRE(pq_item.size() == 1);
        REQUIRE(pq_item.validate() == true);

        THEN("pop will return the highest priority item") {
            item = pq_item.pop();
            REQUIRE(pq_item.size() == 0);
            REQUIRE(item == names[0]);
            REQUIRE(pq_item.validate() == true);
        }
    }

    GIVEN("CTOR w/ T item and int priority") {
        // two arguments constructor
        PQueue<std::string> pq_item(names[0], priority[0]);

        REQUIRE(pq_item.empty() == false);
        REQUIRE(pq_item.size() == 1);
        REQUIRE(pq_item.validate() == true);

        THEN("pop will return the highest priority item") {
            item = pq_item.pop();
            REQUIRE(pq_item.size() == 0);
            REQUIRE(item == names[0]);
            REQUIRE(pq_item.validate() == true);
        }
    }

    GIVEN("CTOR w/ Info list") {
        Info<std::string> ascending_list[SIZE];

        // create Info list
        for(unsigned i = 0; i < SIZE; ++i)
            ascending_list[i] = Info<std::string>(names[i], priority[i]);

        // list constructor
        PQueue<std::string> pq_list(ascending_list, SIZE);

        REQUIRE(pq_list.empty() == false);
        REQUIRE(pq_list.size() == SIZE);
        REQUIRE(pq_list.validate() == true);

        THEN("pop will return the highest priority item") {
            for(unsigned i = SIZE; i > 0; --i) {
                item = pq_list.pop();
                REQUIRE(pq_list.size() == i - 1);
                REQUIRE(item == names[i - 1]);
                REQUIRE(pq_list.validate() == true);
            }
        }
    }
}
