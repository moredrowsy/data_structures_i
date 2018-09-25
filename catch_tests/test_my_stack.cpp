#include <sstream>
#include "../include/my_stack.h"
#include "../lib/catch.hpp"

TEST_CASE("my custom Stack", "[my_stack]") {
    using namespace my_stack;

    // test empty;
    Stack<int> stack;
    REQUIRE(stack.empty() == true);

    // push 0 to 2
    for(int i = 0; i < 3; ++i) {
        stack.push(i);
    }
    REQUIRE(stack.empty() == false);

    SECTION("stack correctly output values") {
        // get list's output via std::string stream
        std::string output;
        std::stringstream ss;

        // test correct output
        ss.str("");
        ss << stack;
        output = ss.str();
        REQUIRE(output == "[2]->[1]->[0]->|||");

        // test front and pop returns the correct values
        for(int i = 2; i >= 0; --i) {
            REQUIRE(stack.top() == i);
            REQUIRE(stack.pop() == i);
        }

        // test empty
        REQUIRE(stack.empty() == true);

        // test output is "|||"
        ss.str("");
        ss << stack;
        output = ss.str();
        REQUIRE(output == "|||");

        // re-push 0 to 2
        for(int i = 0; i < 3; ++i) {
            stack.push(i);
        }

        // test correct output
        ss.str("");
        ss << stack;
        output = ss.str();
        REQUIRE(output == "[2]->[1]->[0]->|||");
    }

    SECTION("copy constructor of Stack") {
        REQUIRE(stack.empty() == false);  // non-empty default stack

        Stack<int> stack1(stack);          // copy default 'stack' into stack1
        REQUIRE(stack1.empty() == false);  // stack1 should not be empty

        // test front and pop returns the correct values
        for(int i = 2; i >= 0; --i) {
            REQUIRE(stack1.top() == i);
            REQUIRE(stack1.pop() == i);
        }

        REQUIRE(stack1.empty() == true);  // stack1 should now be empty
        REQUIRE(stack.empty() == false);  // default stack should not be empty
    }

    SECTION("assignment operator of Stack") {
        REQUIRE(stack.empty() == false);  // non-empty default stack

        // create stack1
        Stack<int> stack1;                // instantiate with empty Stack object
        REQUIRE(stack1.empty() == true);  // stack1 should be empty

        stack1 = stack;                    // assign stack1 to stack's list
        REQUIRE(stack1.empty() == false);  // stack1 should not be empty

        // test front and pop returns the correct values
        for(int i = 2; i >= 0; --i) {
            REQUIRE(stack1.top() == i);
            REQUIRE(stack1.pop() == i);
        }

        REQUIRE(stack1.empty() == true);  // stack1 should now be empty
        REQUIRE(stack.empty() == false);  // default stack should not be empty
    }

    SECTION("multiple reassignments of Stack") {
        REQUIRE(stack.empty() == false);  // non-empty default stack

        // create stack1
        Stack<int> stack1;                // instantiate with empty Stack object
        REQUIRE(stack1.empty() == true);  // stack1 should be empty

        stack1 = stack;                    // assign stack1 to stack's list
        REQUIRE(stack1.empty() == false);  // stack1 should not be empty

        // add values to stack1
        stack1.push(3);
        stack1.push(4);
        stack1.push(5);

        // create stack2 and stack3
        Stack<int> stack2, stack3;
        REQUIRE(stack2.empty() == true);  // stack1 should be empty
        REQUIRE(stack3.empty() == true);  // stack1 should be empty

        stack2 = stack1;
        stack3 = stack2;
        REQUIRE(stack1.empty() == false);  // stack1 should not be empty
        REQUIRE(stack2.empty() == false);  // stack2 should not be empty
        REQUIRE(stack3.empty() == false);  // stack1 should not be empty

        // test front and pop returns the correct values
        for(int i = 5; i >= 0; --i) {
            REQUIRE(stack1.top() == i);
            REQUIRE(stack1.pop() == i);
        }

        stack2 = stack;
        REQUIRE(stack1.empty() == true);   // stack1 should be empty
        REQUIRE(stack2.empty() == false);  // stack2 should not be empty
        REQUIRE(stack3.empty() == false);  // stack1 should not be empty

        stack3 = stack1;                   // assign stack3 to empty stack1
        REQUIRE(stack1.empty() == true);   // stack1 should be empty
        REQUIRE(stack2.empty() == false);  // stack2 should not be empty
        REQUIRE(stack3.empty() == true);   // stack1 should be empty

        // test front and pop returns the correct values
        for(int i = 2; i >= 0; --i) {
            REQUIRE(stack2.top() == i);
            REQUIRE(stack2.pop() == i);
        }

        REQUIRE(stack1.empty() == true);  // stack1 should be empty
        REQUIRE(stack2.empty() == true);  // stack2 should be empty
        REQUIRE(stack3.empty() == true);  // stack1 should be empty

        // multiple reassignments
        stack1 = stack;
        stack2 = stack1;
        stack3 = stack2;
        stack1 = stack2;
        stack2 = stack1;
        stack3 = stack;

        // self assignments
        stack1 = stack1;
        stack2 = stack2;
        stack3 = stack3;

        REQUIRE(stack1.empty() == false);  // stack1 should not be empty
        REQUIRE(stack2.empty() == false);  // stack2 should not be empty
        REQUIRE(stack3.empty() == false);  // stack1 should not be empty
    }
}
