/*******************************************************************************
 * AUTHOR        : Thuan Tang
 * ID            : 00991588
 * CLASS         : CS008
 * ASSIGNMENT 01 : Stack and Queue
 * DESCRIPTION   : This application will test my_queue and my_stack classes.
 *      Three queues and stacks are created by default constructor, copy
 *      constructor and copied via assignment operator.
 ******************************************************************************/
#include <iostream>
#include "../include/my_node.h"
#include "../include/my_queue.h"
#include "../include/my_stack.h"

// test the queue class via the big three (copy, assignment, and destructor)
void test_queue();

// test the stack class via the big three (copy, assignment, and destructor)
void test_stack();

int main(int argc, char *argv[]) {
    test_queue();
    std::cout << std::endl;
    test_stack();

    return 0;
}

/*******************************************************************************
 * PRE-CONDITIONS:
 *  None
 *
 * POST-CONDITIONS:
 *  Queue output showing the correct values
 *
 * RETURN:
 *  none
 ******************************************************************************/
void test_queue() {
    // testing Queue
    std::cout << "Output For Queue:" << std::endl << std::endl;
    my_queue::Queue<int> q, q2;

    // test: add items to q
    for(int i = 0; i < 10; ++i) {
        q.push(i);
    }
    std::cout << "q: " << q << std::endl << std::endl;

    // test: copy constructor from q to q2
    q2 = q;
    std::cout << "q2: " << q2 << std::endl << std::endl;

    // test: pop q while not empty
    while(!q.empty()) {
        std::cout << "{ " << q.pop() << " } " << q << std::endl << std::endl;
    }

    // test: reassign q to q2
    q = q2;
    std::cout << "assigning q back to q2: q: " << q << std::endl << std::endl;
    std::cout << "q2: " << q2 << std::endl << std::endl;
}

/*******************************************************************************
 * PRE-CONDITIONS:
 *  None
 *
 * POST-CONDITIONS:
 *  Stack output showing the correct values
 *
 * RETURN:
 *  none
 ******************************************************************************/
void test_stack() {
    // testing Stack
    std::cout << "Output For Stack:" << std::endl << std::endl;
    my_stack::Stack<int> s, s2;

    // test: add items to s
    for(int i = 0; i < 10; ++i) {
        s.push(i);
    }
    std::cout << "s: " << s << std::endl << std::endl;

    // test: copy constructor from s to s2
    s2 = s;
    std::cout << "s2: " << s2 << std::endl << std::endl;

    // test: pop s while not empty
    while(!s.empty()) {
        std::cout << "{ " << s.pop() << " } " << s << std::endl << std::endl;
    }

    // test: reassign s to s2
    s = s2;
    std::cout << "assigning s back to s2: s: " << s << std::endl << std::endl;
    std::cout << "s2: " << s2 << std::endl << std::endl;
}
