#ifndef TEST_QUEUE_H
#define TEST_QUEUE_H

#include <cassert>
#include <iostream>
#include <sstream>
#include "../include/queue.h"

namespace test_queue {

// test class Queue and its basic member functions
void test_queue_basic();

// test deep copy for copy constructor and assignment operator
void test_queue_copy_and_assignment_op();

// test deep copy with 1mil items each; process over 5 mil items
void test_queue_copy_and_assign_op_with_1mil_items();

// call all test functions
void test_all();

void test_queue_basic() {
    std::cout << "test_queue_basic: ";

    // assert empty
    queue::Queue<int> q;
    assert(q.empty() == true);

    // get list's output via std::string stream
    std::string output;
    std::stringstream ss;

    // assert output is "|||"
    ss << q;
    output = ss.str();
    assert(output == "|||");

    // push 0 to 2
    for(int i = 0; i < 3; ++i) {
        q.push(i);
    }

    // assert correct output
    ss.str("");
    ss << q;
    output = ss.str();
    assert(output == "[0]->[1]->[2]->|||");

    // assert front and pop returns the correct values
    assert(q.front() == 0);
    assert(q.pop() == 0);
    assert(q.front() == 1);
    assert(q.pop() == 1);
    assert(q.front() == 2);
    assert(q.pop() == 2);

    // assert empty
    assert(q.empty() == true);

    // assert output is "|||"
    ss.str("");
    ss << q;
    output = ss.str();
    assert(output == "|||");

    std::cout << "passed." << std::endl;
}

void test_queue_copy_and_assignment_op() {
    std::cout << "test_queue_copy_and_assignment_op: ";

    // test one value queue
    queue::Queue<int> q1;
    for(int i = 0; i < 1; ++i) {
        q1.push(i);
    }

    // get list's output via std::string stream
    std::string output;
    std::stringstream ss;

    // assert correct output for q1
    ss << q1;
    output = ss.str();
    assert(output == "[0]->|||");

    // create copies to q2 and q3
    queue::Queue<int> q2(q1), q3;
    q3 = q1;

    // assert correct output for q2
    ss.str("");
    ss << q2;
    output = ss.str();
    assert(output == "[0]->|||");

    // assert correct output for q3
    ss.str("");
    ss << q3;
    output = ss.str();
    assert(output == "[0]->|||");

    // pop all nodes of q1
    while(!q1.empty()) {
        q1.pop();
    }

    // assert correct output for q1
    ss.str("");
    ss << q1;
    output = ss.str();
    assert(output == "|||");

    // assert correct output for q2
    ss.str("");
    ss << q2;
    output = ss.str();
    assert(output == "[0]->|||");

    // assert correct output for q3
    ss.str("");
    ss << q3;
    output = ss.str();
    assert(output == "[0]->|||");

    // reassign q1 to q2
    q1 = q2;

    // assert correct output for q1
    ss.str("");
    ss << q1;
    output = ss.str();
    assert(output == "[0]->|||");

    // pop all nodes of q2
    while(!q2.empty()) {
        q2.pop();
    }

    // assert correct output for q1
    ss.str("");
    ss << q1;
    output = ss.str();
    assert(output == "[0]->|||");

    // assert correct output for q2
    ss.str("");
    ss << q2;
    output = ss.str();
    assert(output == "|||");

    // assert correct output for q3
    ss.str("");
    ss << q3;
    output = ss.str();
    assert(output == "[0]->|||");

    // pop all nodes of q2
    while(!q3.empty()) {
        q3.pop();
    }

    // assert correct output for q1
    ss.str("");
    ss << q1;
    output = ss.str();
    assert(output == "[0]->|||");

    // assert correct output for q2
    ss.str("");
    ss << q2;
    output = ss.str();
    assert(output == "|||");

    // assert correct output for q3
    ss.str("");
    ss << q3;
    output = ss.str();
    assert(output == "|||");

    // test two values queue
    queue::Queue<int> q4;
    for(int i = 0; i < 2; ++i) {
        q4.push(i);
    }

    // assert correct output for q4
    ss.str("");
    ss << q4;
    output = ss.str();
    assert(output == "[0]->[1]->|||");

    // create copies to q5 and q6
    queue::Queue<int> q5(q4), q6;
    q6 = q4;

    // assert correct output for q5
    ss.str("");
    ss << q5;
    output = ss.str();
    assert(output == "[0]->[1]->|||");

    // assert correct output for q6
    ss.str("");
    ss << q6;
    output = ss.str();
    assert(output == "[0]->[1]->|||");

    // pop all nodes of q4
    while(!q4.empty()) {
        q4.pop();
    }

    // assert correct output for q4
    ss.str("");
    ss << q4;
    output = ss.str();
    assert(output == "|||");

    // assert correct output for q5
    ss.str("");
    ss << q5;
    output = ss.str();
    assert(output == "[0]->[1]->|||");

    // assert correct output for q6
    ss.str("");
    ss << q6;
    output = ss.str();
    assert(output == "[0]->[1]->|||");

    // reassign q4 to q5
    q4 = q5;

    // assert correct output for q4
    ss.str("");
    ss << q4;
    output = ss.str();
    assert(output == "[0]->[1]->|||");

    // pop all nodes of q5
    while(!q5.empty()) {
        q5.pop();
    }

    // assert correct output for q4
    ss.str("");
    ss << q4;
    output = ss.str();
    assert(output == "[0]->[1]->|||");

    // assert correct output for q5
    ss.str("");
    ss << q5;
    output = ss.str();
    assert(output == "|||");

    // assert correct output for q6
    ss.str("");
    ss << q6;
    output = ss.str();
    assert(output == "[0]->[1]->|||");

    // pop all nodes of q5
    while(!q6.empty()) {
        q6.pop();
    }

    // assert correct output for q4
    ss.str("");
    ss << q4;
    output = ss.str();
    assert(output == "[0]->[1]->|||");

    // assert correct output for q5
    ss.str("");
    ss << q5;
    output = ss.str();
    assert(output == "|||");

    // assert correct output for q6
    ss.str("");
    ss << q6;
    output = ss.str();
    assert(output == "|||");

    // test three values queue
    queue::Queue<int> q7;
    for(int i = 0; i < 3; ++i) {
        q7.push(i);
    }

    // assert correct output for q7
    ss.str("");
    ss << q7;
    output = ss.str();
    assert(output == "[0]->[1]->[2]->|||");

    // create copies to q8 and q9
    queue::Queue<int> q8(q7), q9;
    q9 = q7;

    // assert correct output for q8
    ss.str("");
    ss << q8;
    output = ss.str();
    assert(output == "[0]->[1]->[2]->|||");

    // assert correct output for q9
    ss.str("");
    ss << q9;
    output = ss.str();
    assert(output == "[0]->[1]->[2]->|||");

    // pop all nodes of q7
    while(!q7.empty()) {
        q7.pop();
    }

    // assert correct output for q7
    ss.str("");
    ss << q7;
    output = ss.str();
    assert(output == "|||");

    // assert correct output for q8
    ss.str("");
    ss << q8;
    output = ss.str();
    assert(output == "[0]->[1]->[2]->|||");

    // assert correct output for q9
    ss.str("");
    ss << q9;
    output = ss.str();
    assert(output == "[0]->[1]->[2]->|||");

    // reassign q7 to q8
    q7 = q8;

    // assert correct output for q7
    ss.str("");
    ss << q7;
    output = ss.str();
    assert(output == "[0]->[1]->[2]->|||");

    // pop all nodes of q8
    while(!q8.empty()) {
        q8.pop();
    }

    // assert correct output for q7
    ss.str("");
    ss << q7;
    output = ss.str();
    assert(output == "[0]->[1]->[2]->|||");

    // assert correct output for q8
    ss.str("");
    ss << q8;
    output = ss.str();
    assert(output == "|||");

    // assert correct output for q9
    ss.str("");
    ss << q9;
    output = ss.str();
    assert(output == "[0]->[1]->[2]->|||");

    // pop all nodes of q8
    while(!q9.empty()) {
        q9.pop();
    }

    // assert correct output for q7
    ss.str("");
    ss << q7;
    output = ss.str();
    assert(output == "[0]->[1]->[2]->|||");

    // assert correct output for q8
    ss.str("");
    ss << q8;
    output = ss.str();
    assert(output == "|||");

    // assert correct output for q9
    ss.str("");
    ss << q9;
    output = ss.str();
    assert(output == "|||");

    // reassign q7 to q1 when q7 is not empty
    ss.str("");
    ss << q7;
    output = ss.str();
    assert(output == "[0]->[1]->[2]->|||");
    q7 = q1;
    ss.str("");
    ss << q1;
    output = ss.str();
    assert(output == "[0]->|||");

    std::cout << "passed." << std::endl;
}

void test_queue_copy_and_assign_op_with_1mil_items() {
    std::cout << "test_queue_copy_and_assign_op_with_1mil_items: ";

    // push 1 million items into queue and static_queue (for comparison)
    queue::Queue<int> q1, static_queue;
    for(long i = 0; i < 1000000; ++i) {
        q1.push(i);
        static_queue.push(i);
        assert(q1.front() == 0);
        assert(static_queue.front() == 0);
    }

    // test copy constructor and assignment op for 1 million items
    queue::Queue<int> q2, q3(q1);
    q2 = q1;

    // expect same values for q1 and q2
    int item1, item2;
    while(!q2.empty()) {
        item1 = q1.pop();
        item2 = q2.pop();
        assert(item1 == item2);
    }

    // restore q1
    q1 = static_queue;
    while(!q3.empty()) {
        item1 = q1.pop();
        item2 = q3.pop();
        assert(item1 == item2);
    }

    // test for assignment operator via multiple assignments
    q1 = static_queue;
    q2 = static_queue;
    q3 = static_queue;
    q1 = q2;
    q2 = q3;
    q3 = q1;

    std::cout << "passed." << std::endl;
}

void test_all() {
    std::cout << "Unit tests for queue." << std::endl;

    test_queue_basic();
    test_queue_copy_and_assignment_op();
    test_queue_copy_and_assign_op_with_1mil_items();
}

}  // namespace test_queue

#endif  // TEST_QUEUE_H
