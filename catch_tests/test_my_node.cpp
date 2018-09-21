#include "../include/my_node.h"
#include "../lib/catch.hpp"

TEST_CASE("my custom node", "[my_node]") {
    using namespace my_node;

    // dynamically allocate memory
    Node<int> *head = new Node<int>(0), *a = new Node<int>(1),
              *b = new Node<int>(2), *c = new Node<int>(3),
              *tail = new Node<int>(4);

    // link all 4 Nodes
    head->_next = a;
    a->_next = b;
    b->_next = c;
    c->_next = tail;

    REQUIRE(head->_item == 0);
    REQUIRE(head->_next != nullptr);
    REQUIRE(a->_item == 1);
    REQUIRE(a->_next != nullptr);
    REQUIRE(b->_item == 2);
    REQUIRE(b->_next != nullptr);
    REQUIRE(c->_item == 3);
    REQUIRE(c->_next != nullptr);
    REQUIRE(tail->_item == 4);
    REQUIRE(tail->_next == nullptr);

    SECTION("init_head() will set Node to nullptr") {
        delete head;
        delete a;
        delete b;
        delete c;
        delete tail;      // delete all Nodes
        init_head(head);  // init head Node to nullptr

        REQUIRE(head == nullptr);
    }

    SECTION("delete_all() will delete all linked Nodes") {
        delete_all(head);  // delete all Nodes and head should now be nullptr

        REQUIRE(head == nullptr);
    }

    SECTION("empty() will check if a Node is nullptr") {
        REQUIRE(empty(head) == false);

        delete_all(head);  // delete all Nodes and head should now be nullptr

        REQUIRE(empty(head) == true);
    }

    SECTION("copy_list() will copy all linked Nodes from head to new Node") {
        Node<int> *cpy = nullptr, *cpy_walker = nullptr, *walker = nullptr;
        copy_list(head, cpy);  // cpy source head to cpy

        cpy_walker = cpy;
        walker = head;
        while(walker != nullptr) {
            REQUIRE(walker->_item == cpy_walker->_item);  // same values

            // check that _next ptr are different addresses
            if(walker->_next == nullptr)
                REQUIRE(walker->_next == cpy_walker->_next);
            else
                REQUIRE(walker->_next != cpy_walker->_next);

            walker = walker->_next;
            cpy_walker = cpy_walker->_next;
        }

        delete_all(cpy);
    }

    SECTION("insert_head() will insert Node at the front") {
        insert_head(head, -1);
        insert_head(head, -2);
        insert_head(head, -3);

        Node<int> *walker = head;
        for(int i = -3; i < 5; ++i) {
            REQUIRE(walker->_item == i);
            walker = walker->_next;
        }
    }

    SECTION("insert_head() will insert Node at empty Node") {
        Node<int> *empty_node;
        init_head(empty_node);
        REQUIRE(empty(empty_node) == true);

        insert_head(empty_node, 1);
        insert_head(empty_node, 0);
        insert_head(empty_node, -1);

        Node<int> *walker = empty_node;
        for(int i = -1; i < 2; ++i) {
            REQUIRE(walker->_item == i);
            walker = walker->_next;
        }

        delete_all(empty_node);
    }

    SECTION("insert_after() will insert Node at the end") {
        Node<int> *last_node = tail;
        last_node = insert_after(head, last_node, 5);
        last_node = insert_after(head, last_node, 6);
        last_node = insert_after(head, last_node, 7);

        Node<int> *walker = head;
        for(int i = 0; i < 8; ++i) {
            REQUIRE(walker->_item == i);
            walker = walker->_next;
        }

        delete_all(head);
    }

    SECTION("insert_after() will insert Node on empty Node") {
        Node<int> *empty_head_node, *empty_tail_node;
        init_head(empty_head_node);
        init_head(empty_tail_node);
        REQUIRE(empty(empty_head_node) == true);
        REQUIRE(empty(empty_tail_node) == true);

        empty_tail_node = insert_after(empty_head_node, empty_tail_node, 0);
        empty_tail_node = insert_after(empty_head_node, empty_tail_node, 1);
        empty_tail_node = insert_after(empty_head_node, empty_tail_node, 2);

        Node<int> *walker = empty_head_node;
        for(int i = 0; i < 3; ++i) {
            REQUIRE(walker->_item == i);
            walker = walker->_next;
        }

        delete_all(empty_head_node);
    }

    SECTION("delete_head() will remove and deallocate the first Node") {
        for(int i = 0; i < 5; ++i) {
            delete_head(head);
            if(!empty(head)) {
                REQUIRE(head->_item == i + 1);
            }
        }

        REQUIRE(empty(head) == true);
    }

    delete_all(head);
}
