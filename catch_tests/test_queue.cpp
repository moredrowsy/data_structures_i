#include <sstream>
#include "../include/queue.h"
#include "../lib/catch.hpp"

TEST_CASE("my custom queue", "[queue]") {
    using namespace queue;

    // test empty;
    Queue<int> queue;
    REQUIRE(queue.empty() == true);

    // push 0 to 2
    for(int i = 0; i < 3; ++i) {
        queue.push(i);
    }
    REQUIRE(queue.empty() == false);

    SECTION("queue correctly output values") {
        // get list's output via std::string stream
        std::string output;
        std::stringstream ss;

        // assert correct output
        ss.str("");
        ss << queue;
        output = ss.str();
        assert(output == "[0]->[1]->[2]->|||");

        // assert front and pop returns the correct values
        assert(queue.front() == 0);
        assert(queue.pop() == 0);
        assert(queue.front() == 1);
        assert(queue.pop() == 1);
        assert(queue.front() == 2);
        assert(queue.pop() == 2);

        // assert empty
        assert(queue.empty() == true);

        // assert output is "|||"
        ss.str("");
        ss << queue;
        output = ss.str();
        assert(output == "|||");

        std::cout << "passed." << std::endl;
    }

    SECTION("copy constructor of queue") {
        REQUIRE(queue.empty() == false);  // non-empty default stack

        Queue<int> queue1(queue);          // copy default 'stack' into stack1
        REQUIRE(queue1.empty() == false);  // stack1 should not be empty

        // test front and pop returns the correct values
        for(int i = 0; i <= 2; ++i) {
            REQUIRE(queue1.front() == i);
            REQUIRE(queue1.pop() == i);
        }

        REQUIRE(queue1.empty() == true);  // queue1 should now be empty
        REQUIRE(queue.empty() == false);  // default queue should not be empty
    }

    SECTION("assignment operator of queue") {
        REQUIRE(queue.empty() == false);  // non-empty default queue

        // create queue1
        Queue<int> queue1;                // instantiate with empty Queue object
        REQUIRE(queue1.empty() == true);  // queue1 should be empty

        queue1 = queue;                    // assign queue1 to queue's list
        REQUIRE(queue1.empty() == false);  // queue1 should not be empty

        // test front and pop returns the correct values
        for(int i = 0; i <= 2; ++i) {
            REQUIRE(queue1.front() == i);
            REQUIRE(queue1.pop() == i);
        }

        REQUIRE(queue1.empty() == true);  // queue1 should now be empty
        REQUIRE(queue.empty() == false);  // default queue should not be empty
    }

    SECTION("multiple reassignments of queue") {
        REQUIRE(queue.empty() == false);  // non-empty default queue

        // create queue1
        Queue<int> queue1;                // instantiate with empty Queue object
        REQUIRE(queue1.empty() == true);  // queue1 should be empty

        queue1 = queue;                    // assign queue1 to queue's list
        REQUIRE(queue1.empty() == false);  // queue1 should not be empty

        // add values to queue1
        queue1.push(3);
        queue1.push(4);
        queue1.push(5);

        // create queue2 and queue3
        Queue<int> queue2, queue3;
        REQUIRE(queue2.empty() == true);  // queue1 should be empty
        REQUIRE(queue3.empty() == true);  // queue1 should be empty

        queue2 = queue1;
        queue3 = queue2;
        REQUIRE(queue1.empty() == false);  // queue1 should not be empty
        REQUIRE(queue2.empty() == false);  // queue2 should not be empty
        REQUIRE(queue3.empty() == false);  // queue1 should not be empty

        // test front and pop returns the correct values
        for(int i = 0; i <= 5; ++i) {
            REQUIRE(queue1.front() == i);
            REQUIRE(queue1.pop() == i);
        }

        queue2 = queue;
        REQUIRE(queue1.empty() == true);   // queue1 should be empty
        REQUIRE(queue2.empty() == false);  // queue2 should not be empty
        REQUIRE(queue3.empty() == false);  // queue1 should not be empty

        queue3 = queue1;                   // assign queue3 to empty queue1
        REQUIRE(queue1.empty() == true);   // queue1 should be empty
        REQUIRE(queue2.empty() == false);  // queue2 should not be empty
        REQUIRE(queue3.empty() == true);   // queue1 should be empty

        // test front and pop returns the correct values
        for(int i = 0; i <= 2; ++i) {
            REQUIRE(queue2.front() == i);
            REQUIRE(queue2.pop() == i);
        }

        REQUIRE(queue1.empty() == true);  // queue1 should be empty
        REQUIRE(queue2.empty() == true);  // queue2 should be empty
        REQUIRE(queue3.empty() == true);  // queue1 should be empty

        // multiple reassignments
        queue1 = queue;
        queue2 = queue1;
        queue3 = queue2;
        queue1 = queue2;
        queue2 = queue1;
        queue3 = queue;

        // self assignments
        queue1 = queue1;
        queue2 = queue2;
        queue3 = queue3;

        REQUIRE(queue1.empty() == false);  // queue1 should not be empty
        REQUIRE(queue2.empty() == false);  // queue2 should not be empty
        REQUIRE(queue3.empty() == false);  // queue1 should not be empty
    }
}
