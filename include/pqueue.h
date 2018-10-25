/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : pqueue
 * DESCRIPTION : This header defines a templated Info struct and Priority Queue
 *      class. The Info represents an item with a given priority. The PQueue
 *      composes from Info and Heap to implement a queue with the max priority
 *      item on top.
 ******************************************************************************/
#ifndef PQUEUE_H
#define PQUEUE_H

#include <iostream>  // stream objects
#include "heap.h"    // Heap class

namespace pqueue {

template <typename T>
struct Info {
    T _item;
    int _priority;

    // CONSTRUCTORS
    Info(const T& i = T(), int p = 0) : _item(i), _priority(p) {}

    // FRIENDS
    friend std::ostream& operator<<(std::ostream& outs,
                                    const Info<T>& print_me) {
        return outs << "<" << print_me._item << ", " << print_me._priority
                    << ">";
    }

    friend bool operator<(const Info<T>& lhs, const Info<T>& rhs) {
        return lhs._priority < rhs._priority;
    }

    friend bool operator>(const Info<T>& lhs, const Info<T>& rhs) {
        return lhs._priority > rhs._priority;
    }

    friend bool operator<=(const Info<T>& lhs, const Info<T>& rhs) {
        return lhs._priority <= rhs._priority;
    }

    friend bool operator>=(const Info<T>& lhs, const Info<T>& rhs) {
        return lhs._priority >= rhs._priority;
    }
};

template <typename T>
class PQueue {
public:
    // CONSTRUCTORS
    PQueue(bool r = false) : _heap(r) {}  // r = sorting reverse
    PQueue(const T& i, int p = 0, bool r = false) : _heap(Info<T>(i, p), r) {}
    PQueue(const Info<T>& info, bool r = false) : _heap(info, r) {}
    PQueue(const Info<T>* list, unsigned size, bool r = false)
        : _heap(list, size, r) {}

    // ACCESSORS
    bool empty() const;
    unsigned size() const;
    void print_tree() const;
    bool validate() const;  // valide heap structure

    // MUTATORS
    void clear();
    bool insert(const Info<T>& info);
    bool insert(const T& value, int p);
    T pop();
    Info<T> pop_debug();
    bool reserve(unsigned n);
    // set the comparison function
    void set_comp(bool (*cmp)(const T& left, const T& right));
    void set_reverse(bool reverse);  // set heap's ordering when empty

    // FRIENDS
    friend std::ostream& operator<<(std::ostream& outs,
                                    const PQueue<T>& print_me) {
        return outs << print_me._heap;
    }

private:
    heap::Heap<Info<T> > _heap;
};

/*******************************************************************************
 * DESCRIPTION:
 *  Checks for empty PQueue.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  boolean
 ******************************************************************************/
template <typename T>
bool PQueue<T>::empty() const {
    return _heap.empty();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Access PQueue's size.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  unsigned
 ******************************************************************************/
template <typename T>
unsigned PQueue<T>::size() const {
    return _heap.size();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Console output PQueue's info items.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  console output
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void PQueue<T>::print_tree() const {
    std::cout << _heap;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Check PQueue's valid heap structure.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  boolean
 ******************************************************************************/
template <typename T>
bool PQueue<T>::validate() const {
    return _heap.validate();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Remove access all items in Heap.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  All Info items removed from PQueue's heap
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void PQueue<T>::clear() {
    _heap.clear();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Insert an Info item.
 *
 * PRE-CONDITIONS:
 *  const Info<T>& info: Info of T value and int priority
 *
 * POST-CONDITIONS:
 *  Info added to Heap
 *
 * RETURN:
 *  boolean: insertion success/failure
 ******************************************************************************/
template <typename T>
bool PQueue<T>::insert(const Info<T>& info) {
    return _heap.insert(info);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Insert an Info item with T value and int priority.
 *
 * PRE-CONDITIONS:
 *  const T& value: templated item
 *  int p         : priority of templated item
 *
 * POST-CONDITIONS:
 *  Info added to Heap
 *
 * RETURN:
 *  boolean: insertion success/failure
 ******************************************************************************/
template <typename T>
bool PQueue<T>::insert(const T& value, int p) {
    return _heap.insert(Info<T>(value, p));
}

/*******************************************************************************
 * DESCRIPTION:
 *  Remove an item at index 0. Exception is thrown from Heap when pop on empty.
 *
 * PRE-CONDITIONS:
 *  non-empty PQueue
 *
 * POST-CONDITIONS:
 *  Info removed from Heap
 *
 * RETURN:
 *  T: templated value of Info removed from Heap
 ******************************************************************************/
template <typename T>
T PQueue<T>::pop() {
    return _heap.pop()._item;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Remove an item at index 0. Exception is thrown from Heap when pop on empty.
 *
 * PRE-CONDITIONS:
 *  non-empty PQueue
 *
 * POST-CONDITIONS:
 *  Info removed from Heap
 *
 * RETURN:
 *  Info<T>: templated Info removed from Heap
 ******************************************************************************/
template <typename T>
Info<T> PQueue<T>::pop_debug() {
    return _heap.pop();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Reserve capacity of PQueue's _heap by at least n items.
 *
 * PRE-CONDITIONS:
 *  unsigned n: ensure Heap's _capcity with at least n items
 *
 * POST-CONDITIONS:
 *  Heap's _items allocation if n > Heap's capacity
 *
 * RETURN:
 *  boolan: allocation success/failure
 ******************************************************************************/
template <typename T>
bool PQueue<T>::reserve(unsigned n) {
    return _heap.reserve(n);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Set the comparison function for the Heap when empty!
 *
 * PRE-CONDITIONS:
 *  bool (*cmp)(const T& left, const T& right): comparison function
 *
 * POST-CONDITIONS:
 *  _cmp = cmp
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void PQueue<T>::set_comp(bool (*cmp)(const T& left, const T& right)) {
    _heap.set_comp(cmp);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Set the heap's sort ordering. Must be set when heap's size is 0!
 *
 * PRE-CONDITIONS:
 *  bool reverse: true = max ordering, false = min ordering
 *
 * POST-CONDITIONS:
 *  _items allocation if n > _capacity
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void PQueue<T>::set_reverse(bool reverse) {
    _heap.set_reverse(reverse);
}

}  // namespace pqueue

#endif  // PQUEUE_H
