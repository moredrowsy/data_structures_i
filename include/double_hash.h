#ifndef DOUBLE_HASH_H
#define DOUBLE_HASH_H

#include <cassert>
#include <iomanip>
#include <iostream>
#include <string>

namespace double_hash {

template <typename T>
class DoubleHash {
public:
    enum { PREVIOUSLY_USED = -2, NEVER_USED = -1, TABLE_SIZE = 811 };

    // CONSTRUCTORS
    DoubleHash(std::size_t size = TABLE_SIZE);

    // BIG THREE
    ~DoubleHash();
    DoubleHash(const DoubleHash<T>& src);
    DoubleHash<T>& operator=(const DoubleHash<T>& rhs);

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
    friend std::ostream& operator<<(std::ostream& outs,
                                    const DoubleHash<T>& h) {
        return h.print(outs);
    }

private:
    std::size_t _TABLE_SIZE;     // capacity _data array
    std::size_t _collisions;     // number of entry collisions
    std::size_t _total_records;  // number of keys in the table
    T* _data;                    // table of Records

    // ACCESSORS
    inline std::size_t hash1(int key) const;  // hash1 function
    inline std::size_t hash2(int key) const;  // hash2 function
    inline bool find_index(int key, std::size_t& i) const;
    inline std::size_t next_index(int key, std::size_t i) const;
    inline bool never_used(std::size_t i) const;
    inline bool is_vacant(std::size_t i) const;
};

template <typename T>
DoubleHash<T>::DoubleHash(std::size_t size)
    : _TABLE_SIZE(size), _collisions(0), _total_records(0), _data(nullptr) {
    assert(_TABLE_SIZE > 0);
    _data = new T[size];
}

template <typename T>
DoubleHash<T>::~DoubleHash() {
    delete[] _data;
}

template <typename T>
DoubleHash<T>::DoubleHash(const DoubleHash<T>& src)
    : _TABLE_SIZE(src._TABLE_SIZE),
      _collisions(src._collisions),
      _total_records(src._total_records),
      _data(nullptr) {
    _data = new T[_TABLE_SIZE];
    for(std::size_t i = 0; i < _TABLE_SIZE; ++i) _data[i] = src._data[i];
}

template <typename T>
DoubleHash<T>& DoubleHash<T>::operator=(const DoubleHash<T>& rhs) {
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
std::size_t DoubleHash<T>::capacity() const {
    return _TABLE_SIZE;
}

template <typename T>
std::size_t DoubleHash<T>::collisions() const {
    return _collisions;
}

template <typename T>
std::size_t DoubleHash<T>::size() const {
    return _total_records;
}

template <typename T>
bool DoubleHash<T>::find(int key, T& result) const {
    bool is_found = false;
    std::size_t i = -1;

    if(find_index(key, i)) {
        result = _data[i];
        is_found = true;
    }

    return is_found;
}

template <typename T>
bool DoubleHash<T>::is_collision(int key, std::size_t i) const {
    return hash1(key) != i;
}

template <typename T>
bool DoubleHash<T>::is_present(int key) const {
    std::size_t count = 0, i = hash1(key);

    while(count < _TABLE_SIZE && !never_used(i) && _data[i]._key != key) {
        ++count;
        i = next_index(key, i);
    }

    return _data[i]._key == key;
}

template <typename T>
std::ostream& DoubleHash<T>::print(std::ostream& outs) const {
    std::size_t size = std::to_string(_TABLE_SIZE).size();

    outs << std::setfill('0');
    for(std::size_t i = 0; i < _TABLE_SIZE; ++i) {
        outs << "[" << std::setw(size) << i << "] ";

        if(_data[i]._key > NEVER_USED) {
            outs << _data[i] << " (" << std::setw(size) << hash1(_data[i]._key)
                 << ")";

            if(is_collision(_data[i]._key, i)) outs << " *";
        }

        if(_data[i]._key == PREVIOUSLY_USED) outs << "-----";

        outs << std::endl;
    }

    return outs;
}

template <typename T>
bool DoubleHash<T>::insert(const T& entry) {
    assert(entry._key >= 0);

    bool is_inserted = false;
    std::size_t i;

    if(!find_index(entry._key, i)) {
        // EXIT and return false when hash key not found and FULL TABLE
        if(_total_records >= _TABLE_SIZE) return false;

        while(!is_vacant(i)) i = next_index(entry._key, i);
        ++_total_records;
        is_inserted = true;
    }

    if(is_collision(entry._key, i)) ++_collisions;

    _data[i] = entry;

    return is_inserted;
}

template <typename T>
bool DoubleHash<T>::remove(int key) {
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
 *  Convert key to index position via mod _TABLE_SIZE. Recommended _TABLE_SIZE
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
std::size_t DoubleHash<T>::hash1(int key) const {
    return key % _TABLE_SIZE;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Convert key to second hash value to add with original hash1 value.
 *  Hash2's value must be relatively prime to _TABLE_SIZE. In addition,
 *  _TABLE_SIZE and (_TABLE_SIZE - 2) must also be twin primes.
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
std::size_t DoubleHash<T>::hash2(int key) const {
    return 1 + (key % (_TABLE_SIZE - 2));
}

template <typename T>
bool DoubleHash<T>::find_index(int key, std::size_t& i) const {
    std::size_t count = 0;
    i = hash1(key);

    while(count < _TABLE_SIZE && !never_used(i) && _data[i]._key != key) {
        ++count;
        i = next_index(key, i);
    }

    return _data[i]._key == key;
}

template <typename T>
std::size_t DoubleHash<T>::next_index(int key, std::size_t i) const {
    return (i + hash2(key)) % _TABLE_SIZE;
}

template <typename T>
bool DoubleHash<T>::never_used(std::size_t i) const {
    return _data[i]._key == NEVER_USED;
}

template <typename T>
bool DoubleHash<T>::is_vacant(std::size_t i) const {
    return _data[i]._key <= NEVER_USED;
}
}  // namespace double_hash

#endif  // DOUBLE_HASH_H
