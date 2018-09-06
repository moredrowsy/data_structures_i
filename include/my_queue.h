#ifndef MY_QUEUE_H
#define MY_QUEUE_H

#include <cassert>
#include "../include/my_node.h"

namespace my_queue {

template <typename T>
class Queue {
public:
    Queue() : _head(NULL), _tail(NULL) {}

    ~Queue();
    Queue(const Queue<T> &other);
    Queue<T> &operator=(const Queue<T> &rhs);

    void push(T item);
    T pop();
    T front() const;
    bool empty() const;

    friend std::ostream &operator<<(std::ostream &outs, const Queue<T> &q) {
        return print_list(q._head, outs);
    }

private:
    my_node::node<T> *_head;
    my_node::node<T> *_tail;
};

template <typename T>
Queue<T>::~Queue() {
    // delete_all deallocates all nodes when not empty
    delete_all(_head);
}

template <typename T>
Queue<T>::Queue(const Queue<T> &other) {
    // initialize, else copy_list fails
    init_head(_head);

    // call copy_list using 'other' as base to 'this' _head
    _tail = copy_list(other._head, _head);
}

template <typename T>
Queue<T> &Queue<T>::operator=(const Queue<T> &rhs) {
    // copy_list when not same
    if(this != &rhs) {
        _tail = copy_list(rhs._head, _head);
    }

    return *this;
}

template <typename T>
void Queue<T>::push(T item) {
    // insert @ head when empty, else insert @ tail
    if(empty()) {
        _head = _tail = insert_head(_head, item);
    } else {
        _tail = insert_after(_head, _tail, item);
    }
}

template <typename T>
T Queue<T>::pop() {
    // assert not empty list
    assert(!empty());

    T item = delete_head(_head);

    // assign tail to NULL when empty list
    if(empty()) {
        _tail = NULL;
    }

    return item;
}

template <typename T>
T Queue<T>::front() const {
    // assert not empty list
    assert(!empty());

    return _head->_item;
}

template <typename T>
bool Queue<T>::empty() const {
    return my_node::empty(_head);
}

}  // namespace my_queue

#endif  // MY_QUEUE_H
