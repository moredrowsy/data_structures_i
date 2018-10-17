#ifndef TEST_STACK_H
#define TEST_STACK_H

#include <cassert>
#include <iostream>
#include <sstream>
#include "../include/stack.h"

namespace test_stack {

// test class Queue and its basic member functions
void test_stack_basic();

// test deep copy for copy constructor and assignment operator
void test_stack_copy_and_assignment_op();

// test deep copy with 1mil items each; process over 5 mil items
void tests_stack_copy_and_assign_op_with_1mil_items();

// call all test functions
void test_all();

void test_stack_basic() {
    std::cout << "test_stack_basic: ";

    // assert empty;
    stack::Stack<int> s;
    assert(s.empty() == true);

    // get list's output via std::string stream
    std::string output;
    std::stringstream ss;

    // assert output is "|||"
    ss << s;
    output = ss.str();
    assert(output == "|||");

    // push 0 to 2
    for(int i = 0; i < 3; ++i) {
        s.push(i);
    }

    // assert correct output
    ss.str("");
    ss << s;
    output = ss.str();
    assert(output == "[2]->[1]->[0]->|||");

    // assert front and pop returns the correct values
    assert(s.top() == 2);
    assert(s.pop() == 2);
    assert(s.top() == 1);
    assert(s.pop() == 1);
    assert(s.top() == 0);
    assert(s.pop() == 0);

    // assert empty
    assert(s.empty() == true);

    // assert output is "|||"
    ss.str("");
    ss << s;
    output = ss.str();
    assert(output == "|||");

    std::cout << "passed." << std::endl;
}

void test_stack_copy_and_assignment_op() {
    std::cout << "test_stack_copy_and_assignment_op: ";

    // test one value stack
    stack::Stack<int> s1;
    for(int i = 0; i < 1; ++i) {
        s1.push(i);
    }

    // get list's output via std::string stream
    std::string output;
    std::stringstream ss;

    // assert correct output for s1
    ss << s1;
    output = ss.str();
    assert(output == "[0]->|||");

    // create copies to s2 and s3
    stack::Stack<int> s2(s1), s3;
    s3 = s1;

    // assert correct output for s2
    ss.str("");
    ss << s2;
    output = ss.str();
    assert(output == "[0]->|||");

    // assert correct output for s3
    ss.str("");
    ss << s3;
    output = ss.str();
    assert(output == "[0]->|||");

    // pop all nodes of s1
    while(!s1.empty()) {
        s1.pop();
    }

    // assert correct output for s1
    ss.str("");
    ss << s1;
    output = ss.str();
    assert(output == "|||");

    // assert correct output for s2
    ss.str("");
    ss << s2;
    output = ss.str();
    assert(output == "[0]->|||");

    // assert correct output for s3
    ss.str("");
    ss << s3;
    output = ss.str();
    assert(output == "[0]->|||");

    // reassign s1 to s2
    s1 = s2;

    // assert correct output for s1
    ss.str("");
    ss << s1;
    output = ss.str();
    assert(output == "[0]->|||");

    // pop all nodes of s2
    while(!s2.empty()) {
        s2.pop();
    }

    // assert correct output for s1
    ss.str("");
    ss << s1;
    output = ss.str();
    assert(output == "[0]->|||");

    // assert correct output for s2
    ss.str("");
    ss << s2;
    output = ss.str();
    assert(output == "|||");

    // assert correct output for s3
    ss.str("");
    ss << s3;
    output = ss.str();
    assert(output == "[0]->|||");

    // pop all nodes of s2
    while(!s3.empty()) {
        s3.pop();
    }

    // assert correct output for s1
    ss.str("");
    ss << s1;
    output = ss.str();
    assert(output == "[0]->|||");

    // assert correct output for s2
    ss.str("");
    ss << s2;
    output = ss.str();
    assert(output == "|||");

    // assert correct output for s3
    ss.str("");
    ss << s3;
    output = ss.str();
    assert(output == "|||");

    // test two values stack
    stack::Stack<int> s4;
    for(int i = 0; i < 2; ++i) {
        s4.push(i);
    }

    // assert correct output for s4
    ss.str("");
    ss << s4;
    output = ss.str();
    assert(output == "[1]->[0]->|||");

    // create copies to s5 and s6
    stack::Stack<int> s5(s4), s6;
    s6 = s4;

    // assert correct output for s5
    ss.str("");
    ss << s5;
    output = ss.str();
    assert(output == "[1]->[0]->|||");

    // assert correct output for s6
    ss.str("");
    ss << s6;
    output = ss.str();
    assert(output == "[1]->[0]->|||");

    // pop all nodes of s4
    while(!s4.empty()) {
        s4.pop();
    }

    // assert correct output for s4
    ss.str("");
    ss << s4;
    output = ss.str();
    assert(output == "|||");

    // assert correct output for s5
    ss.str("");
    ss << s5;
    output = ss.str();
    assert(output == "[1]->[0]->|||");

    // assert correct output for s6
    ss.str("");
    ss << s6;
    output = ss.str();
    assert(output == "[1]->[0]->|||");

    // reassign s4 to s5
    s4 = s5;

    // assert correct output for s4
    ss.str("");
    ss << s4;
    output = ss.str();
    assert(output == "[1]->[0]->|||");

    // pop all nodes of s5
    while(!s5.empty()) {
        s5.pop();
    }

    // assert correct output for s4
    ss.str("");
    ss << s4;
    output = ss.str();
    assert(output == "[1]->[0]->|||");

    // assert correct output for s5
    ss.str("");
    ss << s5;
    output = ss.str();
    assert(output == "|||");

    // assert correct output for s6
    ss.str("");
    ss << s6;
    output = ss.str();
    assert(output == "[1]->[0]->|||");

    // pop all nodes of s5
    while(!s6.empty()) {
        s6.pop();
    }

    // assert correct output for s4
    ss.str("");
    ss << s4;
    output = ss.str();
    assert(output == "[1]->[0]->|||");

    // assert correct output for s5
    ss.str("");
    ss << s5;
    output = ss.str();
    assert(output == "|||");

    // assert correct output for s6
    ss.str("");
    ss << s6;
    output = ss.str();
    assert(output == "|||");

    // test three values stack
    stack::Stack<int> s7;
    for(int i = 0; i < 3; ++i) {
        s7.push(i);
    }

    // assert correct output for s7
    ss.str("");
    ss << s7;
    output = ss.str();
    assert(output == "[2]->[1]->[0]->|||");

    // create copies to s8 and s9
    stack::Stack<int> s8(s7), s9;
    s9 = s7;

    // assert correct output for s8
    ss.str("");
    ss << s8;
    output = ss.str();
    assert(output == "[2]->[1]->[0]->|||");

    // assert correct output for s9
    ss.str("");
    ss << s9;
    output = ss.str();
    assert(output == "[2]->[1]->[0]->|||");

    // pop all nodes of s7
    while(!s7.empty()) {
        s7.pop();
    }

    // assert correct output for s7
    ss.str("");
    ss << s7;
    output = ss.str();
    assert(output == "|||");

    // assert correct output for s8
    ss.str("");
    ss << s8;
    output = ss.str();
    assert(output == "[2]->[1]->[0]->|||");

    // assert correct output for s9
    ss.str("");
    ss << s9;
    output = ss.str();
    assert(output == "[2]->[1]->[0]->|||");

    // reassign s7 to s8
    s7 = s8;

    // assert correct output for s7
    ss.str("");
    ss << s7;
    output = ss.str();
    assert(output == "[2]->[1]->[0]->|||");

    // pop all nodes of s8
    while(!s8.empty()) {
        s8.pop();
    }

    // assert correct output for s7
    ss.str("");
    ss << s7;
    output = ss.str();
    assert(output == "[2]->[1]->[0]->|||");

    // assert correct output for s8
    ss.str("");
    ss << s8;
    output = ss.str();
    assert(output == "|||");

    // assert correct output for s9
    ss.str("");
    ss << s9;
    output = ss.str();
    assert(output == "[2]->[1]->[0]->|||");

    // pop all nodes of s8
    while(!s9.empty()) {
        s9.pop();
    }

    // assert correct output for s7
    ss.str("");
    ss << s7;
    output = ss.str();
    assert(output == "[2]->[1]->[0]->|||");

    // assert correct output for s8
    ss.str("");
    ss << s8;
    output = ss.str();
    assert(output == "|||");

    // assert correct output for s9
    ss.str("");
    ss << s9;
    output = ss.str();
    assert(output == "|||");

    // reassign s7 to s1 when s7 is not empty
    ss.str("");
    ss << s7;
    output = ss.str();
    assert(output == "[2]->[1]->[0]->|||");
    s7 = s1;
    ss.str("");
    ss << s1;
    output = ss.str();
    assert(output == "[0]->|||");

    std::cout << "passed." << std::endl;
}

void tests_stack_copy_and_assign_op_with_1mil_items() {
    std::cout << "test_stack_copy_and_assign_op_with_1mil_items: ";

    // push 1 million items into stack and static_stack (for comparison)
    stack::Stack<int> s1, static_stack;
    for(long i = 0; i < 1000000; ++i) {
        s1.push(i);
        static_stack.push(i);
        assert(s1.top() == i);
        assert(static_stack.top() == i);
    }

    // test copy constructor and assignment op for 1 million items
    stack::Stack<int> s2, s3(s1);
    s2 = s1;

    // expect same values for s1 and s2
    int item1, item2;
    while(!s2.empty()) {
        item1 = s1.pop();
        item2 = s2.pop();
        assert(item1 == item2);
    }

    // restore s1
    s1 = static_stack;
    while(!s3.empty()) {
        item1 = s1.pop();
        item2 = s3.pop();
        assert(item1 == item2);
    }

    // test for assignment operator via multiple assignments
    s1 = static_stack;
    s2 = static_stack;
    s3 = static_stack;
    s1 = s2;
    s2 = s3;
    s3 = s1;

    std::cout << "passed." << std::endl;
}

void test_all() {
    std::cout << "Unit tests for stack." << std::endl;

    test_stack_basic();
    test_stack_copy_and_assignment_op();
    tests_stack_copy_and_assign_op_with_1mil_items();
}

}  // namespace test_stack

#endif  // TEST_STACK_H
