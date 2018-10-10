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
    const int SIZE = 8;
    std::string str_list[SIZE] = {"Ares", "Boreas", "Cronus", "Dionysus",
                                  "Eos",  "Freya",  "Gaia",   "Hermes"};
    int priority[SIZE] = {1, 2, 3, 4, 5, 6, 7, 8};

    GIVEN("inserted w/ ascending items") {
        is_inserted = pqueue.insert(str_list[0], priority[0]);
        REQUIRE(is_inserted == true);
        REQUIRE(pqueue.empty() == false);
        REQUIRE(pqueue.size() == 1);
        REQUIRE(pqueue.validate() == true);

        is_inserted = pqueue.insert(str_list[1], priority[1]);
        REQUIRE(is_inserted == true);
        REQUIRE(pqueue.empty() == false);
        REQUIRE(pqueue.size() == 2);
        REQUIRE(pqueue.validate() == true);

        is_inserted = pqueue.insert(str_list[2], priority[2]);
        REQUIRE(is_inserted == true);
        REQUIRE(pqueue.empty() == false);
        REQUIRE(pqueue.size() == 3);
        REQUIRE(pqueue.validate() == true);

        is_inserted = pqueue.insert(str_list[3], priority[3]);
        REQUIRE(is_inserted == true);
        REQUIRE(pqueue.empty() == false);
        REQUIRE(pqueue.size() == 4);
        REQUIRE(pqueue.validate() == true);

        is_inserted = pqueue.insert(str_list[4], priority[4]);
        REQUIRE(is_inserted == true);
        REQUIRE(pqueue.empty() == false);
        REQUIRE(pqueue.size() == 5);
        REQUIRE(pqueue.validate() == true);

        is_inserted = pqueue.insert(str_list[5], priority[5]);
        REQUIRE(is_inserted == true);
        REQUIRE(pqueue.empty() == false);
        REQUIRE(pqueue.size() == 6);
        REQUIRE(pqueue.validate() == true);

        is_inserted = pqueue.insert(str_list[6], priority[6]);
        REQUIRE(is_inserted == true);
        REQUIRE(pqueue.empty() == false);
        REQUIRE(pqueue.size() == 7);
        REQUIRE(pqueue.validate() == true);

        is_inserted = pqueue.insert(str_list[7], priority[7]);
        REQUIRE(is_inserted == true);
        REQUIRE(pqueue.empty() == false);
        REQUIRE(pqueue.size() == 8);
        REQUIRE(pqueue.validate() == true);

        THEN("pop will return the highest priority item") {
            for(unsigned i = SIZE; i > 0; --i) {
                item = pqueue.pop();
                REQUIRE(item == str_list[i - 1]);
                REQUIRE(pqueue.validate() == true);
            }
        }
    }

    GIVEN("inserted w/ descending items") {
        is_inserted =
            pqueue.insert(Info<std::string>(str_list[7], priority[7]));
        REQUIRE(is_inserted == true);
        REQUIRE(pqueue.empty() == false);
        REQUIRE(pqueue.size() == 1);
        REQUIRE(pqueue.validate() == true);

        is_inserted =
            pqueue.insert(Info<std::string>(str_list[6], priority[6]));
        REQUIRE(is_inserted == true);
        REQUIRE(pqueue.empty() == false);
        REQUIRE(pqueue.size() == 2);
        REQUIRE(pqueue.validate() == true);

        is_inserted =
            pqueue.insert(Info<std::string>(str_list[5], priority[5]));
        REQUIRE(is_inserted == true);
        REQUIRE(pqueue.empty() == false);
        REQUIRE(pqueue.size() == 3);
        REQUIRE(pqueue.validate() == true);

        is_inserted =
            pqueue.insert(Info<std::string>(str_list[4], priority[4]));
        REQUIRE(is_inserted == true);
        REQUIRE(pqueue.empty() == false);
        REQUIRE(pqueue.size() == 4);
        REQUIRE(pqueue.validate() == true);

        is_inserted =
            pqueue.insert(Info<std::string>(str_list[3], priority[3]));
        REQUIRE(is_inserted == true);
        REQUIRE(pqueue.empty() == false);
        REQUIRE(pqueue.size() == 5);
        REQUIRE(pqueue.validate() == true);

        is_inserted =
            pqueue.insert(Info<std::string>(str_list[2], priority[2]));
        REQUIRE(is_inserted == true);
        REQUIRE(pqueue.empty() == false);
        REQUIRE(pqueue.size() == 6);
        REQUIRE(pqueue.validate() == true);

        is_inserted =
            pqueue.insert(Info<std::string>(str_list[1], priority[1]));
        REQUIRE(is_inserted == true);
        REQUIRE(pqueue.empty() == false);
        REQUIRE(pqueue.size() == 7);
        REQUIRE(pqueue.validate() == true);

        is_inserted =
            pqueue.insert(Info<std::string>(str_list[0], priority[0]));
        REQUIRE(is_inserted == true);
        REQUIRE(pqueue.empty() == false);
        REQUIRE(pqueue.size() == 8);
        REQUIRE(pqueue.validate() == true);

        THEN("pop will return the highest priority item") {
            for(unsigned i = SIZE; i > 0; --i) {
                item = pqueue.pop();
                REQUIRE(item == str_list[i - 1]);
                REQUIRE(pqueue.validate() == true);
            }
        }
    }

    GIVEN("CTOR w/ Info item") {
        PQueue<std::string> pq_item(
            Info<std::string>(str_list[0], priority[0]));

        REQUIRE(pq_item.empty() == false);
        REQUIRE(pq_item.size() == 1);
        REQUIRE(pq_item.validate() == true);

        THEN("pop will return the highest priority item") {
            item = pq_item.pop();
            REQUIRE(item == str_list[0]);
            REQUIRE(pq_item.validate() == true);
        }
    }

    GIVEN("CTOR w/ Info list") {
        Info<std::string> ascending_list[SIZE];

        // create Info list
        for(unsigned i = 0; i < SIZE; ++i)
            ascending_list[i] = Info<std::string>(str_list[i], priority[i]);

        PQueue<std::string> pq_list(ascending_list, SIZE);

        REQUIRE(pq_list.empty() == false);
        REQUIRE(pq_list.size() == 8);
        REQUIRE(pq_list.validate() == true);

        THEN("pop will return the highest priority item") {
            for(unsigned i = SIZE; i > 0; --i) {
                item = pq_list.pop();
                REQUIRE(item == str_list[i - 1]);
                REQUIRE(pq_list.validate() == true);
            }
        }
    }
}
