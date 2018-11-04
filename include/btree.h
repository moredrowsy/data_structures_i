#ifndef BTREE_H
#define BTREE_H

#include <cassert>        // assert()
#include <iomanip>        // setw()
#include "array_utils.h"  // array utilities
#include "sort.h"         // verify() sortedness

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
    bool verify();

    // MUTATORS
    bool insert(const T& entry);
    bool remove(const T& entry);

    void clear();                      // clear data and delete all linked nodes
    void copy(const BTree<T>& other);  // make unique copy from source

    T& get(const T& entry);   // return a reference to entry in the tree
    T* find(const T& entry);  // return ptr to T; else nullptr

    // FRIENDS
    friend std::ostream& operator<<(std::ostream& outs, const BTree<T>& bt) {
        bt.print_tree(outs);
        return outs;
    }

    // private:
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
    void fix_excess(std::size_t i);     // fix excess of data in child i

    // remove element functions
    bool loose_remove(const T& entry);  // allows MINIMUM-1 data in the root
    void fix_shortage(std::size_t i);   // fix shortage of data in child i

    void remove_largest(T& entry);     // remove largest child of tree->entry
    void rotate_left(std::size_t i);   // xfer one data from child i+1 to i
    void rotate_right(std::size_t i);  // xfer one data from child i-1 to i
    void merge_with_next_subset(std::size_t i);  // merge subset i w/ subset i+1

    bool verify_tree(int& depth, bool& has_stored_depth, int level = 0);
    bool is_gt_subset(const BTree<T>* subtree, const T& item);
    bool is_lt_subset(const BTree<T>* subtree, const T& item);
};

template <typename T>
BTree<T>::BTree(bool dups) : _dups_ok(dups), _data_count(0), _child_count(0) {}

template <typename T>
BTree<T>::~BTree() {
    clear();
}

template <typename T>
BTree<T>::BTree(const BTree<T>& src)
    : _dups_ok(src._dups_ok), _data_count(0), _child_count(0) {
    copy(src);
}

template <typename T>
BTree<T>& BTree<T>::operator=(const BTree<T>& rhs) {
    if(this != &rhs) {
        clear();
        copy(rhs);
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
    std::size_t count = _data_count;
    for(std::size_t i = 0; i < _child_count; ++i) count += _subset[i]->size();

    return count;
}

template <typename T>
void BTree<T>::print_tree(std::ostream& outs, int level, int index) const {
    int mid = (_child_count - 1) / 2;  // store midpoint

    for(int i = _child_count - 1; i > mid; --i)
        _subset[i]->print_tree(outs, level + 1, i);  // recurse right of mid

    // outstream data
    outs << std::setw(level * 15) << ' ' << index << " |";
    for(std::size_t i = 0; i < _data_count; ++i) {
        outs << _data[i];

        if(i != _data_count - 1) outs << ", ";
    }
    outs << "|\n";

    for(int i = mid; i >= 0; --i)
        _subset[i]->print_tree(outs, level + 1, i);  // recurse left of mid
}

template <typename T>
bool BTree<T>::verify() {
    bool has_stored_depth = false;
    int depth = 0;
    return verify_tree(depth, has_stored_depth);
}

template <typename T>
bool BTree<T>::insert(const T& entry) {
    bool is_inserted = loose_insert(entry);

    if(_data_count > MAXIMUM) {
        BTree<T>* new_node = new BTree<T>(_dups_ok);  // xfer 'this' to new node

        // copy 'this' data/subset to new node's data/subset
        array_utils::copy_array(_data, _data_count, new_node->_data,
                                new_node->_data_count);
        array_utils::copy_array(_subset, _child_count, new_node->_subset,
                                new_node->_child_count);

        _data_count = 0;        // clear data
        _child_count = 1;       // clear child except 1
        _subset[0] = new_node;  // point only child to new node

        fix_excess(0);  // fix 'this' only child (new node) excess
    }

    return is_inserted;
}

template <typename T>
bool BTree<T>::remove(const T& entry) {
    bool is_removed = loose_remove(entry);

    if(_data_count == 0 && _child_count == 1) {
        BTree<T>* pop = _subset[0];  // hold only child

        // copy all of child's data back to 'this'
        array_utils::copy_array(_subset[0]->_data, _subset[0]->_data_count,
                                _data, _data_count);
        array_utils::copy_array(_subset[0]->_subset, _subset[0]->_child_count,
                                _subset, _child_count);

        pop->_child_count = 0;  // clear child count to prevent double delete
        delete pop;
    }

    return is_removed;
}

template <typename T>
void BTree<T>::clear() {
    for(std::size_t i = 0; i < _child_count; ++i) {
        _subset[i]->clear();  // recurse into subset
        delete _subset[i];
    }

    _child_count = 0;  // must clear child to prevent double delete
    _data_count = 0;
}

// PRE: 'this' tree must be cleared before call copy tree
template <typename T>
void BTree<T>::copy(const BTree<T>& other) {
    assert(this != &other);
    assert(empty());

    // copy data
    array_utils::copy_array(other._data, other._data_count, _data, _data_count);
    _child_count = other._child_count;
    _dups_ok = other._dups_ok;

    if(!other.is_leaf()) {  // copy subset
        for(std::size_t i = 0; i < other._child_count; ++i) {
            _subset[i] = new BTree<T>(other._dups_ok);
            _subset[i]->copy(*other._subset[i]);
        }
    }
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
    bool is_inserted = true;

    if(is_found)
        is_inserted = false;  // no dups, then false
    else if(is_leaf())        // not found @ leaf, insert data @ i
        array_utils::insert_item(_data, i, _data_count, entry);
    else {
        is_inserted = _subset[i]->loose_insert(entry);  // not found, recurse

        // fix child node's over limit
        if(_subset[i]->_data_count > MAXIMUM) fix_excess(i);
    }

    return is_inserted;
}

template <typename T>
void BTree<T>::fix_excess(std::size_t i) {
    BTree<T>* new_node = new BTree<T>(_dups_ok);  // xfer excess data after mid

    // move elements after half of subset[i]'s data to new node's data
    array_utils::split(_subset[i]->_data, _subset[i]->_data_count,
                       new_node->_data, new_node->_data_count);
    --_subset[i]->_data_count;  // exclude midpoint

    // copy half and after of subset[i]'s subset pointers to new node's subset
    array_utils::split(_subset[i]->_subset, _subset[i]->_child_count,
                       new_node->_subset, new_node->_child_count);

    // insert new node after subset[i], which is @ i + 1
    array_utils::insert_item(_subset, i + 1, _child_count, new_node);

    // insert mid back into data[i]; subset[i]'s data_count points to mid
    array_utils::insert_item(_data, i, _data_count,
                             _subset[i]->_data[_subset[i]->_data_count]);
}

template <typename T>
bool BTree<T>::loose_remove(const T& entry) {
    // find index of T that's greater or qual to entry
    std::size_t i = array_utils::first_ge(_data, _data_count, entry);
    bool is_found = (i < _data_count && !(entry < _data[i]));
    bool is_removed = true;

    if(is_leaf()) {
        if(is_found)  // found @ leaf, delete data @ i
            array_utils::delete_item(_data, i, _data_count);
        else
            is_removed = false;  // not found @ leaf, then false
    } else {
        if(is_found)
            _subset[i]->remove_largest(_data[i]);
        else
            is_removed = _subset[i]->loose_remove(entry);  // not found, recurse

        // fix child's shortage
        if(_subset[i]->_data_count < MINIMUM) fix_shortage(i);
    }

    return is_removed;
}

template <typename T>
void BTree<T>::fix_shortage(std::size_t i) {
    if(i + 1 < _child_count && _subset[i + 1]->_data_count > MINIMUM)
        rotate_left(i);  // when right has more than minimum
    else if(i > 0 && i < _child_count && _subset[i - 1]->_data_count > MINIMUM)
        rotate_right(i);  // when left has more than minimum
    else if(i + 1 < _child_count)
        merge_with_next_subset(i);  // if there is right child
    else
        merge_with_next_subset(i - 1);  // if there is left child
}

template <typename T>
void BTree<T>::remove_largest(T& entry) {
    if(is_leaf())
        array_utils::detach_item(_data, _data_count, entry);
    else
        _subset[_child_count - 1]->remove_largest(entry);
}

// PRE: (i < child_count - 1) and subset[i+1]->data > MIN
// rotating [i+1] to [i]
template <typename T>
void BTree<T>::rotate_left(std::size_t i) {
    using namespace array_utils;

    // move data[i] down to subset[i]'s data via attach
    attach_item(_subset[i]->_data, _subset[i]->_data_count, _data[i]);

    // move subset[i+1]'s front data to data[i] via delete
    delete_item(_subset[i + 1]->_data, 0, _subset[i + 1]->_data_count,
                _data[i]);

    // if subest[i+1] has child, transfer to subset[i] via attach
    if(_subset[i + 1]->_child_count) {
        BTree<T>* removed = nullptr;
        delete_item(_subset[i + 1]->_subset, 0, _subset[i + 1]->_child_count,
                    removed);
        attach_item(_subset[i]->_subset, _subset[i]->_child_count, removed);
    }
}

// PRE: (0 < i < child_count) and subset[i-1]->data > MIN
// rotating [i-1] to [i]
template <typename T>
void BTree<T>::rotate_right(std::size_t i) {
    using namespace array_utils;

    // transfer data[i-1] down to front of subset[i]->data via insert
    insert_item(_subset[i]->_data, 0, _subset[i]->_data_count, _data[i - 1]);

    // transfer subset[i-1]'s last data to replace data[i-1] via detach
    detach_item(_subset[i - 1]->_data, _subset[i - 1]->_data_count,
                _data[i - 1]);

    // transfer subset[i-1]'s last subset to front of subset[i]'s subset
    if(_subset[i - 1]->_child_count) {
        BTree<T>* detach = nullptr;
        detach_item(_subset[i - 1]->_subset, _subset[i - 1]->_child_count,
                    detach);
        insert_item(_subset[i]->_subset, 0, _subset[i]->_child_count, detach);
    }
}

// PRE: only merge when one child is at most MIN! Otherwise you'll over fill
// the array and get double free error when firing the destructor
template <typename T>
void BTree<T>::merge_with_next_subset(std::size_t i) {
    // remove data[i] down to subset[i]'s data via append
    T removed;
    array_utils::delete_item(_data, i, _data_count, removed);
    array_utils::attach_item(_subset[i]->_data, _subset[i]->_data_count,
                             removed);

    // move all subset[i+i]'s data and subset to subset[i]
    array_utils::merge(_subset[i]->_data, _subset[i]->_data_count,
                       _subset[i + 1]->_data, _subset[i + 1]->_data_count);
    array_utils::merge(_subset[i]->_subset, _subset[i]->_child_count,
                       _subset[i + 1]->_subset, _subset[i + 1]->_child_count);

    // deallocate empty subset[i+1] and remove subset[i+1] from subset
    delete _subset[i + 1];
    array_utils::delete_item(_subset, i + 1, _child_count);  // shift left
}

template <typename T>
bool BTree<T>::verify_tree(int& depth, bool& has_stored_depth, int level) {
    using namespace array_utils;

    // verify data count limits
    if(_data_count < MINIMUM || _data_count > MAXIMUM) return false;

    // verify data is sorted
    if(!sort::verify(_data, _data_count)) return false;

    if(!is_leaf()) {
        // verify child count limits
        if(_child_count > MAXIMUM + 1 || _child_count != _data_count + 1)
            return false;

        for(std::size_t i = 0; i < _child_count; ++i) {
            if(i + 1 < _child_count) {
                // verify data[i] is greater than all of subset[i]
                if(!is_gt_subset(_subset[i], _data[i])) return false;

                // verify data[i] is less than all of subset[i+1]
                if(!is_lt_subset(_subset[i + 1], _data[i])) return false;
            }

            if(!_subset[i]->verify_tree(depth, has_stored_depth, level + 1))
                return false;
        }
    } else {
        if(!has_stored_depth) {  // store child depth for the first time
            depth = level;
            has_stored_depth = true;
        }

        if(depth != level) return false;  // check child depth are all same
    }

    return true;
}

template <typename T>
bool BTree<T>::is_gt_subset(const BTree<T>* subtree, const T& item) {
    if(!array_utils::is_gt(subtree->_data, subtree->_data_count, item))
        return false;

    for(std::size_t i = 0; i < subtree->_child_count; ++i)
        if(!is_gt_subset(subtree->_subset[i], item)) return false;

    return true;
}

template <typename T>
bool BTree<T>::is_lt_subset(const BTree<T>* subtree, const T& item) {
    if(!array_utils::is_lt(subtree->_data, subtree->_data_count, item))
        return false;

    for(std::size_t i = 0; i < subtree->_child_count; ++i)
        if(!is_lt_subset(subtree->_subset[i], item)) return false;

    return true;
}

}  // namespace btree

#endif  // BTREE_H
