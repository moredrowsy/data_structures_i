/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : stack
 * DESCRIPTION : This header defines a templated Stack and its associated
 *      functions. The Stack will pop and push at the front.
 ******************************************************************************/
#ifndef STACK_H
#define STACK_H

#include <cassert>            // assertions
#include "../include/node.h"  // Node class

namespace stack {

template <typename T>
class Stack {
public:
    // CONSTRUCTORS
    Stack() : _top(nullptr) {}

    // BIG THREE
    ~Stack();
    Stack(const Stack<T>& other);              // make deep copy
    Stack<T>& operator=(const Stack<T>& rhs);  // make deep copy

    // ACCESSORS
    T top() const;       // return top's item
    bool empty() const;  // check boolean for top is nullptr

    // MUTATORS
    void push(T item);  // add item at top
    T pop();            // remove top and return old top's item

    // FRIENDS
    friend std::ostream& operator<<(std::ostream& outs, const Stack<T>& s) {
        return node::print_list(s._top, outs);  // return output
    }

private:
    node::Node<T>* _top;  // pointer to front
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
 *  _top: assigns to nullptr when all nodes deleted successfully
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
Stack<T>::~Stack() {
    node::delete_all(_top);  // deallocates all nodes when not empty
}

/*******************************************************************************
 * DESCRIPTION:
 *  Copy constructor. Initialize top to nullptr and calls copy_list to make
 *  deep copy of list.
 *
 * PRE-CONDITIONS:
 *  Stack<T>& other: source to copy from
 *
 * POST-CONDITIONS:
 *  _top: assigns to first new node from source 'other'
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
Stack<T>::Stack(const Stack<T>& other) {
    node::init_head(_top);              // initialize, else copy_list fails
    node::copy_list(other._top, _top);  // make deep copy
}

/*******************************************************************************
 * DESCRIPTION:
 *  Assignment operator. Calls copy_list to make deep copy of list.
 *
 * PRE-CONDITIONS:
 *  Stack<T> &rhs: source to copy from
 *
 * POST-CONDITIONS:
 *  _top : assigns to first new node from source 'other'
 *
 * RETURN:
 *  self
 ******************************************************************************/
template <typename T>
Stack<T>& Stack<T>::operator=(const Stack<T>& rhs) {
    // copy_list when not same
    if(this != &rhs) {
        node::copy_list(rhs._top, _top);
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
T Stack<T>::top() const {
    assert(!empty());

    return _top->_item;
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
bool Stack<T>::empty() const {
    return node::empty(_top);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Add new node with item at front.
 *
 * PRE-CONDITIONS:
 *  T item: tempalted item for new node's item
 *
 * POST-CONDITIONS:
 *  _top : assigns to new node
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void Stack<T>::push(T item) {
    _top = node::insert_head(_top, item);  // insert @ top
}

/*******************************************************************************
 * DESCRIPTION:
 *  Assert not empty. When not empty, delete front node and update top.
 *
 * PRE-CONDITIONS:
 *  not empty
 *
 * POST-CONDITIONS:
 *  _top: assigns to next node, assigns to nullptr when list becomes empty
 *        after deletion
 *
 * RETURN:
 *  T item from deleted top
 ******************************************************************************/
template <typename T>
T Stack<T>::pop() {
    assert(!empty());

    return node::delete_head(_top);
}

}  // namespace stack

#endif  // STACK_H
