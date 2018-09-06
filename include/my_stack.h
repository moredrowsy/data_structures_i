#ifndef MY_STACK_H
#define MY_STACK_H

#include <cassert>
#include "../include/my_node.h"

namespace my_stack {

template <typename T>
class Stack {
public:
    Stack() : _top(NULL) {}

    ~Stack();
    Stack(const Stack<T>& other);
    Stack<T>& operator=(const Stack<T>& rhs);

    void push(T item);
    T pop();
    T top() const;
    bool empty() const;

    friend std::ostream& operator<<(std::ostream& outs, const Stack<T>& s) {
        return print_list(s._top, outs);
    }

private:
    my_node::node<T>* _top;
};

template <typename T>
Stack<T>::~Stack() {
    // delete_all deallocates all nodes when not empty
    delete_all(_top);
}

template <typename T>
Stack<T>::Stack(const Stack<T>& other) {
    // initialize, else copy_list fails
    init_head(_top);

    // call copy_list using 'other' as base to 'this' _top
    copy_list(other._top, _top);
}

template <typename T>
Stack<T>& Stack<T>::operator=(const Stack<T>& rhs) {
    // copy_list when not same
    if(this != &rhs) {
        copy_list(rhs._top, _top);
    }

    return *this;
}

template <typename T>
void Stack<T>::push(T item) {
    // insert @ top
    _top = insert_head(_top, item);
}

template <typename T>
T Stack<T>::pop() {
    // assert not empty list
    assert(!empty());

    return delete_head(_top);
}

template <typename T>
T Stack<T>::top() const {
    // assert not empty list
    assert(!empty());

    return _top->_item;
}

template <typename T>
bool Stack<T>::empty() const {
    return my_node::empty(_top);
}

}  // namespace my_stack

#endif  // MY_STACK_H
