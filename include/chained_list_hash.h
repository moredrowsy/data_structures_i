/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : chained_list_hash
 * DESCRIPTION : This header defines a templated ChainedHash via singly Linked
 *      List. Table size for hash class uses dynamic allocation for user
 *      specificed table size.
 ******************************************************************************/
#ifndef CHAINED_LIST_HASH_H
#define CHAINED_LIST_HASH_H

#include <cassert>   // assert()
#include <iomanip>   // setw()
#include <iostream>  // stream objects
#include <string>    // string objects
#include "list.h"    // List class

namespace chained_list_hash {

template <typename T>
class ChainedListHash {
public:
    enum { TABLE_SIZE = 811 };

    // CONSTRUCTORS
    ChainedListHash(std::size_t size = TABLE_SIZE);

    // BIG THREE
    ~ChainedListHash();
    ChainedListHash(const ChainedListHash<T>& src);
    ChainedListHash<T>& operator=(const ChainedListHash<T>& rhs);

    // ACCESSORS
    std::size_t capacity() const;                 // total unique entries
    std::size_t collisions() const { return 0; }  // dummy function
    std::size_t size() const;                     // number of keys in the table
    bool find(int key, T& result) const;          // result <- record with key
    std::ostream& print(std::ostream& outs = std::cout) const;

    // MUTATORS
    void clear();                 // remove all items
    bool insert(const T& entry);  // insert key, value pair
    bool remove(int key);         // remove this key

    // FRIENDS
    // print entire table with keys, etc.
    friend std::ostream& operator<<(std::ostream& outs,
                                    const ChainedListHash<T>& h) {
        return h.print(outs);
    }

private:
    std::size_t _TABLE_SIZE;     // capacity _data array
    std::size_t _total_records;  // number of keys in the table
    list::List<T>* _data;        // table of singly linked list of Records

    // ACCESSORS
    inline std::size_t hash(int key) const;  // hash function
};

/*******************************************************************************
 * DESCRIPTION:
 *  Constructor with default size argument to TABLE_SIZE. Dynamically allocate
 *  _data array with _TABLE_SIZE attribute.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  new memory allocated to _data
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
ChainedListHash<T>::ChainedListHash(std::size_t size)
    : _TABLE_SIZE(size), _total_records(0), _data(nullptr) {
    assert(_TABLE_SIZE > 0);
    _data = new list::List<T>[_TABLE_SIZE];
}

/*******************************************************************************
 * DESCRIPTION:
 *  Deallocate all heap data.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  delete memory allocated to _data
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
ChainedListHash<T>::~ChainedListHash() {
    delete[] _data;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Copy constructor
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  new memory allocated to _data
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
ChainedListHash<T>::ChainedListHash(const ChainedListHash<T>& src)
    : _TABLE_SIZE(src._TABLE_SIZE),
      _total_records(src._total_records),
      _data(nullptr) {
    _data = new list::List<T>[_TABLE_SIZE];
    for(std::size_t i = 0; i < _TABLE_SIZE; ++i) _data[i] = src._data[i];
}

/*******************************************************************************
 * DESCRIPTION:
 *  Assginmen operator.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  delete old _data and new memory allocated to _data
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
ChainedListHash<T>& ChainedListHash<T>::operator=(
    const ChainedListHash<T>& rhs) {
    if(this != &rhs) {
        delete[] _data;
        _TABLE_SIZE = rhs._TABLE_SIZE;
        _total_records = rhs._total_records;

        _data = new list::List<T>[_TABLE_SIZE];
        for(std::size_t i = 0; i < _TABLE_SIZE; ++i) _data[i] = rhs._data[i];
    }

    return *this;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Access to _capacity
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
std::size_t ChainedListHash<T>::capacity() const {
    return _TABLE_SIZE;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Access to _total_records
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
std::size_t ChainedListHash<T>::size() const {
    return _total_records;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Find templated item with key.
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
bool ChainedListHash<T>::find(int key, T& result) const {
    typename list::List<T>::Iterator search;

    search = _data[hash(key)].search(T(key));

    if(search) result = *search;

    return search;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Print _data into table format.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  table output
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
std::ostream& ChainedListHash<T>::print(std::ostream& outs) const {
    std::size_t size = std::to_string(_TABLE_SIZE).size();

    outs << std::setfill('0');
    for(std::size_t i = 0; i < _TABLE_SIZE; ++i)
        outs << "[" << std::setw(size) << std::right << i << "] " << _data[i]
             << std::endl;

    return outs;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Clear all data by assigning templated item with default constructor
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  _total_records = 0
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void ChainedListHash<T>::clear() {
    if(_data)
        for(std::size_t i = 0; i < _TABLE_SIZE; ++i) _data[i].clear();

    _total_records = 0;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Insert templated item into _data table via its _key
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  _total_records + 1
 *  item added into _data
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool ChainedListHash<T>::insert(const T& entry) {
    assert(entry._key >= 0);

    typename list::List<T>::Iterator search;
    search = _data[hash(entry._key)].search(entry);

    if(search)
        *search = entry;
    else
        _data[hash(entry._key)].push_back(entry);

    ++_total_records;

    return true;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Remove templated item from _data table via its _key
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  _total_records - 1
 *  item removed from _data if success
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool ChainedListHash<T>::remove(int key) {
    bool is_removed = _data[hash(key)].remove(T(key));
    if(is_removed) --_total_records;

    return is_removed;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Convert key to index position via mod TABLE_SIZE. Recommended TABLE_SIZE
 *  should be 4k + 3 and prime number.
 *
 * PRE-CONDITIONS:
 *  int key: >= 0
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  std::size_t: array index
 ******************************************************************************/
template <typename T>
std::size_t ChainedListHash<T>::hash(int key) const {
    return key % _TABLE_SIZE;
}

}  // namespace chained_list_hash

#endif  // CHAINED_LIST_HASH_H
