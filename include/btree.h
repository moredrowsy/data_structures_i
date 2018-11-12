/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : btree
 * DESCRIPTION : This header provides a templated self-balancing BTree class,
 *      that allows for more than two children per node.
 ******************************************************************************/
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

    // capacity
    std::size_t size() const;
    bool empty() const;

    // element access
    const T& get(const T& entry) const;  // return a ref to entry in the tree
    T& get(const T& entry);              // return a ref to entry in the tree

    // modifiers
    bool insert(const T& entry);
    bool remove(const T& entry);
    void clear();                      // clear data and delete all linked nodes
    void copy(const BTree<T>& other);  // make unique copy from source

    // operations
    T* find(const T& entry);  // return ptr to T; else nullptr
    bool contains(const T& entry) const;
    void print(std::ostream& outs = std::cout, bool debug = false,
               int level = 0, int index = 0) const;
    bool verify() const;

    // FRIENDS
    friend std::ostream& operator<<(std::ostream& outs, const BTree<T>& bt) {
        bt.print(outs);
        return outs;
    }

private:
    static const std::size_t MINIMUM = 1;
    static const std::size_t MAXIMUM = 2 * MINIMUM;

    bool _dups_ok;                   // true if duplicate keys may be inserted
    std::size_t _size;               // count of all elements
    std::size_t _data_count;         // number of data elements
    T _data[MAXIMUM + 1];            // holds the keys
    std::size_t _child_count;        // number of children
    BTree<T>* _subset[MAXIMUM + 2];  // subtrees

    bool is_leaf() const { return _child_count == 0; }  // true if leaf node
    void update_size();

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

    bool verify_tree(int& height, bool& has_stored_height, int level = 0) const;
    bool is_gt_subset(const BTree<T>* subtree, const T& item) const;
    bool is_lt_subset(const BTree<T>* subtree, const T& item) const;
};

/*******************************************************************************
 * DESCRIPTION:
 *  Default constructor.
 *
 * PRE-CONDITIONS:
 *  bool dups: allows duplicate items.
 *
 * POST-CONDITIONS:
 *  initializations
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
BTree<T>::BTree(bool dups)
    : _dups_ok(dups), _size(0), _data_count(0), _child_count(0) {}

/*******************************************************************************
 * DESCRIPTION:
 *  Destructor. Deallocates all heap memory from this object.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  deallocation
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
BTree<T>::~BTree() {
    clear();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Copy constructor.
 *
 * PRE-CONDITIONS:
 *  const BTree<T>& src: source BTree to copy
 *
 * POST-CONDITIONS:
 *  unique copy of src's states
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
BTree<T>::BTree(const BTree<T>& src)
    : _dups_ok(src._dups_ok), _size(0), _data_count(0), _child_count(0) {
    copy(src);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Assignment operator.
 *
 * PRE-CONDITIONS:
 *  const BTree<T>& rhs: source BTree to copy
 *
 * POST-CONDITIONS:
 *  unique copy of rhs's states
 *
 * RETURN:
 *  *this
 ******************************************************************************/
template <typename T>
BTree<T>& BTree<T>::operator=(const BTree<T>& rhs) {
    if(this != &rhs) {
        clear();
        copy(rhs);
    }
    return *this;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns total items in BTree.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  std::size_t
 ******************************************************************************/
template <typename T>
std::size_t BTree<T>::size() const {
    return _size;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Checks if BTree is empty.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool BTree<T>::empty() const {
    return _size == 0;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns const entry contained in the BTree. If the entry is invalid, throws
 *  invalid argument exception.
 *
 * PRE-CONDITIONS:
 *  const T& entry: must be contained in the BTree
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  const T&
 ******************************************************************************/
template <typename T>
const T& BTree<T>::get(const T& entry) const {
    T* found = find(entry);

    if(found)
        return *found;
    else
        throw std::invalid_argument("BTree::get() - invalid entry");
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns the entry contained in the BTree. If entry is not found, then
 *  inserts the entry and return find by deref.
 *
 * PRE-CONDITIONS:
 *  const T& entry: must be contained in the BTree
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  T&
 ******************************************************************************/
template <typename T>
T& BTree<T>::get(const T& entry) {
    T* found = find(entry);

    if(!found) {
        insert(entry);
        found = find(entry);
    }

    return *found;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Insert entry int BTree.
 *  Internally, it first calls loose_insert to insert entry. When returning
 *  from loose_insert, static parent might over MAXIMUM limit. If so, then
 *  transfer all current data/subset to new_node as the only child so that
 *  fix_excess can then fix this child, which was formally the parent.
 *
 * PRE-CONDITIONS:
 *  const T& entry: entry item to be inserted
 *
 * POST-CONDITIONS:
 *  T entry inserted
 *  _size inc if successful
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool BTree<T>::insert(const T& entry) {
    using namespace array_utils;

    if(loose_insert(entry)) {
        if(_data_count > MAXIMUM) {
            BTree<T>* new_node = new BTree<T>(_dups_ok);  // xfer 'this' to new

            // transfer 'this' data/subset to new node's data/subset
            transfer_array(_data, _data_count, new_node->_data,
                           new_node->_data_count);
            transfer_array(_subset, _child_count, new_node->_subset,
                           new_node->_child_count);
            new_node->update_size();

            _size = 0;
            _data_count = 0;
            _child_count = 1;       // clear child except 1
            _subset[0] = new_node;  // point only child to new node

            fix_excess(0);  // fix 'this' only child (new node) excess
            update_size();
        }
        return true;
    } else
        return false;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Remove entry int BTree.
 *  Internally, it first calls loose_remove to insert entry. When returning
 *  from loose_remove, static parent be might under MINIMUM limit with only
 *  ONE child. If so, then store parent to a temporary pointer. Then, transfer
 *  all of the child's data/subset to parent. Finally, deallocate the temporary
 *  pointer.
 *
 * PRE-CONDITIONS:
 *  const T& entry: entry item to be removed
 *
 * POST-CONDITIONS:
 *  T entry removed
 *  _size dec if successful
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool BTree<T>::remove(const T& entry) {
    using namespace array_utils;

    if(loose_remove(entry)) {
        if(_data_count == 0 && _child_count == 1) {
            BTree<T>* pop = _subset[0];  // hold child

            // transfer only child's data/subset back to 'this'
            transfer_array(_subset[0]->_data, _subset[0]->_data_count, _data,
                           _data_count);
            transfer_array(_subset[0]->_subset, _subset[0]->_child_count,
                           _subset, _child_count);
            update_size();

            pop->_child_count = 0;  // prevent double delete
            delete pop;
        }
        return true;
    } else
        return false;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Deallocates all heap BTrees and clear data/subset counts.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  empty
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void BTree<T>::clear() {
    for(std::size_t i = 0; i < _child_count; ++i) {
        _subset[i]->clear();  // recurse into subset
        delete _subset[i];
    }
    _size = 0;
    _data_count = 0;
    _child_count = 0;  // must clear child to prevent double delete
}

/*******************************************************************************
 * DESCRIPTION:
 *  Uniquely copies another BTree's into 'this'. REQUIREMENT: empty 'this'.
 *
 * PRE-CONDITIONS:
 *  const BTree<T>& other: source BTree to copy
 *  'this' BTree must be EMPTY/cleared before copying!
 *
 * POST-CONDITIONS:
 *  unique copy of other's states
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void BTree<T>::copy(const BTree<T>& other) {
    assert(this != &other);
    assert(empty());

    // copy states
    _dups_ok = other._dups_ok;
    _size = other._size;
    _child_count = other._child_count;
    array_utils::copy_array(other._data, other._data_count, _data, _data_count);

    // copy subset
    for(std::size_t i = 0; i < other._child_count; ++i) {
        _subset[i] = new BTree<T>(other._dups_ok);
        _subset[i]->copy(*other._subset[i]);
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns the pointer to entry contained in the BTree. If the entry is not
 *  found, then nullptr.
 *
 * PRE-CONDITIONS:
 *  const T& entry: item to find
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  T*
 ******************************************************************************/
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

/*******************************************************************************
 * DESCRIPTION:
 *  Checks if entry is contained in BTree.
 *
 * PRE-CONDITIONS:
 *  const T& entry: target
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
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

/*******************************************************************************
 * DESCRIPTION:
 *  Prints BTree in tree structure.
 *
 * PRE-CONDITIONS:
 *  std::ostream& outs: ostream by ref
 *  bool debug        : debug flag
 *  int level         : internal recursion depth. Don't touch.
 *  int index         : internal index of each BTree's data. Don't touch.
 *
 * POST-CONDITIONS:
 *  prints
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void BTree<T>::print(std::ostream& outs, bool debug, int level,
                     int index) const {
    if(_data_count)
        for(int i = _data_count - 1; i >= 0; --i) {
            if(_child_count)
                _subset[i + 1]->print(outs, debug, level + 1, i + 1);

            if(!level) index = i;  // assign static root index

            outs << std::setw(level * 15) << ' ';
            if(debug) outs << index << ' ';
            outs << '|' << _data[i] << "|\n";

            if(_child_count && !i) _subset[i]->print(outs, debug, level + 1, i);
        }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Checks if the internal BTree's rules are valid.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool BTree<T>::verify() const {
    bool has_stored_height = false;
    int height = 0;
    return verify_tree(height, has_stored_height);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Update BTree's size.
 *
 * PRE-CONDITIONS:
 *  _data_count and direct child's _size
 *
 * POST-CONDITIONS:
 *  std::size_t _size: sum of _data_count and direct children's _size
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void BTree<T>::update_size() {
    _size = _data_count;
    for(std::size_t i = 0; i < _child_count; ++i) _size += _subset[i]->_size;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Inserts an entry item consistent with BTree rules except that root can be
 *  over MAXIMUM limit.
 *  Internally, if entry exist, it returns false or deal with duplicate.
 *  If not found @ internal node, then recurse. If is not found @ leaf, then
 *  insert into data.
 *  Returning out of recursion, calls fix_excess on child when child is over
 *  MAX limit.
 *
 * PRE-CONDITIONS:
 *  const T& entry: item to insert
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool BTree<T>::loose_insert(const T& entry) {
    // find index of T that's greater or qual to entry
    std::size_t i = array_utils::first_ge(_data, _data_count, entry);
    bool is_found = (i < _data_count && !(entry < _data[i]));
    bool is_inserted = true;

    if(is_found) {
        if(_dups_ok) {
            _data[i] += entry;  // append entry
        } else
            is_inserted = false;  // return false on same entry
    } else if(is_leaf())          // not found @ leaf, insert data @ i
        array_utils::insert_item(_data, i, _data_count, entry);
    else {
        is_inserted = _subset[i]->loose_insert(entry);  // not found, recurse

        // fix child node's over limit
        if(_subset[i]->_data_count > MAXIMUM) fix_excess(i);
    }
    update_size();

    return is_inserted;
}

/*******************************************************************************
 * DESCRIPTION:
 *  At parent, it fixes the excess data at child i.
 *  Internally, it creates a new node and splits half of child i's data to it,
 *  excluding the mid point. It also splits half child i's subset to new node,
 *  including mid point. The midpoint is then inserted back into parent's data
 *  at i.
 *  Effectively, this passes mid point to parent, and splits the excess
 *  child into two nodes.
 *
 * PRE-CONDITIONS:
 *  std::size_t i: index of subset
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
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

    new_node->update_size();
    _subset[i]->update_size();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Removes an entry item consistent with BTree rules except that root can be
 *  under MINIMUM limit.
 *  Internally, if entry does not exist, it returns false.
 *  If not found @ internal node, then recurse.
 *  If found @ leaf, then remove item from data. If is found @ internal node,
 *  then call remove largest and place largest at entry.
 *  Returnign out of recursion, calls fix_shortage on child when child is under
 *  MIN limit.
 *
 * PRE-CONDITIONS:
 *  const T& entry: item to insert
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
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
    update_size();

    return is_removed;
}

/*******************************************************************************
 * DESCRIPTION:
 *  To fix child's shortage, it performs either one of the following: rotate
 *  left from right to i, rotate right from left to i, merge with right child
 *  or merge with left child.
 *
 * PRE-CONDITIONS:
 *  std::size_t i: destintion child
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
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

/*******************************************************************************
 * DESCRIPTION:
 *  Recursively goes into BTree and removes the largest item and calls
 *  fix_shortage on the way out of recursion.
 *
 * PRE-CONDITIONS:
 *  T& entry: entry to hold removed item
 *
 * POST-CONDITIONS:
 *  T& entry: set to removed item
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void BTree<T>::remove_largest(T& entry) {
    if(is_leaf())
        array_utils::detach_item(_data, _data_count, entry);
    else {
        _subset[_child_count - 1]->remove_largest(entry);

        // fix child's shortage
        if(_subset[_child_count - 1]->_data_count < MINIMUM)
            fix_shortage(_child_count - 1);
    }
    update_size();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Rotates right child to destination child i: rotating [i+1] to [i]
 *  Internally, it attaches data @ i into child i's data. Then it transfers
 *  child i+1's front data to data @ i. Finally, if child i+1 has children,
 *  then transfer and attach child i+1's children to child i.
 *
 * PRE-CONDITIONS:
 *  std::size_t i: destintion child
 *  REQUIREMENTS : (i < child_count - 1) and subset[i+1]->data > MIN
 *
 * POST-CONDITIONS:
 *  destintion child changed
 *
 * RETURN:
 *  none
 ******************************************************************************/
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
    _subset[i]->update_size();
    _subset[i + 1]->update_size();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Rotates left child to destination child i: rotating [i-1] to [i]
 *  Internally, it insert data @ i-1 to front of child i's data. Then it
 *  transfers child i-1's last data to data @ i-1. Finally, if child i-1 has
 *  children, then transfer and insert child i-1's children to child i.
 *
 * PRE-CONDITIONS:
 *  std::size_t i: destintion child
 *  REQUIREMENTS : (0 < i < child_count) and subset[i-1]->data > MIN
 *
 * POST-CONDITIONS:
 *  destintion child changed
 *
 * RETURN:
 *  none
 ******************************************************************************/
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
    _subset[i]->update_size();
    _subset[i - 1]->update_size();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Merges/append the entire next child to destination child @ i.
 *  Internally, transfers and append data @ i to child i. Then appends entire
 *  data/subset of child i+1 to child i.
 *
 * PRE-CONDITIONS:
 *  std::size_t i: destintion child
 *  REQUIREMENTS : (i+1) < child_count or i+1 must exist!
 *                  i+1 is at most MIN, otherwise child i is overfilled and
 *                  steps out of bound.
 *
 * POST-CONDITIONS:
 *  destintion child changed
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void BTree<T>::merge_with_next_subset(std::size_t i) {
    // remove data[i] down to subset[i]'s data via attach
    T removed;
    array_utils::delete_item(_data, i, _data_count, removed);
    array_utils::attach_item(_subset[i]->_data, _subset[i]->_data_count,
                             removed);

    // move all subset[i+i]'s data and subset to subset[i]
    array_utils::merge(_subset[i + 1]->_data, _subset[i + 1]->_data_count,
                       _subset[i]->_data, _subset[i]->_data_count);
    array_utils::merge(_subset[i + 1]->_subset, _subset[i + 1]->_child_count,
                       _subset[i]->_subset, _subset[i]->_child_count);

    // deallocate empty subset[i+1] and remove subset[i+1] from subset
    delete _subset[i + 1];
    array_utils::delete_item(_subset, i + 1, _child_count);  // shift left

    _subset[i]->update_size();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Verifies BTree structure:
 *  1) root can have 0 data/child but other nodes must be within MIN and MAX
 *  2) MAX is twice the MIN
 *  3) Entires are stored in data by sorted order
 *  4) Non-leaf nodes' children must be 1 more than data
 *  5) data[i] is greater than all of subset[i] but less than all of subset[i+1]
 *
 * PRE-CONDITIONS:
 *  int& height            : max child's height, set to 0
 *  bool& has_stored_height: if initial height has been stored, set to false
 *  int level              : recursion level. Don't touch.
 *
 * POST-CONDITIONS:
 *  int& height            : set to max child's height
 *  bool& has_stored_height: has been stored, set to true
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool BTree<T>::verify_tree(int& height, bool& has_stored_height,
                           int level) const {
    using namespace array_utils;

    // verify data count limits
    if(level && (_data_count < MINIMUM || _data_count > MAXIMUM)) return false;

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

            if(!_subset[i]->verify_tree(height, has_stored_height, level + 1))
                return false;
        }
    } else {
        if(!has_stored_height) {  // store child height for the first time
            height = level;
            has_stored_height = true;
        }

        if(height != level) return false;  // check child height are all same
    }

    return true;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Checks recursively that item is greater than all of subset.
 *
 * PRE-CONDITIONS:
 *  const BTree<T>* subtree: BTree to recursively check
 *  const T& item          : item to compare
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool BTree<T>::is_gt_subset(const BTree<T>* subtree, const T& item) const {
    if(!array_utils::is_gt(subtree->_data, subtree->_data_count, item))
        return false;

    for(std::size_t i = 0; i < subtree->_child_count; ++i)
        if(!is_gt_subset(subtree->_subset[i], item)) return false;

    return true;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Checks recursively that item is less than all of subset.
 *
 * PRE-CONDITIONS:
 *  const BTree<T>* subtree: BTree to recursively check
 *  const T& item          : item to compare
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool BTree<T>::is_lt_subset(const BTree<T>* subtree, const T& item) const {
    if(!array_utils::is_lt(subtree->_data, subtree->_data_count, item))
        return false;

    for(std::size_t i = 0; i < subtree->_child_count; ++i)
        if(!is_lt_subset(subtree->_subset[i], item)) return false;

    return true;
}

}  // namespace btree

#endif  // BTREE_H
