/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : heap
 * DESCRIPTION : This header defines a templated binary heap tree.
 *      Default ordering is minimum heap, else with reverse = true, max heap.
 *      Desired (min or max) item are on top of the tree and children nodes'
 *      item are less than the parent's.
 ******************************************************************************/
#ifndef HEAP_H
#define HEAP_H

#include <cassert>   // assertions
#include <iostream>  // stream objects
#include <string>    // string objects

namespace heap {

template <typename T, typename C = bool (*)(T const &, T const &)>
class Heap {
public:
    // CONSTRUCTORS
    Heap(bool reverse = false);
    Heap(C cmp);
    Heap(const T &item, bool reverse = false);
    Heap(const T &item, C cmp);
    Heap(const T *list, unsigned size, bool reverse = false);
    Heap(const T *list, unsigned size, C cmp);

    // BIG THREE
    ~Heap();
    Heap(const Heap<T, C> &src);
    Heap<T, C> &operator=(const Heap<T, C> &rhs);

    // ACCESSORS
    bool empty() const;
    unsigned capacity() const;
    unsigned size() const;
    const T *items() const;  // returns const ptr to array of items
    bool validate(unsigned parent = 0) const;  // valide heap structure

    // MUTATORS
    void clear();              // remove access to all items
    bool insert(T item);       // insert item and rearrange heap
    T pop();                   // remove top item and rearrange heap
    bool reserve(unsigned n);  // increase capacity by amount i
    // set the comparison function
    void set_comp(bool (*cmp)(const T &l, const T &r));
    void set_reverse(bool reverse);  // set heap's ordering when empty

    // FRIENDS
    friend std::ostream &operator<<(std::ostream &outs, const Heap<T, C> &h) {
        h.print_tree(outs);
        return outs;
    }

private:
    bool _reverse = false;                        // reverse sorting to max
    bool (*_cmp)(const T &left, const T &right);  // comparison function ptr
    unsigned _capacity;
    unsigned _size;
    T *_items;

    // ACCESSORS
    // min function comparison
    static bool min_cmp(const T &left, const T &right);
    static bool max_cmp(const T &left, const T &right);
    // print tree in array format
    void print_tree(std::ostream &outs = std::cout) const;
    // print tree in 90 degrees counterclockwise binary format
    void print_tree(unsigned root, std::ostream &outs = std::cout,
                    unsigned level = 0) const;
    inline bool is_leaf(unsigned i) const;
    inline unsigned parent_index(unsigned i) const;
    inline unsigned left_child_index(unsigned i) const;
    inline unsigned right_child_index(unsigned i) const;
    inline unsigned big_child_index(unsigned i) const;
    inline unsigned small_child_index(unsigned i) const;
    inline unsigned child_index(unsigned i) const;

    // MUTATORS
    inline void deallocate_and_throw();
    inline bool expand();  // update capacity and array
                           // move root node down till heap structure
    inline void heap_down(bool (*cmp)(const T &l, const T &r));
    // move last node up till heap structure
    inline void heap_up(bool (*cmp)(const T &l, const T &r));
    inline void swap_with_parent(unsigned i);  // swap parent/child node
};

/*******************************************************************************
 * DESCRIPTION:
 *  Default constructor for empty heap.
 *
 * PRE-CONDITIONS:
 *  bool reverse: default is false
 *
 * POST-CONDITIONS:
 *  bool reverse: user provided boolean for reverse sorting
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename C>
Heap<T, C>::Heap(bool reverse)
    : _reverse(reverse),
      _cmp(&min_cmp),
      _capacity(0),
      _size(0),
      _items(nullptr) {
    if(_reverse) _cmp = &max_cmp;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Constructs Heap with provided comparison function.
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
template <typename T, typename C>
Heap<T, C>::Heap(C cmp)
    : _reverse(false), _cmp(cmp), _capacity(0), _size(0), _items(nullptr) {}

/*******************************************************************************
 * DESCRIPTION:
 *  Constructs Heap with an item via insert(). If insertion fails, deallocate
 *  _items and throw exception before constructor fails.
 *
 * PRE-CONDITIONS:
 *  const T& item: templated item
 *  bool reverse : reverse sorting
 *
 * POST-CONDITIONS:
 *  item inserted to Heap, _size and _capacity increased by 1
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename C>
Heap<T, C>::Heap(const T &item, bool reverse)
    : _reverse(reverse),
      _cmp(&min_cmp),
      _capacity(0),
      _size(0),
      _items(nullptr) {
    if(_reverse) _cmp = &max_cmp;
    if(!insert(item)) deallocate_and_throw();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Constructs Heap with an item via insert(). If insertion fails, deallocate
 *  _items and throw exception before constructor fails.
 *
 * PRE-CONDITIONS:
 *  const T& item: templated item
 * bool (*cmp)   : comparision function with two params
 *
 * POST-CONDITIONS:
 *  item inserted to Heap, _size and _capacity increased by 1
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename C>
Heap<T, C>::Heap(const T &item, C cmp)
    : _reverse(false), _cmp(cmp), _capacity(0), _size(0), _items(nullptr) {
    if(!insert(item)) deallocate_and_throw();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Constructs Heap with list of items via insert(). If reserve of
 *  size fails, deallocate _items and throw exception before constructor
 *  fails.
 *
 * PRE-CONDITIONS:
 *  const T* list       : array of templated items
 *  unsigned size: array size
 *
 * POST-CONDITIONS:
 *  templated items inserted to Heap, _size and _capacity increased by
 *  array size
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename C>
Heap<T, C>::Heap(const T *list, unsigned size, bool reverse)
    : _reverse(reverse),
      _cmp(&min_cmp),
      _capacity(0),
      _size(0),
      _items(nullptr) {
    if(_reverse) _cmp = &max_cmp;

    if(!reserve(size)) deallocate_and_throw();
    for(unsigned i = 0; i < size; ++i) insert(list[i]);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Constructs Heap with list of items via insert(). If reserve of
 *  size fails, deallocate _items and throw exception before constructor
 *  fails.
 *
 * PRE-CONDITIONS:
 *  const T* list       : array of templated items
 *  unsigned size: array size
 *
 * POST-CONDITIONS:
 *  templated items inserted to Heap, _size and _capacity increased by
 *  array size
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename C>
Heap<T, C>::Heap(const T *list, unsigned size, C cmp)
    : _reverse(false), _cmp(cmp), _capacity(0), _size(0), _items(nullptr) {
    if(!reserve(size)) deallocate_and_throw();
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
template <typename T, typename C>
Heap<T, C>::~Heap() {
    delete[] _items;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Copy constructor via insert(). If reserve of source's capacity
 *  fails, deallocate _items and throw exception before constructor fails.
 *
 * PRE-CONDITIONS:
 *  const Heap<T, C>& src: Heap source
 *
 * POST-CONDITIONS:
 *  items from source inserted to Heap, _size and _capacity increased by
 *  array size
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename C>
Heap<T, C>::Heap(const Heap<T, C> &src)
    : _reverse(src._reverse),
      _cmp(src._cmp),
      _capacity(0),
      _size(0),
      _items(nullptr) {
    if(!reserve(src._capacity)) deallocate_and_throw();
    for(unsigned i = 0; i < src._size; ++i) insert(src._items[i]);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Assignment operator via insert(). If reserve of source's capacity
 *  fails, deallocate _items and throw exception before constructor fails.
 *
 * PRE-CONDITIONS:
 *  const Heap<T, C>& rhs: Heap source on right side
 *
 * POST-CONDITIONS:
 *  items from source inserted to Heap, _size and _capacity increased by
 *  array size
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename C>
Heap<T, C> &Heap<T, C>::operator=(const Heap<T, C> &rhs) {
    if(this != &rhs) {
        delete[] _items;
        _reverse = rhs._reverse;
        _cmp = rhs._cmp;
        _items = nullptr;
        _size = _capacity = 0;

        if(!reserve(rhs._capacity)) deallocate_and_throw();
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
template <typename T, typename C>
bool Heap<T, C>::empty() const {
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
template <typename T, typename C>
unsigned Heap<T, C>::capacity() const {
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
template <typename T, typename C>
unsigned Heap<T, C>::size() const {
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
template <typename T, typename C>
const T *Heap<T, C>::items() const {
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
template <typename T, typename C>
bool Heap<T, C>::validate(unsigned parent) const {
    if(parent >= _size) return true;

    bool is_valid = true;

    is_valid &= validate(right_child_index(parent));  // recurse left

    // find bitwise AND for all booleans
    if(left_child_index(parent) < _size)
        is_valid &=
            _items[parent_index(parent)] >= _items[left_child_index(parent)];
    if(right_child_index(parent) < _size)
        is_valid &=
            _items[parent_index(parent)] >= _items[right_child_index(parent)];

    is_valid &= validate(right_child_index(parent));  // recurse right

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
template <typename T, typename C>
void Heap<T, C>::clear() {
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
template <typename T, typename C>
bool Heap<T, C>::insert(T item) {
    bool is_good = expand();

    if(is_good) {  // add item/process heap structure if update is good
        ++_size;
        _items[_size - 1] = item;

        if(_size > 1) heap_up(_cmp);
    }

    return is_good;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Remove an item at index 0. Asserts non-empty Heap.
 *
 * PRE-CONDITIONS:
 *  _size > 0
 *
 * POST-CONDITIONS:
 *  item removed from Heap
 *
 * RETURN:
 *  T: templated item removed from Heap
 ******************************************************************************/
template <typename T, typename C>
T Heap<T, C>::pop() {
    assert(!empty());

    T pop = _items[0];
    _items[0] = _items[_size - 1];
    --_size;

    if(_size) heap_down(_cmp);

    return pop;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Reserve _capacity by at least n items. If array expansion fails,
 *  no exception thrown and old _items array is restored and returns false.
 *  Heap allocation uses new(std::nothrow) version and check fail via return
 *  of nullptr. Does nothing if n == 0 or n < _capacity and returns true.
 *
 * PRE-CONDITIONS:
 *  unsigned n: ensure _capcity with at least n items
 *
 * POST-CONDITIONS:
 *  _items allocation if n > _capacity
 *
 * RETURN:
 *  boolan: allocation success/failure
 ******************************************************************************/
template <typename T, typename C>
bool Heap<T, C>::reserve(unsigned n) {
    bool is_good = true;

    if(n > _capacity) {  // expand capacity and create new array
        _capacity = n;

        T *new_items = new(std::nothrow) T[_capacity];

        if(new_items) {  // allocation success return non-nullptr
            for(unsigned i = 0; i < _size; ++i) new_items[i] = _items[i];

            delete[] _items;
            _items = new_items;
        } else  // allocation fail return nullptr
            is_good = false;
    }

    return is_good;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Override the heap's internal comparison function.
 *
 * PRE-CONDITIONS:
 *   bool (*cmp)(const T &l, const T &r): comparison function
 *
 * POST-CONDITIONS:
 *  _cmp = cmp
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename C>
void Heap<T, C>::set_comp(bool (*cmp)(const T &l, const T &r)) {
    assert(empty());
    _cmp = cmp;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Set the heap's internal sort ordering. Must be set when heap is empty!
 *  Will also override user provided comparison function if set to true.
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
template <typename T, typename C>
void Heap<T, C>::set_reverse(bool reverse) {
    assert(empty());
    _reverse = reverse;
    if(_reverse) _cmp = &max_cmp;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Less than comparison
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
template <typename T, typename C>
bool Heap<T, C>::min_cmp(const T &left, const T &right) {
    return left < right;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Greater than comparison
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
template <typename T, typename C>
bool Heap<T, C>::max_cmp(const T &left, const T &right) {
    return left > right;
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
template <typename T, typename C>
void Heap<T, C>::print_tree(std::ostream &outs) const {
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
template <typename T, typename C>
void Heap<T, C>::print_tree(unsigned root, std::ostream &outs,
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
template <typename T, typename C>
bool Heap<T, C>::is_leaf(unsigned i) const {
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
template <typename T, typename C>
unsigned Heap<T, C>::parent_index(unsigned i) const {
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
template <typename T, typename C>
unsigned Heap<T, C>::left_child_index(unsigned i) const {
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
template <typename T, typename C>
unsigned Heap<T, C>::right_child_index(unsigned i) const {
    return i * 2 + 2;
}

/*******************************************************************************
 * DESCRIPTION:
 * Find index of the larger of the two children.
 *
 * PRE-CONDITIONS:
 *  unsigned i: parent's index
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  unsigned
 ******************************************************************************/
template <typename T, typename C>
unsigned Heap<T, C>::big_child_index(unsigned i) const {
    return _items[left_child_index(i)] < _items[right_child_index(i)]
               ? right_child_index(i)
               : left_child_index(i);
}

/*******************************************************************************
 * DESCRIPTION:
 * Find index of the smaller of the two children.
 *
 * PRE-CONDITIONS:
 *  unsigned i: parent's index
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  unsigned
 ******************************************************************************/
template <typename T, typename C>
unsigned Heap<T, C>::small_child_index(unsigned i) const {
    return _items[left_child_index(i)] < _items[right_child_index(i)]
               ? left_child_index(i)
               : right_child_index(i);
}

/*******************************************************************************
 * DESCRIPTION:
 * Find index of the smaller of the two children.
 *
 * PRE-CONDITIONS:
 *  unsigned i: parent's index
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  unsigned
 ******************************************************************************/
template <typename T, typename C>
unsigned Heap<T, C>::child_index(unsigned i) const {
    unsigned child = 0;

    if(!is_leaf(i)) {
        if(_cmp(0, 1))
            child = small_child_index(i);
        else
            child = big_child_index(i);
    }

    return child;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Deallocate _items and throw bad allocation exception. Use for allocation
 *  fail on constructors.
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
template <typename T, typename C>
void Heap<T, C>::deallocate_and_throw() {
    if(_items) delete[] _items;
    throw std::bad_alloc();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Expand _capacity when _size == _capacity. If array expansion fails,
 *  no exception thrown and old _items array is restored and returns false.
 *  Heap allocation uses new(std::nothrow) version and check fail via return
 *  of nullptr. Does nothing if _size != _capacity and returns true.
 *
 * PRE-CONDITIONS:
 *  _size == _capacity
 *
 * POST-CONDITIONS:
 *  _items allocation if _size == _capacity
 *
 * RETURN:
 *  boolan: allocation success/failure
 ******************************************************************************/
template <typename T, typename C>
bool Heap<T, C>::expand() {
    bool is_good = true;

    if(_size == _capacity) {  // expand capacity and create new array
        _capacity = _capacity > 0 ? _capacity * 2 : 1;

        T *new_items = new(std::nothrow) T[_capacity];

        if(new_items) {  // allocation success return non-nullptr
            for(unsigned i = 0; i < _size; ++i) new_items[i] = _items[i];

            delete[] _items;
            _items = new_items;
        } else  // allocation fail return nullptr
            is_good = false;
    }

    return is_good;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Move item at top down until child compares to parent is false.
 *  NOTE: big_child used for max cmp, while small_child for min cmp.
 *  REQUIRE _size > 0. Does nothing if size is 1.
 *
 * PRE-CONDITIONS:
 *  unsigned _size > 0
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename C>
void Heap<T, C>::heap_down(bool (*cmp)(const T &l, const T &r)) {
    assert(_size);

    unsigned parent = 0, child = child_index(parent);

    while(!is_leaf(parent) && cmp(_items[child], _items[parent])) {
        swap_with_parent(child);

        parent = child;
        child = child_index(parent);
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Move item at last leaf up until item is less than parent path.
 *  REQUIRE _size > 0. Does nothing when _size is 1.
 *
 * PRE-CONDITIONS:
 *  unsigned _size > 0
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename C>
void Heap<T, C>::heap_up(bool (*cmp)(const T &l, const T &r)) {
    assert(_size);

    unsigned child = _size - 1, parent = parent_index(child);

    while(cmp(_items[child], _items[parent])) {
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
template <typename T, typename C>
void Heap<T, C>::swap_with_parent(unsigned i) {
    if(i) {
        T parent = _items[parent_index(i)];
        _items[parent_index(i)] = _items[i];
        _items[i] = parent;
    }
}

}  // namespace heap

#endif  // HEAP_H
