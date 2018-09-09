/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : MY_NODE
 * DESCRIPTION : This header defines a templated node and templated functions
 *      that will process the nodes. These functions are the base to build
 *      higher level linked lists.
 ******************************************************************************/
#ifndef MY_NODE_H
#define MY_NODE_H

#include <iostream>

namespace my_node {

template <typename T>
struct node {
    T _item;                                         // templated item
    node<T> *_next;                                  // pointer to next node
    node(const T &item = T(), node<T> *next = NULL)  // constructor
        : _item(item), _next(next) {}
    friend std::ostream &operator<<(std::ostream &outs,         // oustream
                                    const node<T> &print_me) {  // print node
        outs << "[" << print_me._item << "]->";
        return outs;  // return out stream with node's item
    }
};

// initializes head to NULL
template <typename T>
node<T> *init_head(node<T> *&head);

// deletes all the nodes in the list
template <typename T>
void delete_all(node<T> *&head);

// true if head is NULL, false otherwise.
template <typename T>
bool empty(const node<T> *head);

// makes a copy of the list, returns a pointer to the last node:
template <typename T>
node<T> *copy_list(const node<T> *head, node<T> *&cpy);

// insert at the beginning of the list:
template <typename T>
node<T> *insert_head(node<T> *&head, T item);

// insert_after: if after is NULL, inserts at head
template <typename T>
node<T> *insert_after(node<T> *&head, node<T> *after, const T &item);

// delete the node at the head of the list, return the item:
template <typename T>
T delete_head(node<T> *&head);

// print the list and return outs
template <typename T>
std::ostream &print_list(const node<T> *head, std::ostream &outs = std::cout);

template <typename T>
node<T> *init_head(node<T> *&head) {
    return head = NULL;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Deallocates all nodes linked to the head node.
 *
 * PRE-CONDITIONS:
 *  node<T> *&head: templated node
 *
 * POST-CONDITIONS:
 *  node<T> *&head: assigns to NULL when all nodes deleted successfully
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void delete_all(node<T> *&head) {
    node<T> *pop = NULL;

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
 *  node<T> *&head: templated node
 *
 * POST-CONDITIONS:
 *  node<T> *&head: assigns to NULL
 *
 * RETURN:
 *  Boolean condition.
 ******************************************************************************/
template <typename T>
bool empty(const node<T> *head) {
    return head == NULL;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Deletes all nodes linked to cpy and linked to a new copy of list.
 *
 * PRE-CONDITIONS:
 *  node<T> *&head: source node to copy from
 *  node<T> *&cpy : target node to copy to
 *
 * POST-CONDITIONS:
 *  node<T> *&cpy : old values deleted and to new copy of list
 *
 * RETURN:
 *  Pointer to last new node.
 ******************************************************************************/
template <typename T>
node<T> *copy_list(const node<T> *head, node<T> *&cpy) {
    // delete all nodes when target cpy is not empty
    if(cpy != NULL) {
        delete_all(cpy);
    }

    // initialize walker for cpy
    node<T> *copy_walker = NULL;

    // copy the rest of the nodes
    while(head != NULL) {
        // copy & assign first node to cpy, then copy the rest via walker
        if(cpy == NULL) {
            copy_walker = cpy = new node<T>(head->_item);
            head = head->_next;
        } else {
            copy_walker->_next = new node<T>(head->_item);
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
 *  node<T> *&head: target node to insert
 *  T item        : templated item to copy into new node's item
 *
 * POST-CONDITIONS:
 *  New node is added before head
 *
 * RETURN:
 *  Pointer to new node.
 ******************************************************************************/
template <typename T>
node<T> *insert_head(node<T> *&head, T item) {
    // assign head to new node with T and next pointer as old head
    return head = new node<T>(item, head);
}

/*******************************************************************************
 * DESCRIPTION:
 *  New node is created with param of item to T, and next pointer NULL. When
 *  head or after is NULL, insert at head or when after is not NULL, insert
 *  behind after.
 *
 * PRE-CONDITIONS:
 *  node<T> *&head: target node to insert when head or after is NULL
 *  node<T> *after: target node to insert when after is not NULL
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
node<T> *insert_after(node<T> *&head, node<T> *after, const T &item) {
    node<T> *insert = new node<T>(item);

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
 *  node<T> *&head: target node delete
 *
 * POST-CONDITIONS:
 *  node<T> *&head: head assigns to next node
 *
 * RETURN:
 *  Deleted node's item
 ******************************************************************************/
template <typename T>
T delete_head(node<T> *&head) {
    // assign pop to head, copy head's item, increment head
    node<T> *pop = head;
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
 *  node<T> *&head: empty or non-empty node
 *  ostream &outs : out stream object
 *
 * POST-CONDITIONS:
 *  Extraction of items to out stream object
 *
 * RETURN:
 *  ostream object for chaining
 ******************************************************************************/
template <typename T>
std::ostream &print_list(const node<T> *head, std::ostream &outs) {
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
