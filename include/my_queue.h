/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : MY_QUEUE
 * DESCRIPTION : This header defines a templated Queue and its associated
 *      functions. The Queue will pop items at front and push from back.
 ******************************************************************************/
#ifndef MY_QUEUE_H
#define MY_QUEUE_H

#include <cassert>               // provides assert
#include "../include/my_node.h"  // Node class

namespace my_queue {

template <typename T>
class Queue {
public:
    // CONSTRUCTORS
    Queue() : _head(NULL), _tail(NULL) {}

    // BIG THREE
    ~Queue();
    Queue(const Queue<T> &other);              // make deep copy
    Queue<T> &operator=(const Queue<T> &rhs);  // make deep copy

    // ACCESSORS
    T front() const;     // return head's item
    bool empty() const;  // check boolean for head is NULL

    // MUTATORS
    void push(T item);  // add item behind tail
    T pop();            // remove head and return old head's item

    // FRIENDS
    friend std::ostream &operator<<(std::ostream &outs, const Queue<T> &q) {
        return my_node::print_list(q._head, outs);  // return output
    }

private:
    my_node::Node<T> *_head;  // pointer to front
    my_node::Node<T> *_tail;  // pointer to back
};

/*******************************************************************************
 * DESCRIPTION:
 *  Destructor. Call delete_all to deallocate all nodes. delete_all will only
 *  delete when not empty.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  _head: assigns to NULL when all nodes deleted successfully
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
Queue<T>::~Queue() {
    // delete_all deallocates all nodes when not empty
    my_node::delete_all(_head);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Copy constructor. Initialize head to NULL and calls copy_list to make deep
 *  copy of list.
 *
 * PRE-CONDITIONS:
 *  Queue<T> &other: source to copy from
 *
 * POST-CONDITIONS:
 *  _head: assigns to first new node from source 'other'
 *  _top : assigns to last new node from source 'other'
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
Queue<T>::Queue(const Queue<T> &other) {
    // initialize, else copy_list fails
    my_node::init_head(_head);

    // make deep copy and update tail to last new node
    _tail = my_node::copy_list(other._head, _head);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Assignment operator. Calls copy_list to make deep copy of list.
 *
 * PRE-CONDITIONS:
 *  Queue<T> &rhs: source to copy from
 *
 * POST-CONDITIONS:
 *  _head: assigns to first new node from source 'other'
 *  _top : assigns to last new node from source 'other'
 *
 * RETURN:
 *  self
 ******************************************************************************/
template <typename T>
Queue<T> &Queue<T>::operator=(const Queue<T> &rhs) {
    // copy_list when not same
    if(this != &rhs) {
        _tail = my_node::copy_list(rhs._head, _head);
    }

    return *this;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Assert not empty. When not empty, return front's item.
 *
 * PRE-CONDITIONS:
 *  not empty
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  T item from front
 ******************************************************************************/
template <typename T>
T Queue<T>::front() const {
    // assert not empty list
    assert(!empty());

    return _head->_item;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Check for empty list.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  Boolean condition
 ******************************************************************************/
template <typename T>
bool Queue<T>::empty() const {
    return my_node::empty(_head);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Add new node with item at front when empty or at back when not.
 *
 * PRE-CONDITIONS:
 *  T item: tempalted item for new node's item
 *
 * POST-CONDITIONS:
 *  _head: assigns to new node when empty
 *  _top : assigns to new node whe empty or not empty
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void Queue<T>::push(T item) {
    // insert @ head when empty, else insert @ tail
    if(empty()) {
        _head = _tail = my_node::insert_head(_head, item);
    } else {
        _tail = my_node::insert_after(_head, _tail, item);
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Assert not empty. When not empty, delete front node and update head or
 *  tail pointers.
 *
 * PRE-CONDITIONS:
 *  not empty
 *
 * POST-CONDITIONS:
 *  _head: assigns to next node
 *  _top : assigns to NULL when list becomes empty after deletion
 *
 * RETURN:
 *  T item from deleted head
 ******************************************************************************/
template <typename T>
T Queue<T>::pop() {
    // assert not empty list
    assert(!empty());

    T item = my_node::delete_head(_head);

    // assign tail to NULL when list becomes empty after delete_head
    if(empty()) {
        _tail = NULL;
    }

    return item;
}

}  // namespace my_queue

#endif  // MY_QUEUE_H
