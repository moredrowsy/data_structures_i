/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : MY_STACK
 * DESCRIPTION : This header defines a templated Stack and its associated
 *      functions. The Stack will pop and push at the front.
 ******************************************************************************/
#ifndef MY_STACK_H
#define MY_STACK_H

#include <cassert>
#include "../include/my_node.h"

namespace my_stack {

template <typename T>
class Stack {
public:
    // CONSTRUCTORS
    Stack() : _top(NULL) {}

    // BIG THREE
    ~Stack();
    Stack(const Stack<T>& other);              // make deep copy
    Stack<T>& operator=(const Stack<T>& rhs);  // make deep copy

    // ACCESSORS
    T top() const;       // return top's item
    bool empty() const;  // check boolean for top is NULL

    // MUTATORS
    void push(T item);  // add item at top
    T pop();            // remove top and return old top's item

    // FRIENDS
    friend std::ostream& operator<<(std::ostream& outs, const Stack<T>& s) {
        return my_node::print_list(s._top, outs);  // return output
    }

private:
    my_node::Node<T>* _top;  // pointer to front
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
 *  _top: assigns to NULL when all nodes deleted successfully
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
Stack<T>::~Stack() {
    // delete_all deallocates all nodes when not empty
    my_node::delete_all(_top);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Copy constructor. Initialize top to NULL and calls copy_list to make deep
 *  copy of list.
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
    // initialize, else copy_list fails
    my_node::init_head(_top);

    // make deep copy
    my_node::copy_list(other._top, _top);
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
        my_node::copy_list(rhs._top, _top);
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
    // assert not empty list
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
    return my_node::empty(_top);
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
    // insert @ top
    _top = my_node::insert_head(_top, item);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Assert not empty. When not empty, delete front node and update top.
 *
 * PRE-CONDITIONS:
 *  not empty
 *
 * POST-CONDITIONS:
 *  _top: assigns to next node, assigns to NULL when list becomes empty
 *        after deletion
 *
 * RETURN:
 *  T item from deleted top
 ******************************************************************************/
template <typename T>
T Stack<T>::pop() {
    // assert not empty list
    assert(!empty());

    return my_node::delete_head(_top);
}

}  // namespace my_stack

#endif  // MY_STACK_H
