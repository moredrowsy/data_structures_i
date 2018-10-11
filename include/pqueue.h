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
    Info() {}
    Info(const T& i, int p = 0) : _item(i), _priority(p) {}

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
    PQueue() {}
    PQueue(const T& i, int p = 0) : _heap(Info<T>(i, p)) {}
    PQueue(const Info<T>& info) : _heap(info) {}
    PQueue(const Info<T>* list, unsigned size);

    // ACCESSORS
    bool empty() const;
    int size() const;
    void print_tree() const;
    bool validate() const;  // valide heap structure

    // MUTATORS
    void clear();
    bool insert(const Info<T>& info);
    bool insert(const T& value, int p);
    T pop();

    // FRIENDS
    friend std::ostream& operator<<(std::ostream& outs,
                                    const PQueue<T>& print_me) {
        return outs << print_me._heap;
    }

private:
    heap::Heap<Info<T> > _heap;
};

template <typename T>
PQueue<T>::PQueue(const Info<T>* list, unsigned size) {
    _heap = heap::Heap<Info<T> >(list, size);
}

template <typename T>
bool PQueue<T>::empty() const {
    return _heap.empty();
}

template <typename T>
int PQueue<T>::size() const {
    return _heap.size();
}

template <typename T>
void PQueue<T>::print_tree() const {
    std::cout << _heap;
}

template <typename T>
bool PQueue<T>::validate() const {
    return _heap.validate();
}

template <typename T>
void PQueue<T>::clear() {
    _heap.clear();
}

template <typename T>
bool PQueue<T>::insert(const Info<T>& info) {
    return _heap.insert(info);
}

template <typename T>
bool PQueue<T>::insert(const T& value, int p) {
    return _heap.insert(Info<T>(value, p));
}

template <typename T>
T PQueue<T>::pop() {
    return _heap.pop()._item;
}

}  // namespace pqueue

#endif  // PQUEUE_H
