/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : bptree
 * DESCRIPTION : This header provides a templated self-balancing BPTree class,
 *      the B+ Tree, that allows for more than two children per node but with
 *      the real data only at the leaf nodes.
 *
 *      RULES:
 *      1. Root can have 0 entries if no children, or at least 1 entry if it
 *         has children. Every other node requires minimum entries.
 *      2. Maximum entries is 2x minimum.
 *      3. Key entries are stored in partially filled arrays, in ascending
 *         order while real entires are stored only at leaves (leaf entries).
 *      4. Number of children is 1 + number of key entries.
 *      5. Every nonleaf node is:
 *         A) entry at i is greater than all entries in child i.
 *         B) entry at i is less than or equal to all entires in child i+1.
 *      6. Every key entries require a corresponding leaf entry.
 *         Therefore, there exist at MAX 1 key entry per leaf entry but not
 *         every leaf entry require a key entry.
 ******************************************************************************/
#ifndef BPTREE_H
#define BPTREE_H

#include <cassert>        // assert()
#include <string>         // string objects
#include "array_utils.h"  // array utilities
#include "sort.h"         // verify() sortedness

namespace bptree {

enum { MINIMUM = 1 };

template <class T>
class BPTree {
public:
    class Iterator {
    public:
        friend class BPTree;

        // CONSTRUCTOR
        Iterator(BPTree<T>* it = nullptr, std::size_t index = 0)
            : _it(it), _index(index) {}

        bool is_null() { return !_it; }
        explicit operator bool() { return _it; }

        T& operator*() {
            if(!_it)
                throw std::invalid_argument("BPTree::Iterator - nullptr check");

            if(_index >= _it->_data_count)
                throw std::out_of_range("BPTree::Iterator - range check");

            return _it->_data[_index];
        }

        T* operator->() {
            if(!_it)
                throw std::invalid_argument("BPTree::Iterator - nullptr check");

            if(_index >= _it->_data_count)
                throw std::out_of_range("BPTree::Iterator - range check");

            return &_it->_data[_index];
        }

        Iterator& operator++() {  // pre-inc
            if(_it && ++_index == _it->_data_count) {
                _it = _it->_next;
                _index = 0;
            }
            return *this;
        }

        Iterator operator++(int _u) {  // post-inc
            (void)_u;                  // suppress unused warning
            Iterator it = *this;       // make temp
            operator++();              // pre-inc
            return it;                 // return previous state
        }

        void print_Iterator() { std::cout << *_it; }

        // FRIENDS
        friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
            return lhs._it == rhs._it && lhs._index == rhs._index;
        }

        friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
            return lhs._it != rhs._it;
        }

    private:
        BPTree<T>* _it;
        std::size_t _index;
    };

    // CONSTRUCTOR
    BPTree(bool dups = false, std::size_t min = MINIMUM);

    // BIG THREE
    ~BPTree();
    BPTree(const BPTree<T>& src);
    BPTree<T>& operator=(const BPTree<T>& rhs);

    // capacity
    std::size_t size() const;
    bool empty() const;

    // element access
    Iterator begin();
    Iterator end();
    Iterator find(const T& entry);
    const T& get(const T& entry) const;  // return a ref to entry in the tree
    T& get(const T& entry);              // return a ref to entry in the tree

    // modifiers
    bool insert(const T& entry);
    bool remove(const T& entry);
    void clear();  // clear data and delete all nodes

    // misc
    bool contains(const T& entry) const;
    void print(std::ostream& outs = std::cout, bool debug = false,
               int level = 0, int index = 0) const;
    bool verify() const;

    // FRIENDS
    friend std::ostream& operator<<(std::ostream& outs, const BPTree<T>& bt) {
        bt.print(outs, true);
        return outs;
    }

private:
    std::size_t _min;
    std::size_t _max;
    bool _dups_ok;             // true if duplicate keys may be inserted
    std::size_t _size;         // count of all elements
    std::size_t _data_count;   // number of data elements
    T* _data;                  // holds the keys
    std::size_t _child_count;  // number of children
    BPTree<T>** _subset;       // subtrees
    BPTree<T>* _next;          // next sibling's subset

    void copy(const BPTree<T>& other);                    // wrapper to copy
    void copy(const BPTree<T>& other, BPTree<T>*& next);  // copy tree
    void deallocate();

    inline bool is_leaf() const { return _child_count == 0; }  // check if leaf
    void update_size();

    // insert element functions
    bool loose_insert(const T& entry);  // allows _max+1 data in the root
    void fix_excess(std::size_t i);     // fix excess of data in child i

    // remove element functions
    bool loose_remove(const T& entry);    // allows _min-1 data in the root
    void fix_shortage(std::size_t i);     // fix shortage of data in child i
    void remove_dup_key(const T& entry);  // remove duplicate key after delete

    void rotate_left(std::size_t i);   // xfer one data from child i+1 to i
    void rotate_right(std::size_t i);  // xfer one data from child i-1 to i
    void merge_with_next_subset(std::size_t i);  // merge subset i w/ subset i+1

    BPTree<T>* get_smallest_node();
    BPTree<T>* get_largest_node();
    void get_smallest(T& entry);    // entry := leftmost leaf
    void get_largest(T& entry);     // entry := rightmost leaf
    void remove_largest(T& entry);  // remove largest child of tree->entry
    T* find_ptr(const T& entry);    // return ptr to T; else nullptr

    bool verify_tree(int& height, bool& has_stored_height, int level = 0) const;
    bool is_gt_subset(const BPTree<T>* subtree, const T& item) const;
    bool is_le_subset(const BPTree<T>* subtree, const T& item) const;
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
BPTree<T>::BPTree(bool dups, std::size_t min)
    : _min(min),
      _max(2 * _min),
      _dups_ok(dups),
      _size(0),
      _data_count(0),
      _data(nullptr),
      _child_count(0),
      _subset(nullptr),
      _next(nullptr) {
    _data = new T[_max + 1];
    _subset = new BPTree<T>*[_max + 2];
}

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
BPTree<T>::~BPTree() {
    deallocate();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Copy constructor.
 *
 * PRE-CONDITIONS:
 *  const BPTree<T>& src: source BPTree to copy
 *
 * POST-CONDITIONS:
 *  unique copy of src states
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
BPTree<T>::BPTree(const BPTree<T>& src)
    : _min(src._min),
      _max(src._max),
      _dups_ok(src._dups_ok),
      _size(0),
      _data_count(0),
      _data(nullptr),
      _child_count(0),
      _subset(nullptr),
      _next(nullptr) {
    _data = new T[_max + 1];
    _subset = new BPTree<T>*[_max + 2];
    copy(src);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Assignment operator.
 *
 * PRE-CONDITIONS:
 *  const BPTree<T>& rhs: source BPTree to copy
 *
 * POST-CONDITIONS:
 *  unique copy of rhs states
 *
 * RETURN:
 *  *this
 ******************************************************************************/
template <typename T>
BPTree<T>& BPTree<T>::operator=(const BPTree<T>& rhs) {
    if(this != &rhs) {
        _min = rhs._min;
        _max = rhs._max;
        clear();
        copy(rhs);
    }
    return *this;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns total items in BPTree.
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
std::size_t BPTree<T>::size() const {
    return _size;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Checks if BPTree is empty.
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
bool BPTree<T>::empty() const {
    return _size == 0;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Points to left most element in tree.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  BPTree<T>::Iterator: points to left most element
 ******************************************************************************/
template <typename T>
typename BPTree<T>::Iterator BPTree<T>::begin() {
    return size() ? BPTree<T>::Iterator(get_smallest_node())
                  : BPTree<T>::Iterator(nullptr);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Points to nullptr.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  BPTree<T>::Iterator: points to nullptr
 ******************************************************************************/
template <typename T>
typename BPTree<T>::Iterator BPTree<T>::end() {
    return BPTree<T>::Iterator(nullptr);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Return iterator to entry; else iterator points to nullptr.
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
typename BPTree<T>::Iterator BPTree<T>::find(const T& entry) {
    // find index of T that's greater or qual to entry
    std::size_t i = array_utils::first_ge(_data, _data_count, entry);
    bool is_found = (i < _data_count && !(entry < _data[i]));

    if(is_leaf()) {
        if(is_found)
            return BPTree<T>::Iterator(this, i);
        else
            return BPTree<T>::Iterator(nullptr);
    } else {                                     // @ !leaf
        if(is_found)                             // when found
            return _subset[i + 1]->find(entry);  // recurse i+1
        else                                     // when !found
            return _subset[i]->find(entry);      // recurse to find entry
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns const entry contained in the BPTree. If the entry is invalid,
 *throws invalid argument exception.
 *
 * PRE-CONDITIONS:
 *  const T& entry: must be contained in the BPTree
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  const T&
 ******************************************************************************/
template <typename T>
const T& BPTree<T>::get(const T& entry) const {
    T* found = find_ptr(entry);

    if(found)
        return *found;
    else
        throw std::invalid_argument("BPTree::get() - invalid entry");
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns the entry contained in the BPTree. If entry is not found, then
 *  inserts the entry and return find by deref.
 *
 * PRE-CONDITIONS:
 *  const T& entry: must be contained in the BPTree
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  T&
 ******************************************************************************/
template <typename T>
T& BPTree<T>::get(const T& entry) {
    T* found = find_ptr(entry);

    if(!found) {
        insert(entry);
        found = find_ptr(entry);
    }

    return *found;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Insert entry int BPTree.
 *  Internally, it first calls loose_insert to insert entry. When returning
 *  from loose_insert, static parent might over _max limit. If so, then
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
bool BPTree<T>::insert(const T& entry) {
    using namespace array_utils;

    if(loose_insert(entry)) {
        if(_data_count > _max) {
            BPTree<T>* new_node =
                new BPTree<T>(_dups_ok);  // xfer 'this' to new

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
 *  Remove entry int BPTree.
 *  Internally, it first calls loose_remove to insert entry. When returning
 *  from loose_remove, static parent be might under _min limit with only
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
bool BPTree<T>::remove(const T& entry) {
    using namespace array_utils;

    if(loose_remove(entry)) {
        if(_data_count <= 1 && _child_count == 1) {
            BPTree<T>* pop = _subset[0];  // hold child

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
 *  Deallocates all heap BPTrees and clear data/subset counts. Reallocate data
 *  and subset.
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
void BPTree<T>::clear() {
    deallocate();
    _data = new T[_max + 1];
    _subset = new BPTree<T>*[_max + 2];
}

/*******************************************************************************
 * DESCRIPTION:
 *  Checks if entry is contained in BPTree.
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
bool BPTree<T>::contains(const T& entry) const {
    // find index of T that's greater or qual to entry
    std::size_t i = array_utils::first_ge(_data, _data_count, entry);
    bool is_found = (i < _data_count && !(entry < _data[i]));

    if(is_leaf()) {
        if(is_found)
            return true;
        else
            return false;
    } else {                                         // @ !leaf
        if(is_found)                                 // when found
            return _subset[i + 1]->contains(entry);  // recurse i+1
        else                                         // when !found
            return _subset[i]->contains(entry);      // recurse to find entry
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Prints BPTree in tree structure.
 *
 * PRE-CONDITIONS:
 *  std::ostream& outs: ostream by ref
 *  bool debug        : debug flag
 *  int level         : internal recursion depth. Don't touch.
 *  int index         : internal index of each BPTree's data. Don't touch.
 *
 * POST-CONDITIONS:
 *  prints
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void BPTree<T>::print(std::ostream& outs, bool debug, int level,
                      int index) const {
    if(_data_count)
        for(int i = _data_count - 1; i >= 0; --i) {
            if(!is_leaf()) _subset[i + 1]->print(outs, debug, level + 1, i + 1);

            if(!level) index = i;  // assign static root index

            outs << std::string(level * 15, ' ');
            if(debug) outs << index << ' ';
            outs << '|' << _data[i] << "|\n";

            if(!is_leaf() && !i) _subset[i]->print(outs, debug, level + 1, i);
        }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Checks if the internal BPTree's rules are valid.
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
bool BPTree<T>::verify() const {
    bool has_stored_height = false;
    int height = 0;
    return verify_tree(height, has_stored_height);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Uniquely copies another BPTree's into 'this'. REQUIREMENT: empty 'this'.
 *  This is a wrapper to call the copy() with tree pointer by ref to keep track
 *  of leaf nodes.
 *
 * PRE-CONDITIONS:
 *  const BPTree<T>& other: source BPTree to copy
 *  'this' BPTree must be EMPTY/cleared before copying!
 *
 * POST-CONDITIONS:
 *  unique copy of other's states
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void BPTree<T>::copy(const BPTree<T>& other) {
    assert(this != &other);
    assert(empty());

    BPTree<T>* next = nullptr;  // to keep track of leaf node by ref
    copy(other, next);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Walkbackwards to uniquely copy another BPTree. Update every unique leaf's
 *  _next to ref next (which will keep track of next unique leaf node).
 *
 * PRE-CONDITIONS:
 *  const BPTree<T>& other: source BPTree to copy
 *  BPTree<T>*& next = nullptr and keep track of next unique leaf node
 *
 * POST-CONDITIONS:
 *  next := the next leaf node
 *  unique copy of other's states
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void BPTree<T>::copy(const BPTree<T>& other, BPTree<T>*& next) {
    // copy states
    _dups_ok = other._dups_ok;
    _size = other._size;
    _child_count = other._child_count;
    array_utils::copy_array(other._data, other._data_count, _data, _data_count);

    if(is_leaf()) {    // when leaf
        _next = next;  // assign this' _next to ref next
        next = this;   // update ref next to this
    } else {
        for(int i = (int)_child_count - 1; i >= 0; --i) {  // when !leaf
            _subset[i] = new BPTree<T>(other._dups_ok);    // copy backwards
            _subset[i]->copy(*other._subset[i], next);
        }
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Deallocates all heap BPTrees and clear data/subset counts.
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
void BPTree<T>::deallocate() {
    for(std::size_t i = 0; i < _child_count; ++i) {
        _subset[i]->clear();  // recurse into subset
        delete _subset[i];
    }
    _size = 0;
    _data_count = 0;
    _child_count = 0;  // must clear child to prevent double delete
    _next = nullptr;

    delete[] _data;
    delete[] _subset;
    _data = nullptr;    // set to nullptr to prevent double delete
    _subset = nullptr;  // set to nullptr to prevent double delete
}

/*******************************************************************************
 * DESCRIPTION:
 *  Update BPTree's size.
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
void BPTree<T>::update_size() {
    _size = _child_count ? 0 : _data_count;
    for(std::size_t i = 0; i < _child_count; ++i) _size += _subset[i]->_size;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Inserts an entry item consistent with BPTree rules except that root can
 *be over _max limit. Internally, if entry exist, it returns false or deal
 *with duplicate. If not found @ internal node, then recurse. If is not found @
 *leaf, then insert into data. Returning out of recursion, calls fix_excess on
 *child when child is over MAX limit.
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
bool BPTree<T>::loose_insert(const T& entry) {
    // find index of T that's greater or qual to entry
    std::size_t i = array_utils::first_ge(_data, _data_count, entry);
    bool is_found = (i < _data_count && !(entry < _data[i]));
    bool is_inserted = true;

    if(is_leaf()) {
        if(is_found) {
            if(_dups_ok)
                _data[i] += entry;  // append entry
            else
                is_inserted = false;  // return false on same entry
        } else
            array_utils::insert_item(_data, i, _data_count, entry);
    } else {
        if(is_found) {
            is_inserted = _subset[i + 1]->loose_insert(entry);  // recurse i+1

            // fix child node's over limit
            if(_subset[i + 1]->_data_count > _max) fix_excess(i + 1);
        } else {
            is_inserted = _subset[i]->loose_insert(entry);  // !found, recurse i

            // fix child node's over limit
            if(_subset[i]->_data_count > _max) fix_excess(i);
        }
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
void BPTree<T>::fix_excess(std::size_t i) {
    using namespace array_utils;

    bool is_after_mid = _subset[i]->is_leaf() ? false : true;  // after mid?
    BPTree<T>* new_node = new BPTree<T>(_dups_ok);             // xfer excess

    // move after half of subset[i]'s data to new node's data
    split(_subset[i]->_data, _subset[i]->_data_count, new_node->_data,
          new_node->_data_count, is_after_mid);
    if(is_after_mid) --_subset[i]->_data_count;  // exclude midpoint after mid

    // copy after half of subset[i]'s subset pointers to new node's subset
    split(_subset[i]->_subset, _subset[i]->_child_count, new_node->_subset,
          new_node->_child_count, is_after_mid);

    // insert new node after subset[i], which is @ i + 1
    insert_item(_subset, i + 1, _child_count, new_node);

    // get mid
    T mid = is_after_mid ? _subset[i]->_data[_subset[i]->_data_count]
                         : new_node->_data[0];

    // insert mid back into data[i]; subset[i]'s data_count points to mid
    insert_item(_data, i, _data_count, std::move(mid));

    new_node->_next = _subset[i]->_next;  // update next pointers
    _subset[i]->_next = new_node;

    new_node->update_size();
    _subset[i]->update_size();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Removes an entry item consistent with BPTree rules except that root can
 *be under _min limit. Internally, if entry does not exist, it returns false.
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
bool BPTree<T>::loose_remove(const T& entry) {
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
        if(is_found) {
            is_removed = _subset[i + 1]->loose_remove(entry);  // recurse i+1

            // fix child's shortage
            if(_subset[i + 1]->_data_count < _min) fix_shortage(i + 1);
            remove_dup_key(entry);
        } else {
            is_removed = _subset[i]->loose_remove(entry);  // !found, recurse i

            // fix child's shortage
            if(_subset[i]->_data_count < _min) fix_shortage(i);
        }
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
void BPTree<T>::fix_shortage(std::size_t i) {
    if(i + 1 < _child_count && _subset[i + 1]->_data_count > _min)
        rotate_left(i);  // when right has more than minimum
    else if(i > 0 && i < _child_count && _subset[i - 1]->_data_count > _min)
        rotate_right(i);  // when left has more than minimum
    else if(i + 1 < _child_count)
        merge_with_next_subset(i);  // if there is right child
    else
        merge_with_next_subset(i - 1);  // if there is left child
}

/*******************************************************************************
 * DESCRIPTION:
 *  Searches for the duplicate key entry at data of i. If found, replaces data
 *  of i with smallest entry of child i+1.
 *
 * PRE-CONDITIONS:
 *  const T& entry: duplicate key entry to remove
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void BPTree<T>::remove_dup_key(const T& entry) {
    // find index of T that's greater or qual to entry
    std::size_t i = array_utils::first_ge(_data, _data_count, entry);
    bool is_found = (i < _data_count && !(entry < _data[i]));

    if(is_leaf()) {
        if(is_found)  // found @ leaf, delete data @ i
            array_utils::delete_item(_data, i, _data_count);
        else
            return;
    } else {
        if(is_found)
            _subset[i + 1]->get_smallest(_data[i]);
        else
            _subset[i]->remove_dup_key(entry);
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Rotates right child to destination child i: rotating [i+1] to [i]
 *  Internally, it attaches data @ i into child i's data. Then it transfers
 *  child i+1's front data to data @ i. Finally, if child i+1 has children,
 *  then transfer and attach child i+1's children to child i.
 *  Algorithm:
 *  1. Attach data @ i to back of child i's data
 *  2. If child i+1 is !leaf, then transfer child i+1's front data to data @ i
 *        and transfer child i+1's front subset to back of child i's subset.
 *     else simply delete child i+1's front data and then add child i+1's front
 *          data, which was previously child i+1's second data after delete, to
 *          data @ i.
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
void BPTree<T>::rotate_left(std::size_t i) {
    using namespace array_utils;

    // move data[i] down to subset[i]'s data via attach
    attach_item(_subset[i]->_data, _subset[i]->_data_count, _data[i]);

    // if subset[i+1] has child, transfer to subset[i] via attach
    if(!_subset[i + 1]->is_leaf()) {
        // move subset[i+1]'s front data to data[i] via delete
        delete_item(_subset[i + 1]->_data, 0, _subset[i + 1]->_data_count,
                    _data[i]);

        BPTree<T>* removed = nullptr;
        delete_item(_subset[i + 1]->_subset, 0, _subset[i + 1]->_child_count,
                    removed);
        attach_item(_subset[i]->_subset, _subset[i]->_child_count, removed);
    } else {
        // simply delete subset[i+1]'s front data
        delete_item(_subset[i + 1]->_data, 0, _subset[i + 1]->_data_count);

        // move subset[i+1]'s front data (previously second front) to data[i]
        _data[i] = _subset[i + 1]->_data[0];
    }

    _subset[i]->update_size();
    _subset[i + 1]->update_size();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Rotates left child to destination child i: rotating [i-1] to [i]
 *  Algorithm:
 *  1. Insert data @ i-1 to front of child i's data.
 *  2. Detach child i-1's last data to data @ i-1.
 *  3. If child i-1 is !leaf, then detach child i-1's last subset and
 *     insert to child i's front subset;
 *     else insert data @ i-1 to front of child i's data.
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
void BPTree<T>::rotate_right(std::size_t i) {
    using namespace array_utils;

    // copy data[i-1] down to front of subset[i]->data via insert
    insert_item(_subset[i]->_data, 0, _subset[i]->_data_count, _data[i - 1]);

    // transfer subset[i-1]'s last data to replace data[i-1] via detach
    detach_item(_subset[i - 1]->_data, _subset[i - 1]->_data_count,
                _data[i - 1]);

    // transfer subset[i-1]'s last subset to front of subset[i]'s subset
    if(!_subset[i - 1]->is_leaf()) {
        BPTree<T>* detach = nullptr;
        detach_item(_subset[i - 1]->_subset, _subset[i - 1]->_child_count,
                    detach);
        insert_item(_subset[i]->_subset, 0, _subset[i]->_child_count, detach);
    } else {
        // copy data[i-1] to subset[i]'s front data
        insert_item(_subset[i]->_data, 0, _subset[i]->_data_count,
                    _data[i - 1]);
    }

    _subset[i]->update_size();
    _subset[i - 1]->update_size();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Merges/append the entire next child to destination child @ i.
 *  Algorithm:
 *  1. If child i is leaf, then delete data[i];
 *     else transfer and append data @ i to child i.
 *  2. Appends entire data/subset of child i+1 to child i.
 *  3. Update child i's next pointer and delete child i+1.
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
void BPTree<T>::merge_with_next_subset(std::size_t i) {
    // remove data[i] down to subset[i]'s data via attach
    T removed;
    array_utils::delete_item(_data, i, _data_count, removed);

    if(!_subset[i]->is_leaf())
        array_utils::attach_item(_subset[i]->_data, _subset[i]->_data_count,
                                 removed);

    // move all subset[i+i]'s data and subset to subset[i]
    array_utils::merge(_subset[i + 1]->_data, _subset[i + 1]->_data_count,
                       _subset[i]->_data, _subset[i]->_data_count);
    array_utils::merge(_subset[i + 1]->_subset, _subset[i + 1]->_child_count,
                       _subset[i]->_subset, _subset[i]->_child_count);

    _subset[i]->_next = _subset[i + 1]->_next;  // update next pointer

    // deallocate empty subset[i+1] and remove subset[i+1] from subset
    delete _subset[i + 1];
    array_utils::delete_item(_subset, i + 1, _child_count);  // shift left

    // _subset[i]->_next = _subset[i + 1];  // update next pointer
    _subset[i]->update_size();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Return pointer to smallest item @ leaf node.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  BPTree<T>*: pointer to smallest
 ******************************************************************************/
template <typename T>
BPTree<T>* BPTree<T>::get_smallest_node() {
    if(is_leaf())
        return this;
    else
        return _subset[0]->get_smallest_node();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Return pointer to largest item @ leaf node.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  BPTree<T>*: pointer to largest
 ******************************************************************************/
template <typename T>
BPTree<T>* BPTree<T>::get_largest_node() {
    if(is_leaf())
        return this;
    else
        return _subset[_child_count - 1]->get_largest_node();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Return by reference the smallest item @ leaf node.
 *
 * PRE-CONDITIONS:
 *  T& entry: item to hold smallest
 *
 * POST-CONDITIONS:
 *  T& entry: set to smallest item in tree
 *
 * RETURN:
 *  by ref to entry
 ******************************************************************************/
template <typename T>
void BPTree<T>::get_smallest(T& entry) {
    if(is_leaf())
        entry = _data[0];
    else
        _subset[0]->get_smallest(entry);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Return by reference the largest item @ leaf node.
 *
 * PRE-CONDITIONS:
 *  T& entry: item to hold largest
 *
 * POST-CONDITIONS:
 *  T& entry: set to largest item in tree
 *
 * RETURN:
 *  by ref to entry
 ******************************************************************************/
template <typename T>
void BPTree<T>::get_largest(T& entry) {
    if(is_leaf())
        entry = _data[_data_count - 1];
    else
        _subset[_child_count - 1]->get_largest(entry);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Recursively goes into BPTree and removes the largest item and calls
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
void BPTree<T>::remove_largest(T& entry) {
    if(is_leaf())
        array_utils::detach_item(_data, _data_count, entry);
    else {
        _subset[_child_count - 1]->remove_largest(entry);

        // fix child's shortage
        if(_subset[_child_count - 1]->_data_count < _min)
            fix_shortage(_child_count - 1);
    }
    update_size();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns the pointer to entry contained in the BPTree. If the entry is not
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
T* BPTree<T>::find_ptr(const T& entry) {
    // find index of T that's greater or qual to entry
    std::size_t i = array_utils::first_ge(_data, _data_count, entry);
    bool is_found = (i < _data_count && !(entry < _data[i]));

    if(is_leaf()) {
        if(is_found)
            return &_data[i];
        else
            return nullptr;
    } else {                                         // @ !leaf
        if(is_found)                                 // when found
            return _subset[i + 1]->find_ptr(entry);  // recurse i+1
        else                                         // when !found
            return _subset[i]->find_ptr(entry);      // recurse to find entry
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Verifies BPTree structure:
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
bool BPTree<T>::verify_tree(int& height, bool& has_stored_height,
                            int level) const {
    using namespace array_utils;

    // verify data count limits
    if(level && (_data_count < _min || _data_count > _max)) return false;

    // verify data is sorted
    if(!sort::verify(_data, _data_count)) return false;

    if(!is_leaf()) {
        // verify child count limits
        if(_child_count > _max + 1 || _child_count != _data_count + 1)
            return false;

        for(std::size_t i = 0; i < _child_count; ++i) {
            if(i + 1 < _child_count) {
                // verify that data[i] exists in one of the subset
                if(!contains(_data[i])) return false;

                // verify data[i] is greater than all of subset[i]
                if(!is_gt_subset(_subset[i], _data[i])) return false;

                // verify data[i] is less than all of subset[i+1]
                if(!is_le_subset(_subset[i + 1], _data[i])) return false;
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
 *  const BPTree<T>* subtree: BPTree to recursively check
 *  const T& item          : item to compare
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool BPTree<T>::is_gt_subset(const BPTree<T>* subtree, const T& item) const {
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
 *  const BPTree<T>* subtree: BPTree to recursively check
 *  const T& item          : item to compare
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool BPTree<T>::is_le_subset(const BPTree<T>* subtree, const T& item) const {
    if(!array_utils::is_le(subtree->_data, subtree->_data_count, item))
        return false;

    for(std::size_t i = 0; i < subtree->_child_count; ++i)
        if(!is_le_subset(subtree->_subset[i], item)) return false;

    return true;
}

}  // namespace bptree

#endif  // BPTREE_H
