#ifndef LIST_H
#define LIST_H

#include <cassert>            // assertions
#include "../include/node.h"  // Node class

namespace list {

template <typename T>
class List {
public:
    class Iterator {
    public:
        friend class List;  // give access to list to access _ptr

        // CONSTRUCTORS
        Iterator(node::Node<T> *p = nullptr) : _ptr(p) {}

        // ACCESSORS
        bool is_null() const { return _ptr == nullptr; }
        operator bool() const { return _ptr != nullptr; }  // explicit bool conv

        // MUTATORS
        T &operator*() { return _ptr->_item; }    // deference
        T *operator->() { return &_ptr->_item; }  // member access
        Iterator &operator++() {                  // pre-inc
            _ptr = _ptr->_next;
            return *this;
        }

        friend bool operator!=(const Iterator &lhs, const Iterator &rhs) {
            return lhs._ptr != rhs._ptr;
        }

        friend bool operator==(const Iterator &lhs, const Iterator &rhs) {
            return lhs._ptr == rhs._ptr;
        }

        friend Iterator operator++(Iterator &it, int u) {  // post-inc
            List<T>::Iterator t = &it;
            it._ptr = it._ptr._next;
            return t;
        }

    private:
        node::Node<T> *_ptr;  // pointer being encapsulated
    };

    // CONSTRUCTORS
    List() : _head(nullptr), _tail(nullptr) {}

    // BIG THREE
    ~List();
    List(const List<T> &src);                // make deep copy
    List<T> &operator=(const List<T> &rhs);  // make deep copy

    // ACCESSORS
    Iterator begin() const;                  //  iterator to head
    Iterator end() const;                    //  iterator after tail
    Iterator last_node() const;              //  iterator to tail
    Iterator search(const T &target) const;  //  search for target
    bool search(const T &target, Iterator &prev, Iterator &found) const;
    T front() const;     //  head's item
    T back() const;      //  tail's item
    bool empty() const;  // check empty list

    // MUTATORS
    Iterator insert_head(const T &item);                     // at head
    Iterator insert_after(const T &item, Iterator marker);   // before marker
    Iterator insert_before(const T &item, Iterator marker);  // after marker
    void push_front(const T &item);            // add item before head
    void push_back(const T &item);             // add item behind tail
    void clear();                              // remove all items
    T pop_front();                             // remove head
    bool remove(const T &target);              // remove node with target
    T remove(Iterator prev, Iterator marker);  // delete with prev and marker
    T remove(Iterator marker);                 // delete node pointed by marker
    T &operator[](int i);                      // item at position index

    // FRIENDS
    friend std::ostream &operator<<(std::ostream &outs, const List<T> &l) {
        return node::print_list(l._head, outs);  // return output
    }

    // rivate:
    node::Node<T> *_head;  // pointer to front
    node::Node<T> *_tail;  // pointer to back
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
 *  _head: assigns to nullptr when all nodes deleted successfully
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
List<T>::~List() {
    node::delete_all(_head);  // deallocates all nodes when not empty
}

/*******************************************************************************
 * DESCRIPTION:
 *  Copy constructor. Initialize head to nullptr and calls copy_list to make
 *  deep copy of list.
 *
 * PRE-CONDITIONS:
 *  List<T> &src: source to copy from
 *
 * POST-CONDITIONS:
 *  _head: assigns to first new node from source 'src'
 *  _tail: assigns to last new node from source 'src'
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
List<T>::List(const List<T> &src) {
    node::init_head(_head);  // initialize, else copy_list fails

    // make deep copy and update tail to last new node
    _tail = node::copy_list(src._head, _head);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Assignment operator. Calls copy_list to make deep copy of list.
 *
 * PRE-CONDITIONS:
 *  List<T> &rhs: source to copy from
 *
 * POST-CONDITIONS:
 *  _head: assigns to first new node from source 'src'
 *  _tail: assigns to last new node from source 'src'
 *
 * RETURN:
 *  self
 ******************************************************************************/
template <typename T>
List<T> &List<T>::operator=(const List<T> &rhs) {
    // make deep copy and update tail to last new node
    if(this != &rhs) _tail = node::copy_list(rhs._head, _head);

    return *this;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Points to head.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  List<T>::Iterator: pointer to first element
 ******************************************************************************/
template <typename T>
typename List<T>::Iterator List<T>::begin() const {
    return List<T>::Iterator(_head);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Points after tail == nullptr.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  List<T>::Iterator: iterator points to nullptr
 ******************************************************************************/
template <typename T>
typename List<T>::Iterator List<T>::end() const {
    return List<T>::Iterator(nullptr);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Points to tail.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  List<T>::Iterator: iterator points to nullptr
 ******************************************************************************/
template <typename T>
typename List<T>::Iterator List<T>::last_node() const {
    return List<T>::Iterator(_tail);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Search for templated item and return Iterator pointing to it. If not found,
 *  then points to nullptr.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  List<T>::Iterator: iterator points item; else nullptr
 ******************************************************************************/
template <typename T>
typename List<T>::Iterator List<T>::search(const T &target) const {
    node::Node<T> *walker = _head, *found = nullptr;

    while(walker) {
        if(walker->_item == target) {
            found = walker;
            break;
        }

        walker = walker->_next;
    }

    return List<T>::Iterator(found);
}

template <typename T>
bool List<T>::search(const T &target, Iterator &prev, Iterator &found) const {
    bool is_found = false;
    node::Node<T> *walker = _head;

    while(walker) {
        if(walker->_item == target) {  // when target @ head
            found = List<T>::Iterator(walker);
            prev = List<T>::Iterator();
            is_found = true;

            break;
        }

        // when target @ walker->_next
        if(walker->_next && walker->_next->_item == target) {
            found = List<T>::Iterator(walker->_next);
            prev = List<T>::Iterator(walker);
            is_found = true;

            break;
        }

        walker = walker->_next;
    }

    return is_found;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Assert not empty. When not empty, return first item.
 *
 * PRE-CONDITIONS:
 *  not empty
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  const T &item from front
 ******************************************************************************/
template <typename T>
T List<T>::front() const {
    assert(!empty());

    return _head->_item;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Assert not empty. When not empty, return last item.
 *
 * PRE-CONDITIONS:
 *  not empty
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  const T &item from back
 ******************************************************************************/
template <typename T>
T List<T>::back() const {
    assert(!empty());

    return _tail->_item;
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
bool List<T>::empty() const {
    return node::empty(_head);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Add new node with item before head and returns Iterator at head.
 *
 * PRE-CONDITIONS:
 *  const T &item: tempalted item for new node's item
 *
 * POST-CONDITIONS:
 *  _head: assigns to new node
 *  _tail: assigns to new node whe empty
 *
 * RETURN:
 *  List<T>::Iterator: points to new node at head
 ******************************************************************************/
template <typename T>
typename List<T>::Iterator List<T>::insert_head(const T &item) {
    if(empty())  // update _tail when empty
        _head = _tail = node::insert_head(_head, item);
    else
        _head = node::insert_head(_head, item);

    return List<T>::Iterator(_head);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Add new node with item after marker
 *
 * PRE-CONDITIONS:
 *  const T &item: tempalted item for new node's item
 *
 * POST-CONDITIONS:
 *  _head: assigns to new node
 *  _tail: assigns to new node whe empty
 *
 * RETURN:
 *  List<T>::Iterator: points to new node after marker
 ******************************************************************************/
template <typename T>
typename List<T>::Iterator List<T>::insert_after(const T &item,
                                                 Iterator marker) {
    node::Node<T> *new_node = node::insert_after(_head, marker._ptr, item);

    if(marker._ptr == _tail) _tail = new_node;

    return List<T>::Iterator(new_node);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Add new node with item before marker
 *
 * PRE-CONDITIONS:
 *  const T &item: tempalted item for new node's item
 *
 * POST-CONDITIONS:
 *  _head: assigns to new node
 *  _tail: assigns to new node whe empty
 *
 * RETURN:
 *  List<T>::Iterator: points to new node before marker
 ******************************************************************************/
template <typename T>
typename List<T>::Iterator List<T>::insert_before(const T &item,
                                                  Iterator marker) {
    node::Node<T> *walker = _head, *new_node = nullptr;

    if(walker == marker._ptr) {  // when marker @ head
        new_node = node::insert_head(_head, item);
        if(!_tail) _tail = new_node;
    } else {
        while(walker->_next != marker._ptr) {  // find previous node
            walker = walker->_next;
        }

        // insert after previous node to marker
        new_node = node::insert_after(_head, walker, item);
    }

    return List<T>::Iterator(new_node);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Add new node with item before head.
 *
 * PRE-CONDITIONS:
 *  const T &item: tempalted item for new node's item
 *
 * POST-CONDITIONS:
 *  _head: assigns to new node
 *  _tail: assigns to new node whe empty
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void List<T>::push_front(const T &item) {
    if(empty())  // update _tail when empty
        _tail = node::insert_head(_head, item);
    else
        node::insert_head(_head, item);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Add new node with item at front when empty or at back when not.
 *
 * PRE-CONDITIONS:
 *  const T &item: tempalted item for new node's item
 *
 * POST-CONDITIONS:
 *  _head: assigns to new node when empty
 *  _tail: assigns to new node whe empty or not empty
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void List<T>::push_back(const T &item) {
    _tail = node::insert_after(_head, _tail, item);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Delete all linked nodes.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void List<T>::clear() {
    return node::delete_all(_head);
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
 *  _tail: assigns to nullptr when list becomes empty after deletion
 *
 * RETURN:
 *  const T &item from deleted head
 ******************************************************************************/
template <typename T>
T List<T>::pop_front() {
    assert(!empty());

    T item = node::delete_head(_head);

    // assign tail to nullptr when list becomes empty after delete_head
    if(empty()) _tail = nullptr;

    return item;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Remove node at target.
 *
 * PRE-CONDITIONS:
 *  not empty
 *
 * POST-CONDITIONS:
 *  _head: is updated if removed
 *  _tail: is updated if removed
 *
 * RETURN:
 *  bool: remove success/failure
 ******************************************************************************/
template <typename T>
bool List<T>::remove(const T &target) {
    bool is_removed = false;
    node::Node<T> *walker = _head, *pop = nullptr;

    while(walker) {
        if(walker->_item == target) {  // when target @ head
            pop_front();
            is_removed = true;

            break;
        }

        // when target @ walker->_next
        if(walker->_next && walker->_next->_item == target) {
            pop = walker->_next;
            walker->_next = walker->_next->_next;

            if(pop == _tail) _tail = walker;
            delete pop;
            is_removed = true;

            break;
        }

        walker = walker->_next;
    }

    return is_removed;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Remove node pointed by marker with previous marker. Target marker can not
 *  be nullptr
 *
 * PRE-CONDITIONS:
 *  Iterator prev   : previous node point by marker
 *  Iterator marker : node point by marker; marker != nullptr
 *
 * POST-CONDITIONS:
 *  _head: is updated if removed
 *  _tail: is updated if removed
 *
 * RETURN:
 *  T: templated item that was removed
 ******************************************************************************/
template <typename T>
T List<T>::remove(Iterator prev, Iterator marker) {
    node::Node<T> *pop = marker._ptr;
    T target = marker._ptr->_item;

    marker._ptr = marker._ptr->_next;

    if(prev) prev._ptr->_next = marker._ptr;
    if(pop == _head) _head = marker._ptr;
    if(pop == _tail) _tail = prev._ptr;

    delete pop;

    return target;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Remove node pointed by marker.
 *
 * PRE-CONDITIONS:
 *  not empty
 *
 * POST-CONDITIONS:
 *  _head: is updated if removed
 *  _tail: is updated if removed
 *
 * RETURN:
 *  T: templated item that was removed
 ******************************************************************************/
template <typename T>
T List<T>::remove(Iterator marker) {
    node::Node<T> *walker = _head, *pop = nullptr;
    T target;

    while(walker) {
        if(walker == marker._ptr) {  // when marker @ head
            target = pop_front();

            break;
        }

        // when marker @ walker->_next
        if(walker->_next && walker->_next == marker._ptr) {
            target = walker->_next->_item;
            pop = walker->_next;
            walker->_next = walker->_next->_next;

            if(pop == _tail) _tail = walker;
            delete pop;

            break;
        }

        walker = walker->_next;
    }

    return target;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Array operator to access List's item
 *
 * PRE-CONDITIONS:
 *  int i: >= 0 and less than list's size
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  T: templated item at index
 ******************************************************************************/
template <typename T>
T &List<T>::operator[](int i) {
    assert(i >= 0);

    int count = 0;
    node::Node<T> *walker = _head;

    while(count != i) {
        ++count;
        walker = walker->_next;

        if(!walker) throw std::range_error("Index exceeded List's size");
    }

    return walker->_item;
}

}  // namespace list

#endif  // LIST_H
