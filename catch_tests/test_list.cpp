#include <sstream>
#include "../include/list.h"
#include "../lib/catch.hpp"

TEST_CASE("my custom list", "[list]") {
    using namespace list;

    // test empty;
    List<int> list;
    REQUIRE(list.empty() == true);

    List<int>::Iterator begin = nullptr, end = nullptr, last_node = nullptr,
                        search = nullptr;

    // push 0 to 2
    list.push_back(0);
    REQUIRE(list[0] == 0);

    list.push_back(1);
    REQUIRE(list[0] == 0);
    REQUIRE(list[1] == 1);

    list.push_back(2);
    REQUIRE(list[0] == 0);
    REQUIRE(list[1] == 1);
    REQUIRE(list[2] == 2);

    REQUIRE(list.empty() == false);

    // assert iterators
    begin = list.begin();
    REQUIRE((bool)begin == true);
    REQUIRE(*begin == 0);

    end = list.end();
    REQUIRE((bool)end == false);
    REQUIRE(end == nullptr);

    last_node = list.last_node();
    REQUIRE((bool)last_node == true);
    REQUIRE(*last_node == 2);

    WHEN("push_front()") {
        List<int> list1;

        REQUIRE(list1.empty() == true);

        list1.push_front(0);
        REQUIRE(list1[0] == 0);
        REQUIRE(list1.front() == 0);
        REQUIRE(list1.back() == 0);

        list1.push_front(1);
        REQUIRE(list1[0] == 1);
        REQUIRE(list1[1] == 0);
        REQUIRE(list1.front() == 1);
        REQUIRE(list1.back() == 0);

        list1.push_front(2);
        REQUIRE(list1[0] == 2);
        REQUIRE(list1[1] == 1);
        REQUIRE(list1[2] == 0);
        REQUIRE(list1.front() == 2);
        REQUIRE(list1.back() == 0);
    }

    WHEN("insert_head") {
        List<int> list1;
        List<int>::Iterator it = nullptr;

        REQUIRE(list1.empty() == true);

        it = list1.insert_head(0);
        REQUIRE(list1[0] == 0);
        REQUIRE(list1.front() == 0);
        REQUIRE(list1.back() == 0);
        REQUIRE(*it == 0);

        it = list1.insert_head(1);
        REQUIRE(list1[0] == 1);
        REQUIRE(list1[1] == 0);
        REQUIRE(list1.front() == 1);
        REQUIRE(list1.back() == 0);
        REQUIRE(*it == 1);

        it = list1.insert_head(2);
        REQUIRE(list1[0] == 2);
        REQUIRE(list1[1] == 1);
        REQUIRE(list1[2] == 0);
        REQUIRE(list1.front() == 2);
        REQUIRE(list1.back() == 0);
        REQUIRE(*it == 2);
    }

    WHEN("insert_after on non-empty list1") {
        List<int> list1;
        List<int>::Iterator it = list1.begin();
        REQUIRE(list1.empty() == true);

        list1.insert_after(0, it);
        REQUIRE(list1[0] == 0);
        REQUIRE(list1.front() == 0);
        REQUIRE(list1.back() == 0);

        it = list1.begin();
        list1.insert_after(1, it);
        REQUIRE(list1[0] == 0);
        REQUIRE(list1[1] == 1);
        REQUIRE(list1.front() == 0);
        REQUIRE(list1.back() == 1);

        it = list1.last_node();
        list1.insert_after(2, it);
        REQUIRE(list1[0] == 0);
        REQUIRE(list1[1] == 1);
        REQUIRE(list1[2] == 2);
        REQUIRE(list1.front() == 0);
        REQUIRE(list1.back() == 2);

        it = list1.begin();

        ++it;
        list1.insert_after(3, it);
        REQUIRE(list1[0] == 0);
        REQUIRE(list1[1] == 1);
        REQUIRE(list1[2] == 3);
        REQUIRE(list1[3] == 2);
        REQUIRE(list1.front() == 0);
        REQUIRE(list1.back() == 2);
    }

    WHEN("insert_after on non-empty list1") {
        List<int> list1;
        List<int>::Iterator it = list1.begin();
        REQUIRE(list1.empty() == true);

        list1.insert_before(0, it);
        REQUIRE(list1[0] == 0);
        REQUIRE(list1.front() == 0);
        REQUIRE(list1.back() == 0);

        it = list1.begin();
        list1.insert_before(1, it);
        REQUIRE(list1[0] == 1);
        REQUIRE(list1[1] == 0);
        REQUIRE(list1.front() == 1);
        REQUIRE(list1.back() == 0);

        list1.insert_before(2, it);
        REQUIRE(list1[0] == 1);
        REQUIRE(list1[1] == 2);
        REQUIRE(list1[2] == 0);
        REQUIRE(list1.front() == 1);
        REQUIRE(list1.back() == 0);

        it = list1.begin();
        ++it;
        list1.insert_before(3, it);
        REQUIRE(list1[0] == 1);
        REQUIRE(list1[1] == 3);
        REQUIRE(list1[2] == 2);
        REQUIRE(list1[3] == 0);
        REQUIRE(list1.front() == 1);
        REQUIRE(list1.back() == 0);
    }

    WHEN("searching: return found Iterator") {
        search = list.search(0);
        REQUIRE(*search == 0);

        search = list.search(1);
        REQUIRE(*search == 1);

        search = list.search(2);
        REQUIRE(*search == 2);

        search = list.search(3);
        REQUIRE(search == nullptr);
    }

    WHEN("searching: return prev and found Iterator") {
        bool is_found = false;
        List<int>::Iterator prev, found;

        is_found = list.search(0, prev, found);
        REQUIRE(is_found == true);
        REQUIRE((bool)prev == false);
        REQUIRE(*found == 0);

        is_found = list.search(1, prev, found);
        REQUIRE(is_found == true);
        REQUIRE(*prev == 0);
        REQUIRE(*found == 1);

        is_found = list.search(2, prev, found);
        REQUIRE(is_found == true);
        REQUIRE(*prev == 1);
        REQUIRE(*found == 2);

        is_found = list.search(3, prev, found);
        REQUIRE(is_found == false);

        List<int> list1;

        is_found = list1.search(0, prev, found);
        REQUIRE(is_found == false);
    }

    GIVEN("remove: with T item") {
        bool is_removed = false;

        WHEN("first element") {
            is_removed = list.remove(0);
            REQUIRE(is_removed == true);
            REQUIRE(list[0] == 1);
            REQUIRE(list[1] == 2);
            REQUIRE(list.front() == 1);
            REQUIRE(list.back() == 2);
        }

        WHEN("second element") {
            is_removed = list.remove(1);
            REQUIRE(is_removed == true);
            REQUIRE(list[0] == 0);
            REQUIRE(list[1] == 2);
            REQUIRE(list.front() == 0);
            REQUIRE(list.back() == 2);
        }

        WHEN("last element") {
            is_removed = list.remove(2);
            REQUIRE(is_removed == true);
            REQUIRE(list[0] == 0);
            REQUIRE(list[1] == 1);
            REQUIRE(list.front() == 0);
            REQUIRE(list.back() == 1);
        }

        WHEN("invalid item") {
            is_removed = list.remove(-1);
            REQUIRE(is_removed == false);
            REQUIRE(list[0] == 0);
            REQUIRE(list[1] == 1);
            REQUIRE(list[2] == 2);
            REQUIRE(list.front() == 0);
            REQUIRE(list.back() == 2);
        }
    }

    GIVEN("remove: with Iterator") {
        int target = -1;
        List<int>::Iterator marker;

        WHEN("first element") {
            marker = list.begin();
            REQUIRE(*marker == 0);

            target = list.remove(marker);
            REQUIRE(target == 0);
            REQUIRE(list[0] == 1);
            REQUIRE(list[1] == 2);
            REQUIRE(list.front() == 1);
            REQUIRE(list.back() == 2);
        }

        WHEN("second element") {
            marker = list.begin();
            ++marker;
            REQUIRE(*marker == 1);

            target = list.remove(marker);
            REQUIRE(target == 1);
            REQUIRE(list[0] == 0);
            REQUIRE(list[1] == 2);
            REQUIRE(list.front() == 0);
            REQUIRE(list.back() == 2);
        }
        WHEN("last element") {
            marker = list.last_node();
            REQUIRE(*marker == 2);

            target = list.remove(marker);
            REQUIRE(target == 2);
            REQUIRE(list[0] == 0);
            REQUIRE(list[1] == 1);
            REQUIRE(list.front() == 0);
            REQUIRE(list.back() == 1);
        }
    }

    SECTION("list correctly output values") {
        // get list's output via std::string stream
        std::string output;
        std::stringstream ss;

        // assert correct output
        ss.str("");
        ss << list;
        output = ss.str();
        assert(output == "[0]->[1]->[2]->|||");

        // assert front and pop returns the correct values
        assert(list.front() == 0);
        assert(list.pop_front() == 0);
        assert(list.front() == 1);
        assert(list.pop_front() == 1);
        assert(list.front() == 2);
        assert(list.pop_front() == 2);

        // assert empty
        assert(list.empty() == true);

        // assert output is "|||"
        ss.str("");
        ss << list;
        output = ss.str();
        assert(output == "|||");
    }

    SECTION("copy constructor of list") {
        REQUIRE(list.empty() == false);  // non-empty default stack

        List<int> list1(list);            // copy default 'stack' into stack1
        REQUIRE(list1.empty() == false);  // stack1 should not be empty

        // test front and pop returns the correct values
        for(int i = 0; i <= 2; ++i) {
            REQUIRE(list1.front() == i);
            REQUIRE(list1.pop_front() == i);
        }

        REQUIRE(list1.empty() == true);  // list1 should now be empty
        REQUIRE(list.empty() == false);  // default list should not be empty
    }

    SECTION("assignment operator of list") {
        REQUIRE(list.empty() == false);  // non-empty default list

        // create list1
        List<int> list1;                 // instantiate with empty list object
        REQUIRE(list1.empty() == true);  // list1 should be empty

        list1 = list;                     // assign list1 to list's list
        REQUIRE(list1.empty() == false);  // list1 should not be empty

        // test front and pop returns the correct values
        for(int i = 0; i <= 2; ++i) {
            REQUIRE(list1.front() == i);
            REQUIRE(list1.pop_front() == i);
        }

        REQUIRE(list1.empty() == true);  // list1 should now be empty
        REQUIRE(list.empty() == false);  // default list should not be empty
    }

    SECTION("multiple reassignments of list") {
        REQUIRE(list.empty() == false);  // non-empty default list

        // create list1
        List<int> list1;                 // instantiate with empty list object
        REQUIRE(list1.empty() == true);  // list1 should be empty

        list1 = list;                     // assign list1 to list's list
        REQUIRE(list1.empty() == false);  // list1 should not be empty

        // add values to list1
        list1.push_back(3);
        list1.push_back(4);
        list1.push_back(5);

        // create list2 and list3
        List<int> list2, list3;
        REQUIRE(list2.empty() == true);  // list1 should be empty
        REQUIRE(list3.empty() == true);  // list1 should be empty

        list2 = list1;
        list3 = list2;
        REQUIRE(list1.empty() == false);  // list1 should not be empty
        REQUIRE(list2.empty() == false);  // list2 should not be empty
        REQUIRE(list3.empty() == false);  // list1 should not be empty

        // test front and pop returns the correct values
        for(int i = 0; i <= 5; ++i) {
            REQUIRE(list1.front() == i);
            REQUIRE(list1.pop_front() == i);
        }

        list2 = list;
        REQUIRE(list1.empty() == true);   // list1 should be empty
        REQUIRE(list2.empty() == false);  // list2 should not be empty
        REQUIRE(list3.empty() == false);  // list1 should not be empty

        list3 = list1;                    // assign list3 to empty list1
        REQUIRE(list1.empty() == true);   // list1 should be empty
        REQUIRE(list2.empty() == false);  // list2 should not be empty
        REQUIRE(list3.empty() == true);   // list1 should be empty

        // test front and pop returns the correct values
        for(int i = 0; i <= 2; ++i) {
            REQUIRE(list2.front() == i);
            REQUIRE(list2.pop_front() == i);
        }

        REQUIRE(list1.empty() == true);  // list1 should be empty
        REQUIRE(list2.empty() == true);  // list2 should be empty
        REQUIRE(list3.empty() == true);  // list1 should be empty

        // multiple reassignments
        list1 = list;
        list2 = list1;
        list3 = list2;
        list1 = list2;
        list2 = list1;
        list3 = list;

        // self assignments
        list1 = list1;
        list2 = list2;
        list3 = list3;

        REQUIRE(list1.empty() == false);  // list1 should not be empty
        REQUIRE(list2.empty() == false);  // list2 should not be empty
        REQUIRE(list3.empty() == false);  // list1 should not be empty
    }
}
