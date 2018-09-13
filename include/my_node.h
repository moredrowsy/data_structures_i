/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : my_node
 * DESCRIPTION : This header defines a templated node and templated functions
 *      that will process the nodes. These functions are the base to build
 *      higher level linked lists.
 ******************************************************************************/
#ifndef MY_NODE_H
#define MY_NODE_H

#include <iostream>  // output stream

namespace my_node {

template <typename T>
struct Node {
    // CONSTRUCTORS
    Node(const T &item = T(), Node<T> *next = NULL)
        : _item(item), _next(next) {}

    // VARIABLES
    T _item;         // templated item
    Node<T> *_next;  // pointer to next node

    // FRIENDS
    friend std::ostream &operator<<(std::ostream &outs,         // oustream
                                    const Node<T> &print_me) {  // print node
        outs << "[" << print_me._item << "]->";
        return outs;  // return out stream with node's item
    }
};

// assigns pointer to NULL
template <typename T>
Node<T> *init_head(Node<T> *&head);

// deletes all the nodes in the list
template <typename T>
void delete_all(Node<T> *&head);

// true if head is NULL, false otherwise.
template <typename T>
bool empty(const Node<T> *head);

// makes a copy of the list, returns a pointer to the last node:
template <typename T>
Node<T> *copy_list(const Node<T> *head, Node<T> *&cpy);

// insert at the beginning of the list:
template <typename T>
Node<T> *insert_head(Node<T> *&head, T item);

// insert_after: if after is NULL, inserts at head
template <typename T>
Node<T> *insert_after(Node<T> *&head, Node<T> *after, const T &item);

// delete the node at the head of the list, return the item:
template <typename T>
T delete_head(Node<T> *&head);

// print the list and return outs
template <typename T>
std::ostream &print_list(const Node<T> *head, std::ostream &outs = std::cout);

template <typename T>
Node<T> *init_head(Node<T> *&head) {
    return head = NULL;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Deallocates all nodes linked to the head node.
 *
 * PRE-CONDITIONS:
 *  Node<T> *&head: templated node
 *
 * POST-CONDITIONS:
 *  Node<T> *&head: assigns to NULL when all nodes deleted successfully
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void delete_all(Node<T> *&head) {
    Node<T> *pop = NULL;

    // delete head until empty
    while(head != NULL) {
        pop = head;
        head = head->_next;
        delete pop;
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Simply checks for NULL.
 *
 * PRE-CONDITIONS:
 *  Node<T> *&head: templated node
 *
 * POST-CONDITIONS:
 *  Node<T> *&head: assigns to NULL
 *
 * RETURN:
 *  Boolean condition.
 ******************************************************************************/
template <typename T>
bool empty(const Node<T> *head) {
    return head == NULL;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Deletes all nodes linked to cpy and linked to a new copy of list.
 *
 * PRE-CONDITIONS:
 *  Node<T> *&head: source node to copy from
 *  Node<T> *&cpy : target node to copy to
 *
 * POST-CONDITIONS:
 *  Node<T> *&cpy : old values deleted and to new copy of list
 *
 * RETURN:
 *  Pointer to last new node.
 ******************************************************************************/
template <typename T>
Node<T> *copy_list(const Node<T> *head, Node<T> *&cpy) {
    // delete all nodes when target cpy is not empty
    if(cpy != NULL) {
        delete_all(cpy);
    }

    // initialize walker for cpy
    Node<T> *copy_walker = NULL;

    // copy the rest of the nodes
    while(head != NULL) {
        // copy & assign first node to cpy, then copy the rest via walker
        if(cpy == NULL) {
            copy_walker = cpy = new Node<T>(head->_item);
            head = head->_next;
        } else {
            copy_walker->_next = new Node<T>(head->_item);
            copy_walker = copy_walker->_next;
            head = head->_next;
        }
    }

    return copy_walker;
}

/*******************************************************************************
 * DESCRIPTION:
 *  New node is created with param of item to T, and next pointer to old head.
 *  Then, assigned to head and return.
 *
 * PRE-CONDITIONS:
 *  Node<T> *&head: target node to insert
 *  T item        : templated item to copy into new node's item
 *
 * POST-CONDITIONS:
 *  New node is added before head
 *
 * RETURN:
 *  Pointer to new node.
 ******************************************************************************/
template <typename T>
Node<T> *insert_head(Node<T> *&head, T item) {
    // assign head to new node with T and next pointer as old head
    return head = new Node<T>(item, head);
}

/*******************************************************************************
 * DESCRIPTION:
 *  New node is created with param of item to T, and next pointer NULL. When
 *  head or after is NULL, insert at head or when after is not NULL, insert
 *  behind after.
 *
 * PRE-CONDITIONS:
 *  Node<T> *&head: target node to insert when head or after is NULL
 *  Node<T> *after: target node to insert when after is not NULL
 *  T item        : templated item to copy into new node's item
 *
 * POST-CONDITIONS:
 *  New node is added at head when head or tail is NULL; else new node is
 *  inserted behind after.
 *
 * RETURN:
 *  Pointer to new node.
 ******************************************************************************/
template <typename T>
Node<T> *insert_after(Node<T> *&head, Node<T> *after, const T &item) {
    Node<T> *insert = new Node<T>(item);

    // when pointers are NULL, insert at head, else insert behind 'after'
    if(head == NULL || after == NULL) {
        head = insert;
    } else {
        after->_next = insert;
    }

    return insert;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Head is stored in pop and assigned to next node. Then pop is deleted.
 *
 * PRE-CONDITIONS:
 *  Node<T> *&head: target node delete
 *
 * POST-CONDITIONS:
 *  Node<T> *&head: head assigns to next node
 *
 * RETURN:
 *  Deleted node's item
 ******************************************************************************/
template <typename T>
T delete_head(Node<T> *&head) {
    // assign pop to head, copy head's item, increment head
    Node<T> *pop = head;
    T item = head->_item;
    head = head->_next;

    delete pop;

    return item;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Extract all nodes' item in specified format.
 *
 * PRE-CONDITIONS:
 *  Node<T> *&head: empty or non-empty node
 *  ostream &outs : out stream object
 *
 * POST-CONDITIONS:
 *  Extraction of items to out stream object
 *
 * RETURN:
 *  ostream object for chaining
 ******************************************************************************/
template <typename T>
std::ostream &print_list(const Node<T> *head, std::ostream &outs) {
    // print node's item until empty
    while(head != NULL) {
        outs << *head;
        head = head->_next;
    }

    outs << "|||";

    return outs;
}

}  // namespace my_node

#endif  // MY_NODE_H
