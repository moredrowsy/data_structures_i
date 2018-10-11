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
    Heap(const T& item);                        // construct with one item
    Heap(const T* list, const unsigned& size);  // construct with array list

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
    void clear();         // remove access to all items
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
    void print_tree(unsigned root, std::ostream& outs = std::cout,
                    unsigned level = 0) const;
    bool is_leaf(unsigned i) const;
    unsigned parent_index(unsigned i) const;
    unsigned left_child_index(unsigned i) const;
    unsigned right_child_index(unsigned i) const;
    unsigned big_child_index(unsigned i) const;

    // MUTATORS
    void heapDown();  // move root node down till heap structure
    void heapUp();    // move last node up till heap structure
    void swap_with_parent(unsigned i);  // swap parent/child node
    bool update();                      // update capacity and make new array
};

/*******************************************************************************
 * DESCRIPTION:
 *  Constructs Heap with an item
 *
 * PRE-CONDITIONS:
 *  const T& item: templated item
 *
 * POST-CONDITIONS:
 *  item inserted to Heap, _size and _capacity increased by 1
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
Heap<T>::Heap(const T& item) : _capacity(0), _size(0), _items(nullptr) {
    insert(item);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Constructs Heap with array of items
 *
 * PRE-CONDITIONS:
 *  const T* list       : array of templated items
 *  const unsigned& size: array size
 *
 * POST-CONDITIONS:
 *  templated items inserted to Heap, _size and _capacity increased by
 *  array size
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
Heap<T>::Heap(const T* list, const unsigned& size)
    : _capacity(0), _size(0), _items(nullptr) {
    for(unsigned i = 0; i < size; ++i) insert(list[i]);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Deallocates all items.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  memory freed
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
Heap<T>::~Heap() {
    if(_items) delete[] _items;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Copy constructor.
 *
 * PRE-CONDITIONS:
 *  const Heap<T>& src: Heap source
 *
 * POST-CONDITIONS:
 *  items from source inserted to Heap, _size and _capacity increased by
 *  array size
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
Heap<T>::Heap(const Heap<T>& src) : _capacity(0), _size(0), _items(nullptr) {
    for(unsigned i = 0; i < src._size; ++i) insert(src._items[i]);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Assignment operator.
 *
 * PRE-CONDITIONS:
 *  const Heap<T>& rhs: Heap source on right side
 *
 * POST-CONDITIONS:
 *  items from source inserted to Heap, _size and _capacity increased by
 *  array size
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
Heap<T>& Heap<T>::operator=(const Heap<T>& rhs) {
    if(this != &rhs) {
        if(_items) {
            delete[] _items;
            _items = nullptr;
            _size = _capacity = 0;
        }

        for(unsigned i = 0; i < rhs._size; ++i) insert(rhs._items[i]);
    }

    return *this;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Checks for empty Heap.
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
bool Heap<T>::empty() const {
    return _size == 0;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Access Heap's capacity.
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
unsigned Heap<T>::capacity() const {
    return _capacity;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Access Heap's size.
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
unsigned Heap<T>::size() const {
    return _size;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Access Heap's array of items.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  const T*: pointer to array of items
 ******************************************************************************/
template <typename T>
const T* Heap<T>::items() const {
    return _items;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Check Heap for valid heap structure
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

/*******************************************************************************
 * DESCRIPTION:
 *  Remove access to all items.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  _size = 0.
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void Heap<T>::clear() {
    _size = 0;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Insert an item.
 *
 * PRE-CONDITIONS:
 *  T item: templated item
 *
 * POST-CONDITIONS:
 *  item added to Heap
 *
 * RETURN:
 *  boolean: insertion success/failure
 ******************************************************************************/
template <typename T>
bool Heap<T>::insert(T item) {
    bool is_good = update();

    if(is_good) {  // add item/process heap structure if update is good
        ++_size;
        _items[_size - 1] = item;

        if(_size > 1) heapUp();
    }

    return is_good;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Remove an item at index 0. Exception is thrown when pop on empty.
 *
 * PRE-CONDITIONS:
 *  non-empty Heap
 *
 * POST-CONDITIONS:
 *  item removed from Heap
 *
 * RETURN:
 *  T: templated item removed from Heap
 ******************************************************************************/
template <typename T>
T Heap<T>::pop() {
    if(empty()) throw std::range_error("Heap::pop() on size = 0");

    T pop = _items[0];
    _items[0] = _items[_size - 1];
    --_size;

    heapDown();

    return pop;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Outstream heap structure.
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
void Heap<T>::print_tree(std::ostream& outs) const {
    print_tree(0, outs);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Outstream heap structure recursively starting at index 0 until size.
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
void Heap<T>::print_tree(unsigned root, std::ostream& outs,
                         unsigned level) const {
    if(root >= _size) {
        outs << std::string(10 * level, ' ') << "|||" << std::endl;
        return;
    }

    print_tree(right_child_index(root), outs, level + 1);
    outs << std::string(10 * level, ' ') << _items[root] << std::endl;
    print_tree(left_child_index(root), outs, level + 1);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Check if index is leaf.
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
bool Heap<T>::is_leaf(unsigned i) const {
    return (i * 2) + 2 > _size;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Find parent's index with current index. Return 0 when current index is 0.
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
unsigned Heap<T>::parent_index(unsigned i) const {
    return (static_cast<int>(i) - 1) / 2;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Find left child's index with current index.
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
unsigned Heap<T>::left_child_index(unsigned i) const {
    return i * 2 + 1;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Find right child's index with current index.
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
unsigned Heap<T>::right_child_index(unsigned i) const {
    return i * 2 + 2;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Find index of the largest of the two children.
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
unsigned Heap<T>::big_child_index(unsigned i) const {
    return _items[left_child_index(i)] < _items[right_child_index(i)]
               ? right_child_index(i)
               : left_child_index(i);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Move item at top down until item is less than parent. Move down to the
 *  larger of children's path.
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
void Heap<T>::heapDown() {
    unsigned parent = 0, child = 0;
    if(!is_leaf(parent)) child = big_child_index(parent);

    while(!is_leaf(parent) && _items[parent] < _items[child]) {
        swap_with_parent(child);

        parent = child;
        if(!is_leaf(parent)) child = big_child_index(parent);
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Move item at last leaf up until item is less than parent path.
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
void Heap<T>::heapUp() {
    unsigned child = _size - 1, parent = parent_index(child);

    while(_items[child] > _items[parent]) {
        swap_with_parent(child);

        child = parent;
        parent = parent_index(child);
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Swap item at current index with parent's. Does nothing if current index
 *  is zero.
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
void Heap<T>::swap_with_parent(unsigned i) {
    if(i) {
        T parent = _items[parent_index(i)];
        _items[parent_index(i)] = _items[i];
        _items[i] = parent;
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  If size is equal or greater than capacity, double capacity and allocate
 *  new array of items.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  boolan: allocation success/failure
 ******************************************************************************/
template <typename T>
bool Heap<T>::update() {
    bool is_good = true;

    if(_size >= _capacity) {  // update capacity and create new array
        _capacity = _capacity > 0 ? _capacity * 2 : 1;

        T* old_items = _items;
        _items = new(std::nothrow) T[_capacity];

        if(_items) {  // allocation success return non-nullptr
            for(unsigned i = 0; i < _size; ++i) _items[i] = old_items[i];

            delete[] old_items;
        } else {  // allocation fails return nullptr
            _items = old_items;
            is_good = false;
        }
    }

    return is_good;
}

}  // namespace heap

#endif  // HEAP_H
