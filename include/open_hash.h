#ifndef OPEN_HASH_H
#define OPEN_HASH_H

#include <cassert>
#include <iomanip>
#include <iostream>
#include <string>

namespace open_hash {

template <typename T>
class OpenHash {
public:
    enum { PREVIOUSLY_USED = -2, NEVER_USED = -1, TABLE_SIZE = 811 };

    // CONSTRUCTORS
    OpenHash(std::size_t size = TABLE_SIZE);

    // BIG THREE
    ~OpenHash();
    OpenHash(const OpenHash<T>& src);
    OpenHash<T>& operator=(const OpenHash<T>& rhs);

    // ACCESSORS
    std::size_t capacity() const;         // total unique entries
    std::size_t collisions() const;       // number of collisions
    std::size_t size() const;             // number of keys in the table
    bool find(int key, T& result) const;  // result <- record with key
    bool is_collision(int key, std::size_t i) const;  // is key collidded?
    bool is_present(int key) const;  // is this key present in table?
    std::ostream& print(std::ostream& outs = std::cout) const;

    // MUTATORS
    bool insert(const T& entry);  // insert key, value pair
    bool remove(int key);         // remove this key

    // FRIENDS
    // print entire table with keys, etc.
    friend std::ostream& operator<<(std::ostream& outs, const OpenHash<T>& h) {
        return h.print(outs);
    }

private:
    std::size_t _TABLE_SIZE;     // capacity _data array
    std::size_t _collisions;     // number of entry collisions
    std::size_t _total_records;  // number of keys in the table
    T* _data;                    // table of Records

    // ACCESSORS
    inline std::size_t hash(int key) const;  // hash function
    inline bool find_index(int key, std::size_t& i) const;
    inline std::size_t next_index(std::size_t i) const;
    inline bool never_used(std::size_t i) const;
    inline bool is_vacant(std::size_t i) const;
};

template <typename T>
OpenHash<T>::OpenHash(std::size_t size)
    : _TABLE_SIZE(size), _collisions(0), _total_records(0), _data(nullptr) {
    assert(_TABLE_SIZE > 0);
    _data = new T[size];
}

template <typename T>
OpenHash<T>::~OpenHash() {
    delete[] _data;
}

template <typename T>
OpenHash<T>::OpenHash(const OpenHash<T>& src)
    : _TABLE_SIZE(src._TABLE_SIZE),
      _collisions(src._collisions),
      _total_records(src._total_records),
      _data(nullptr) {
    _data = new T[_TABLE_SIZE];
    for(std::size_t i = 0; i < _TABLE_SIZE; ++i) _data[i] = src._data[i];
}

template <typename T>
OpenHash<T>& OpenHash<T>::operator=(const OpenHash<T>& rhs) {
    if(this != &rhs) {
        delete[] _data;
        _TABLE_SIZE = rhs._TABLE_SIZE;
        _collisions = rhs._collisions;
        _total_records = rhs._total_records;

        _data = new T[_TABLE_SIZE];
        for(std::size_t i = 0; i < _TABLE_SIZE; ++i) _data[i] = rhs._data[i];
    }
}

template <typename T>
std::size_t OpenHash<T>::capacity() const {
    return _TABLE_SIZE;
}

template <typename T>
std::size_t OpenHash<T>::collisions() const {
    return _collisions;
}

template <typename T>
std::size_t OpenHash<T>::size() const {
    return _total_records;
}

template <typename T>
bool OpenHash<T>::find(int key, T& result) const {
    bool is_found = false;
    std::size_t i = -1;

    if(find_index(key, i)) {
        result = _data[i];
        is_found = true;
    }

    return is_found;
}

template <typename T>
bool OpenHash<T>::is_collision(int key, std::size_t i) const {
    return hash(key) != i;
}

template <typename T>
bool OpenHash<T>::is_present(int key) const {
    std::size_t count = 0, i = hash(key);

    while(count < _TABLE_SIZE && !never_used(i) && _data[i]._key != key) {
        ++count;
        i = next_index(i);
    }

    return _data[i]._key == key;
}
template <typename T>
std::ostream& OpenHash<T>::print(std::ostream& outs) const {
    std::size_t size = std::to_string(_TABLE_SIZE).size();

    outs << std::setfill('0');
    for(std::size_t i = 0; i < _TABLE_SIZE; ++i) {
        outs << "[" << std::setw(size) << i << "] ";

        if(_data[i]._key > NEVER_USED) {
            outs << _data[i] << " (" << std::setw(size) << hash(_data[i]._key)
                 << ")";

            if(is_collision(_data[i]._key, i)) outs << " *";
        }

        if(_data[i]._key == PREVIOUSLY_USED) outs << "-----";

        outs << std::endl;
    }

    return outs;
}

template <typename T>
bool OpenHash<T>::insert(const T& entry) {
    assert(entry._key >= 0);

    bool is_inserted = false;
    std::size_t i;

    if(!find_index(entry._key, i)) {
        // EXIT and return false when hash key not found and FULL TABLE
        if(_total_records >= _TABLE_SIZE) return false;

        while(!is_vacant(i)) i = next_index(i);
        ++_total_records;
        is_inserted = true;
    }

    if(is_collision(entry._key, i)) ++_collisions;

    _data[i] = entry;

    return is_inserted;
}

template <typename T>
bool OpenHash<T>::remove(int key) {
    bool is_removed = false;
    std::size_t i = -1;

    if(find_index(key, i)) {
        if(is_collision(key, i)) --_collisions;

        _data[i]._key = PREVIOUSLY_USED;
        --_total_records;
        is_removed = true;
    }

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
std::size_t OpenHash<T>::hash(int key) const {
    return key % _TABLE_SIZE;
}

template <typename T>
bool OpenHash<T>::find_index(int key, std::size_t& i) const {
    std::size_t count = 0;
    i = hash(key);

    while(count < _TABLE_SIZE && !never_used(i) && _data[i]._key != key) {
        ++count;
        i = next_index(i);
    }

    return _data[i]._key == key;
}

template <typename T>
std::size_t OpenHash<T>::next_index(std::size_t i) const {
    return ++i % _TABLE_SIZE;
}

template <typename T>
bool OpenHash<T>::never_used(std::size_t i) const {
    return _data[i]._key == NEVER_USED;
}

template <typename T>
bool OpenHash<T>::is_vacant(std::size_t i) const {
    return _data[i]._key <= NEVER_USED;
}
}  // namespace open_hash

#endif  // OPEN_HASH_H
