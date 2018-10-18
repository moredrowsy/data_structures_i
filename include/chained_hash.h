#ifndef CHAINED_HASH_H
#define CHAINED_HASH_H

#include <cassert>
#include <iomanip>
#include <iostream>
#include <string>
#include "list.h"

namespace chained_hash {

template <typename T>
class ChainedHash {
public:
    enum { TABLE_SIZE = 811 };

    // CONSTRUCTORS
    ChainedHash(std::size_t size = TABLE_SIZE);

    // BIG THREE
    ~ChainedHash();
    ChainedHash(const ChainedHash<T>& src);
    ChainedHash<T>& operator=(const ChainedHash<T>& rhs);

    // ACCESSORS
    std::size_t capacity() const;         // total unique entries
    std::size_t size() const;             // number of keys in the table
    bool find(int key, T& result) const;  // result <- record with key
    std::ostream& print(std::ostream& outs = std::cout) const;

    // MUTATORS
    void clear();                 // remove all items
    bool insert(const T& entry);  // insert key, value pair
    bool remove(int key);         // remove this key

    // FRIENDS
    // print entire table with keys, etc.
    friend std::ostream& operator<<(std::ostream& outs,
                                    const ChainedHash<T>& h) {
        return h.print(outs);
    }

private:
    std::size_t _TABLE_SIZE;     // capacity _data array
    std::size_t _total_records;  // number of keys in the table
    list::List<T>* _data;        // table of linked list of Records

    // ACCESSORS
    inline std::size_t hash(int key) const;  // hash function
};

template <typename T>
ChainedHash<T>::ChainedHash(std::size_t size)
    : _TABLE_SIZE(size), _total_records(0), _data(nullptr) {
    assert(_TABLE_SIZE > 0);
    _data = new list::List<T>[size];
}

template <typename T>
ChainedHash<T>::~ChainedHash() {
    delete[] _data;
}

template <typename T>
ChainedHash<T>::ChainedHash(const ChainedHash<T>& src)
    : _TABLE_SIZE(src._TABLE_SIZE),
      _total_records(src._total_records),
      _data(nullptr) {
    _data = new list::List<T>[_TABLE_SIZE];
    for(std::size_t i = 0; i < _TABLE_SIZE; ++i) _data[i] = src._data[i];
}

template <typename T>
ChainedHash<T>& ChainedHash<T>::operator=(const ChainedHash<T>& rhs) {
    if(this != &rhs) {
        delete[] _data;
        _TABLE_SIZE = rhs._TABLE_SIZE;
        _total_records = rhs._total_records;

        _data = new list::List<T>[_TABLE_SIZE];
        for(std::size_t i = 0; i < _TABLE_SIZE; ++i) _data[i] = rhs._data[i];
    }
}

template <typename T>
std::size_t ChainedHash<T>::capacity() const {
    return _TABLE_SIZE;
}

template <typename T>
std::size_t ChainedHash<T>::size() const {
    return _total_records;
}

template <typename T>
bool ChainedHash<T>::find(int key, T& result) const {
    typename list::List<T>::Iterator search;

    search = _data[hash(key)].search(T(key));

    if(search) result = *search;

    return search;
}

template <typename T>
std::ostream& ChainedHash<T>::print(std::ostream& outs) const {
    std::size_t size = std::to_string(_TABLE_SIZE).size();

    outs << std::setfill('0');
    for(std::size_t i = 0; i < _TABLE_SIZE; ++i)
        outs << "[" << std::setw(size) << std::right << i << "] " << _data[i]
             << std::endl;

    return outs;
}

template <typename T>
void ChainedHash<T>::clear() {
    if(_data)
        for(std::size_t i = 0; i < _TABLE_SIZE; ++i) _data[i].clear();

    _total_records = 0;
}

template <typename T>
bool ChainedHash<T>::insert(const T& entry) {
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

template <typename T>
bool ChainedHash<T>::remove(int key) {
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
std::size_t ChainedHash<T>::hash(int key) const {
    return key % _TABLE_SIZE;
}

}  // namespace chained_hash

#endif  // CHAINED_HASH_H
