#ifndef BTREE_H
#define BTREE_H

#include <iomanip>  // setw()
#include "array_utils.h"

namespace btree {

template <class T>
class BTree {
public:
    // CONSTRUCTOR
    BTree(bool dups = false);

    // BIG THREE
    ~BTree();
    BTree(const BTree<T>& src);
    BTree<T>& operator=(const BTree<T>& rhs);

    // ACCESSORS
    bool contains(const T& entry) const;
    bool empty() const;
    std::size_t size() const;
    void print_tree(std::ostream& outs = std::cout, int level = 0,
                    int index = 0) const;

    // MUTATORS
    void insert(const T& entry);
    void remove(const T& entry);

    void clear_tree();
    void copy_tree(const BTree<T>& other);

    T& get(const T& entry);   // return a reference to entry in the tree
    T* find(const T& entry);  // return ptr to T; else nullptr

    // FRIENDS
    friend std::ostream& operator<<(std::ostream& outs, const BTree<T>& bt) {
        bt.print_tree(outs);
        return outs;
    }

private:
    static const std::size_t MINIMUM = 1;
    static const std::size_t MAXIMUM = 2 * MINIMUM;

    bool _dups_ok;                   // true if duplicate keys may be inserted
    std::size_t _data_count;         // number of data elements
    T _data[MAXIMUM + 1];            // holds the keys
    std::size_t _child_count;        // number of children
    BTree<T>* _subset[MAXIMUM + 2];  // subtrees

    bool is_leaf() const { return _child_count == 0; }  // true if leaf node

    // insert element functions
    bool loose_insert(const T& entry);  // allows MAXIMUM+1 data in the root
    void fix_excess(std::size_t i);  // fix excess of data elements in child i

    // remove element functions:
    void loose_remove(const T& entry);  // allows MINIMUM-1 data in the root
    void fix_shortage(int i);  // fix shortage of data elements in child i

    void remove_biggest(T& entry);  // remove the biggest child of tree->entry
    void rotate_left(int i);        // transfer one element LEFT from child i
    void rotate_right(int i);       // transfer one element RIGHT from child i
    void merge_with_next_subset(int i);  // merge subset i with subset i+1
};

template <typename T>
BTree<T>::BTree(bool dups) : _dups_ok(dups), _data_count(0), _child_count(0) {
    for(std::size_t i = 0; i < MAXIMUM + 2; ++i) _subset[i] = nullptr;
}

template <typename T>
BTree<T>::~BTree() {
    clear_tree();
}

template <typename T>
BTree<T>::BTree(const BTree<T>& src)
    : _dups_ok(src._dups_ok), _data_count(0), _child_count(0) {
    copy_tree(src);
}

template <typename T>
BTree<T>& BTree<T>::operator=(const BTree<T>& rhs) {
    if(this != &rhs) {
        _dups_ok = rhs._dups_ok;
        clear_tree();
        copy_tree(rhs);
    }

    return *this;
}

template <typename T>
bool BTree<T>::contains(const T& entry) const {
    // find index of T that's greater or qual to entry
    std::size_t i = array_utils::first_ge(_data, _data_count, entry);
    bool is_found = (i < _data_count && !(entry < _data[i]));

    if(is_found)
        return true;
    else if(is_leaf())
        return false;
    else
        return _subset[i]->contains(entry);  // recurse to find entry
}

template <typename T>
bool BTree<T>::empty() const {
    return _data_count == 0;
}

template <typename T>
std::size_t BTree<T>::size() const {
    std::size_t count = 0;

    if(!is_leaf())
        for(std::size_t i = 0; i < _child_count; ++i)
            count += _subset[i]->size();

    return count + _data_count;
}

template <typename T>
void BTree<T>::print_tree(std::ostream& outs, int level, int index) const {
    int mid = (_child_count - 1) / 2;  // store midpoint

    if(!is_leaf())  // recurse right of subtrees
        for(int i = _child_count - 1; i > mid; --i)
            _subset[i]->print_tree(outs, level + 1, i);

    // outstream data
    outs << std::setw(level * 15) << ' ' << index << " |";
    for(std::size_t i = 0; i < _data_count; ++i) {
        outs << _data[i];

        if(i != _data_count - 1) outs << ", ";
    }
    outs << "|\n";

    if(!is_leaf())  // recurse left of subtrees
        for(int i = mid; i >= 0; --i)
            _subset[i]->print_tree(outs, level + 1, i);
}

template <typename T>
void BTree<T>::insert(const T& entry) {
    loose_insert(entry);

    if(_data_count > MAXIMUM) {
        BTree<T>* new_node = new BTree<T>;  // for copying 'this' to new node

        // copy this data and subset to new node
        for(std::size_t i = 0; i < _data_count; ++i)
            new_node->_data[i] = _data[i];
        for(std::size_t i = 0; i < _child_count; ++i)
            new_node->_subset[i] = _subset[i];

        // update new node's data count and child count
        new_node->_data_count = _data_count;
        new_node->_child_count = _child_count;

        // set subset[0] to point to new node and update 'this' count
        _subset[0] = new_node;
        _data_count = 0;
        _child_count = 1;

        // fix this' only child (new node) excess
        fix_excess(0);
    }
}

template <typename T>
void BTree<T>::clear_tree() {
    if(!is_leaf()) {  // recurse into subset
        for(std::size_t i = 0; i < _child_count; ++i) {
            _subset[i]->clear_tree();
            delete _subset[i];
        }
        _child_count = 0;
    }
    _data_count = 0;
}

template <typename T>
void BTree<T>::copy_tree(const BTree<T>& other) {
    if(!other.is_leaf())
        for(std::size_t i = 0; i < other._child_count; ++i)
            copy_tree(*other._subset[i]);

    for(std::size_t i = 0; i < other._data_count; ++i) insert(other._data[i]);
}

template <typename T>
T& BTree<T>::get(const T& entry) {
    // find index of T that's greater or qual to entry
    std::size_t i = array_utils::first_ge(_data, _data_count, entry);
    bool is_found = (i < _data_count && !(entry < _data[i]));

    if(is_found)
        return _data[i];
    else if(is_leaf())
        throw std::invalid_argument("BTree::get() - invalid entry");
    else
        return _subset[i]->get(entry);
}

template <typename T>
T* BTree<T>::find(const T& entry) {
    // find index of T that's greater or qual to entry
    std::size_t i = array_utils::first_ge(_data, _data_count, entry);
    bool is_found = (i < _data_count && !(entry < _data[i]));

    if(is_found)
        return &_data[i];
    else if(is_leaf())
        return nullptr;
    else
        return _subset[i]->find(entry);
}

template <typename T>
bool BTree<T>::loose_insert(const T& entry) {
    // find index of T that's greater or qual to entry
    std::size_t i = array_utils::first_ge(_data, _data_count, entry);
    bool is_found = (i < _data_count && !(entry < _data[i]));

    if(is_found)
        return false;
    else if(is_leaf()) {
        array_utils::insert_item(_data, i, _data_count, entry);
        return true;
    } else {
        _subset[i]->loose_insert(entry);  // recurse subset to insert entry

        if(_subset[i]->_data_count > MAXIMUM)  // fix child node's over limit
            fix_excess(i);

        return true;
    }
}

template <typename T>
void BTree<T>::fix_excess(std::size_t i) {
    BTree<T>* new_node = new BTree<T>;  // to hold excess elements after mid

    // move elements after half of subset[i]'s data to new node's data
    array_utils::split(_subset[i]->_data, _subset[i]->_data_count,
                       new_node->_data, new_node->_data_count);
    --_subset[i]->_data_count;  // exclude midpoint index

    // copy half and after of subset[i]'s subset pointers to new node's subset
    array_utils::split(_subset[i]->_subset, _subset[i]->_child_count,
                       new_node->_subset, new_node->_child_count);

    // insert new node after subset[i], which is @ i + 1
    array_utils::insert_item(_subset, i + 1, _child_count, new_node);

    // insert mid back into current data
    array_utils::insert_item(_data, i, _data_count,
                             _subset[i]->_data[_subset[i]->_data_count]);
}

}  // namespace btree

#endif  // BTREE_H
