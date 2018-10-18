/*******************************************************************************
 * AUTHOR        : Thuan Tang
 * ID            : 00991588
 * CLASS         : CS008
 * ASSIGNMENT 01 : List, Stack and Queue
 * DESCRIPTION   : This application will test queue and stack classes.
 *      Three queues and stacks are created by default constructor, copy
 *      constructor and copied via assignment operator.
 ******************************************************************************/
#include <iostream>            // output stream
#include "../include/list.h"   // List class
#include "../include/queue.h"  // Queue class
#include "../include/stack.h"  // Stack class

// test the queue class via the big three (copy, assignment, and destructor)
void test_list();

// test the queue class via the big three (copy, assignment, and destructor)
void test_queue();

// test the stack class via the big three (copy, assignment, and destructor)
void test_stack();

int main() {
    test_list();
    std::cout << std::endl;
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
void test_list() {
    using namespace list;

    // testing List
    std::cout << "Output For List:" << std::endl << std::endl;
    List<int> l, l2;

    // test: add items to l
    for(int i = 0; i < 10; ++i) l.push_front(i);

    std::cout << "l: " << l << std::endl << std::endl;

    // test: copy constructor from l to l2
    l2 = l;
    std::cout << "l2: " << l2 << std::endl << std::endl;

    // test: pop l while not empty
    while(!l.empty())
        std::cout << "{ " << l.pop_front() << " } " << l << std::endl
                  << std::endl;

    // test: reassign l to l2
    l = l2;
    std::cout << "assigning l back to l2: l: " << l << std::endl << std::endl;
    std::cout << "l2: " << l2 << std::endl << std::endl;

    std::cout << "Testing List's iterators:" << std::endl;
    List<int>::Iterator begin = l.begin(), end = l.end(),
                        last_node = l.last_node();

    std::cout << "begin = " << *begin << std::endl
              << "end = " << end << std::endl
              << "last_node = " << *last_node << std::endl;

    l.remove(-1);
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
    using namespace queue;

    // testing Queue
    std::cout << "Output For Queue:" << std::endl << std::endl;
    Queue<int> q, q2;

    // test: add items to q
    for(int i = 0; i < 10; ++i) q.push(i);

    std::cout << "q: " << q << std::endl << std::endl;

    // test: copy constructor from q to q2
    q2 = q;
    std::cout << "q2: " << q2 << std::endl << std::endl;

    // test: pop q while not empty
    while(!q.empty())
        std::cout << "{ " << q.pop() << " } " << q << std::endl << std::endl;

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
    using namespace stack;

    // testing Stack
    std::cout << "Output For Stack:" << std::endl << std::endl;
    Stack<int> s, s2;

    // test: add items to s
    for(int i = 0; i < 10; ++i) s.push(i);

    std::cout << "s: " << s << std::endl << std::endl;

    // test: copy constructor from s to s2
    s2 = s;
    std::cout << "s2: " << s2 << std::endl << std::endl;

    // test: pop s while not empty
    while(!s.empty())
        std::cout << "{ " << s.pop() << " } " << s << std::endl << std::endl;

    // test: reassign s to s2
    s = s2;
    std::cout << "assigning s back to s2: s: " << s << std::endl << std::endl;
    std::cout << "s2: " << s2 << std::endl << std::endl;
}
