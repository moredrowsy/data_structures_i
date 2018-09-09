#ifndef TEST_MY_NODE_H
#define TEST_MY_NODE_H

#include <cassert>
#include <iostream>
#include "../include/my_node.h"

namespace test_my_node {

// test that pointer is assigned to NULL
void test_init_head();

// test that delete all assigns first pointer to NULL when deallocated success
void test_delete_all();

// test for correct boolean
void test_empty();

// test for deep copy of list
void test_copy_list();

// test that node is inserted at head
void test_insert_head();

// test that node is inserted behind
void testing_insert_after();

// test that first node is deallocated
void testing_delete_head();

// call all test functions
void test_all();

void test_init_head() {
    std::cout << "test_init_head: ";

    my_node::node<int> *head = new my_node::node<int>(10);
    my_node::node<int> *temp = head;

    init_head(head);

    assert(head == NULL);

    delete temp;

    std::cout << "passed." << std::endl;
}

void test_delete_all() {
    std::cout << "test_delete_all: ";

    // create nodes for list
    my_node::node<int> *a, *b, *c;
    a = new my_node::node<int>(0);
    b = new my_node::node<int>(1);
    c = new my_node::node<int>(2);

    // link nodes like a list
    a->_next = b;
    b->_next = c;

    delete_all(a);

    assert(a == NULL);

    std::cout << "passed." << std::endl;
}

void test_empty() {
    std::cout << "test_int empty: ";
    my_node::node<int> *a;

    a = new my_node::node<int>(0);
    assert(empty(a) == false);

    delete a;
    a = NULL;
    assert(empty(a) == true);

    std::cout << "passed." << std::endl;
}

void test_copy_list() {
    std::cout << "test_copy_list: ";

    // create nodes for list
    my_node::node<int> *a, *b, *c, *d, *e, *f;
    a = new my_node::node<int>(0);
    b = new my_node::node<int>(1);
    c = new my_node::node<int>(2);
    d = new my_node::node<int>(3);
    e = new my_node::node<int>(4);
    f = new my_node::node<int>(5);

    // link nodes for list one
    a->_next = b;
    b->_next = c;

    // link nodes for list two
    d->_next = e;
    e->_next = f;

    // assert not empty list
    assert(a != NULL);
    assert(d != NULL);

    copy_list(a, d);

    assert(&a != &d);
    assert(d->_item == 0);
    assert(d->_next != NULL);
    assert(d->_next->_item == 1);
    assert(d->_next->_next != NULL);
    assert(d->_next->_next->_item == 2);
    assert(d->_next->_next->_next == NULL);

    delete_all(a);
    delete_all(d);

    std::cout << "passed." << std::endl;
}

void test_insert_head() {
    std::cout << "test_insert_head: ";

    my_node::node<int> *a = NULL;

    // assert first insertion
    insert_head(a, 1);
    assert(a != NULL);
    assert(a->_item == 1);
    assert(a->_next == NULL);

    // assert second insertion
    insert_head(a, 2);
    assert(a != NULL);
    assert(a->_item == 2);
    assert(a->_next->_item == 1);
    assert(a->_next->_next == NULL);

    // assert third insertion
    insert_head(a, 3);
    assert(a != NULL);
    assert(a->_item == 3);
    assert(a->_next->_item == 2);
    assert(a->_next->_next->_item == 1);
    assert(a->_next->_next->_next == NULL);

    delete_all(a);

    std::cout << "passed." << std::endl;
}

void testing_insert_after() {
    std::cout << "test_insert_after: ";

    my_node::node<int> *a = NULL, *b = NULL;

    // test first insert after
    b = insert_after(a, b, 0);
    assert(a != NULL);
    assert(b != NULL);
    assert(a->_item == 0);
    assert(a->_next == NULL);
    assert(b->_item == 0);
    assert(b->_next == NULL);

    // test second insert after
    b = insert_after(a, b, 1);
    assert(a != NULL);
    assert(b != NULL);
    assert(a->_item == 0);
    assert(a->_next->_item == 1);
    assert(a->_next->_next == NULL);
    assert(b->_item == 1);
    assert(b->_next == NULL);

    // test third insert after
    b = insert_after(a, b, 2);
    assert(a != NULL);
    assert(b != NULL);
    assert(a->_item == 0);
    assert(a->_next->_item == 1);
    assert(a->_next->_next->_item == 2);
    assert(a->_next->_next->_next == NULL);
    assert(b->_item == 2);
    assert(b->_next == NULL);

    delete_all(a);

    std::cout << "passed." << std::endl;
}

void testing_delete_head() {
    std::cout << "test_delete_head: ";

    my_node::node<int> *a, *b, *c;
    a = new my_node::node<int>(0);
    b = new my_node::node<int>(1);
    c = new my_node::node<int>(2);
    a->_next = b;
    b->_next = c;

    // test first deletion
    assert(delete_head(a) == 0);
    assert(a == b);

    // test second deletion
    assert(delete_head(a) == 1);
    assert(a == c);

    // tests third deletion
    assert(delete_head(a) == 2);
    assert(a == NULL);

    delete_all(a);

    std::cout << "passed." << std::endl;
}

void tests_my_list() {
    std::cout << "Unit tests for my_node." << std::endl;

    test_init_head();
    test_delete_all();
    test_empty();
    test_copy_list();
    test_insert_head();
    testing_insert_after();
    testing_delete_head();
}

void test_all() {
    std::cout << "Unit tests for my_list." << std::endl;

    test_init_head();
    test_delete_all();
    test_empty();
    test_copy_list();
    test_insert_head();
    testing_insert_after();
    testing_delete_head();
}

}  // namespace test_my_node

#endif  // TEST_MY_NODE_H
