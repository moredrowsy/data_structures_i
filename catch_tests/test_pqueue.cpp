#include <string>
#include "../include/pqueue.h"
#include "../lib/catch.hpp"

SCENARIO("Priority Queue", "[pqueue]") {
    using namespace pqueue;

    Info<std::string> info;
    std::string item;
    PQueue<std::string> pqueue;

    GIVEN("CTOR w/ Info item") {
        PQueue<std::string> pq_item(Info<std::string>("Ares", 1));

        REQUIRE(pq_item.empty() == false);
        REQUIRE(pq_item.size() == 1);
        REQUIRE(pq_item.root() != nullptr);
        REQUIRE(pq_item.root()[0]._item == "Ares");
    }

    GIVEN("CTOR w/ Info list") {
        // prepare list
        const int SIZE = 8;
        std::string str_list[SIZE] = {"Ares", "Boreas", "Cronus", "Dionysus",
                                      "Eos",  "Freya",  "Gaia",   "Hermes"};
        int priority[SIZE] = {1, 2, 3, 4, 5, 6, 7, 8};
        Info<std::string> ascending_list[SIZE];

        // create Info list
        for(unsigned i = 0; i < SIZE; ++i)
            ascending_list[i] = Info<std::string>(str_list[i], priority[i]);

        PQueue<std::string> pq_list(ascending_list, SIZE);

        REQUIRE(pq_list.empty() == false);
        REQUIRE(pq_list.size() == 8);
        REQUIRE(pq_list.root() != nullptr);
        REQUIRE(pq_list.root()[0]._item == "Hermes");
        REQUIRE(pq_list.root()[1]._item == "Gaia");
        REQUIRE(pq_list.root()[2]._item == "Freya");
        REQUIRE(pq_list.root()[3]._item == "Dionysus");
        REQUIRE(pq_list.root()[4]._item == "Cronus");
        REQUIRE(pq_list.root()[5]._item == "Boreas");
        REQUIRE(pq_list.root()[6]._item == "Eos");
        REQUIRE(pq_list.root()[7]._item == "Ares");
    }
}
