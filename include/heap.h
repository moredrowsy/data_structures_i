/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : heap
 * DESCRIPTION : This header defines a templated binary MAX heap tree. Max item
 *      are on top of the tree and children nodes' item are less than the
 *      parent's.
 ******************************************************************************/
#ifndef HEAP_H
#define HEAP_H

#include <cassert>   // assertions
#include <iostream>  // stream objects
#include <string>    // string objects

namespace heap {

template <typename T>
class Heap {
public:
    // CONSTRUCTORS
    Heap() : _capacity(0), _size(0), _items(nullptr) {}
    Heap(const T& item);                 // construct with one item
    Heap(const T* list, unsigned size);  // construct Heap with array list

    // BIG THREE
    ~Heap();
    Heap(const Heap<T>& src);
    Heap<T>& operator=(const Heap<T>& rhs);

    // ACCESSORS
    bool empty() const;
    unsigned capacity() const;
    unsigned size() const;
    const T* items() const;  // returns const ptr to array of items
    bool validate(unsigned parent = 0) const;  // valide heap structure

    // MUTATORS
    void clear();         // remove all items
    bool insert(T item);  // insert item and rearrange heap
    T pop();              // remove top item and rearrange heap

    // FRIENDS
    friend std::ostream& operator<<(std::ostream& outs,
                                    const Heap<T>& print_me) {
        print_me.print_tree(outs);
        return outs;
    }

private:
    unsigned _capacity;
    unsigned _size;
    T* _items;

    // ACCESSORS
    // print tree in array format
    void print_tree(std::ostream& outs = std::cout) const;
    // print tree in 90 degrees counterclockwise binary format
    void print_tree(unsigned root, unsigned level = 0,
                    std::ostream& outs = std::cout) const;
    bool is_leaf(unsigned i) const;
    unsigned parent_index(unsigned i) const;
    unsigned left_child_index(unsigned i) const;
    unsigned right_child_index(unsigned i) const;
    unsigned big_child_index(unsigned i) const;

    // MUTATORS
    void heapDown();  // move root node down till heap structure
    void heapUp();    // move last node up till heap structure
    void swap_with_parent(unsigned i);  // swap parent/child node
    void update();                      // update capacity and make new array
};

template <typename T>
Heap<T>::Heap(const T* list, unsigned size)
    : _capacity(0), _size(0), _items(nullptr) {
    for(unsigned i = 0; i < size; ++i) insert(list[i]);
}

template <typename T>
Heap<T>::~Heap() {
    if(_items) delete[] _items;
}

template <typename T>
Heap<T>::Heap(const T& item) {
    _capacity = 1;
    _size = 1;
    _items = new T[_capacity];
    _items[0] = item;
}

template <typename T>
Heap<T>::Heap(const Heap<T>& src) {
    _capacity = src._capacity;
    _size = src._size;
    _items = new T[_capacity];

    for(unsigned i = 0; i < _size; ++i) _items[i] = src._items[i];
}

template <typename T>
Heap<T>& Heap<T>::operator=(const Heap<T>& rhs) {
    if(this != &rhs) {
        _capacity = rhs._capacity;
        _size = rhs._size;
        _items = new T[_capacity];

        for(unsigned i = 0; i < _size; ++i) _items[i] = rhs._items[i];
    }

    return *this;
}

template <typename T>
bool Heap<T>::empty() const {
    return _size == 0;
}

template <typename T>
unsigned Heap<T>::capacity() const {
    return _capacity;
}

template <typename T>
unsigned Heap<T>::size() const {
    return _size;
}

template <typename T>
const T* Heap<T>::items() const {
    return _items;
}

template <typename T>
bool Heap<T>::validate(unsigned parent) const {
    if(parent >= _size) return true;

    bool is_valid = true;

    is_valid &= validate(right_child_index(parent));  // recurve left

    // find bitwise AND for all booleans
    if(left_child_index(parent) < _size)
        is_valid &=
            _items[parent_index(parent)] >= _items[left_child_index(parent)];
    if(right_child_index(parent) < _size)
        is_valid &=
            _items[parent_index(parent)] >= _items[right_child_index(parent)];

    is_valid &= validate(right_child_index(parent));  // recurve right

    return is_valid;
}

template <typename T>
void Heap<T>::clear() {
    _size = 0;
}

template <typename T>
bool Heap<T>::insert(T item) {
    update();

    ++_size;
    _items[_size - 1] = item;

    heapUp();

    return _items != nullptr ? true : false;
}

template <typename T>
T Heap<T>::pop() {
    assert(!empty());

    T pop = _items[0];
    _items[0] = _items[_size - 1];
    --_size;

    heapDown();

    return pop;
}

template <typename T>
void Heap<T>::print_tree(std::ostream& outs) const {
    print_tree(0, 0, outs);
}

template <typename T>
void Heap<T>::print_tree(unsigned root, unsigned level,
                         std::ostream& outs) const {
    if(root >= _size) {
        outs << std::string(10 * level, ' ') << "|||" << std::endl;
        return;
    }

    print_tree(right_child_index(root), level + 1, outs);
    outs << std::string(10 * level, ' ') << _items[root] << std::endl;
    print_tree(left_child_index(root), level + 1, outs);
}

template <typename T>
bool Heap<T>::is_leaf(unsigned i) const {
    return (i * 2) + 2 > _size;
}

template <typename T>
unsigned Heap<T>::parent_index(unsigned i) const {
    return (static_cast<int>(i) - 1) / 2;
}

template <typename T>
unsigned Heap<T>::left_child_index(unsigned i) const {
    return i * 2 + 1;
}

template <typename T>
unsigned Heap<T>::right_child_index(unsigned i) const {
    return i * 2 + 2;
}

template <typename T>
unsigned Heap<T>::big_child_index(unsigned i) const {
    return _items[left_child_index(i)] < _items[right_child_index(i)]
               ? right_child_index(i)
               : left_child_index(i);
}

template <typename T>
void Heap<T>::heapDown() {
    unsigned parent = 0, child = 0;
    if(!is_leaf(parent)) child = big_child_index(parent);

    while(!is_leaf(parent) && _items[parent] < _items[child]) {
        swap_with_parent(child);

        parent = child;
        if(!is_leaf(parent)) child = big_child_index(parent);
    }
}

template <typename T>
void Heap<T>::heapUp() {
    unsigned child = _size - 1, parent = parent_index(child);

    while(_items[child] > _items[parent]) {
        swap_with_parent(child);

        child = parent;
        parent = parent_index(child);
    }
}

template <typename T>
void Heap<T>::swap_with_parent(unsigned i) {
    T parent = _items[parent_index(i)];
    _items[parent_index(i)] = _items[i];
    _items[i] = parent;
}

template <typename T>
void Heap<T>::update() {
    if(_size >= _capacity) {  // update capacity and create new array
        _capacity = _capacity > 0 ? _capacity * 2 : 1;

        T* old_items = _items;
        _items = nullptr;
        _items = new T[_capacity];

        if(_items) {
            for(unsigned i = 0; i < _size; ++i) _items[i] = old_items[i];

            delete[] old_items;
        }
    }
}

}  // namespace heap

#endif  // HEAP_H
