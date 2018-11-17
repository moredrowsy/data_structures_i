/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : open_hash
 * DESCRIPTION : This header defines a templated OpenHash. Item insertions are
 *      done via record type's _key. If key's hash clashes with previous key,
 *      then insertion attempts next +1 position until it finds an empty spot.
 *      Item insertions are limited to hash table size. Table size for hash
 *      class uses dynamic allocation for user specificed table size.
 *
 *      NOTE: Requires T's _key as unsigned for hash insert!
 ******************************************************************************/
#ifndef OPEN_HASH_H
#define OPEN_HASH_H

#include <cassert>   // assert()
#include <iomanip>   // setw()
#include <iostream>  // stream objects
#include <string>    // string objects

namespace open_hash {

template <typename T>
class OpenHash {
    enum { PREVIOUSLY_USED = -2, NEVER_USED = -1, TABLE_SIZE = 811 };

public:
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
    void clear();                 // remove all items
    bool insert(const T& entry);  // insert key, value pair
    bool remove(int key);         // remove this key

    // FRIENDS
    // print entire table with keys, etc.
    friend std::ostream& operator<<(std::ostream& outs, const OpenHash<T>& h) {
        return h.print(outs);
    }

private:
    std::size_t _table_size;     // capacity _data array
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

/*******************************************************************************
 * DESCRIPTION:
 *  Constructor with default size argument to TABLE_SIZE. Dynamically allocate
 *  _data array with _table_size attribute.
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
OpenHash<T>::OpenHash(std::size_t size)
    : _table_size(size), _collisions(0), _total_records(0), _data(nullptr) {
    assert(_table_size > 0);
    _data = new T[_table_size];
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
OpenHash<T>::~OpenHash() {
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
OpenHash<T>::OpenHash(const OpenHash<T>& src)
    : _table_size(src._table_size),
      _collisions(src._collisions),
      _total_records(src._total_records),
      _data(nullptr) {
    _data = new T[_table_size];
    for(std::size_t i = 0; i < _table_size; ++i) _data[i] = src._data[i];
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
OpenHash<T>& OpenHash<T>::operator=(const OpenHash<T>& rhs) {
    if(this != &rhs) {
        delete[] _data;
        _table_size = rhs._table_size;
        _collisions = rhs._collisions;
        _total_records = rhs._total_records;

        _data = new T[_table_size];
        for(std::size_t i = 0; i < _table_size; ++i) _data[i] = rhs._data[i];
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
std::size_t OpenHash<T>::capacity() const {
    return _table_size;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Access to _collisions
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
std::size_t OpenHash<T>::collisions() const {
    return _collisions;
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
std::size_t OpenHash<T>::size() const {
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
bool OpenHash<T>::find(int key, T& result) const {
    bool is_found = false;
    std::size_t i = 0;

    if(find_index(key, i)) {
        result = _data[i];
        is_found = true;
    }

    return is_found;
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
bool OpenHash<T>::is_collision(int key, std::size_t i) const {
    return hash(key) != i;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Find if _data table has given key occupied.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  table output
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool OpenHash<T>::is_present(int key) const {
    std::size_t i;
    return find_index(key, i);
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
std::ostream& OpenHash<T>::print(std::ostream& outs) const {
    std::size_t size = std::to_string(_table_size).size();

    outs << std::setfill('0');
    for(std::size_t i = 0; i < _table_size; ++i) {
        outs << "[" << std::setw(size) << std::right << i << "] ";

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

/*******************************************************************************
 * DESCRIPTION:
 *  Clear all data by assigning templated item with default constructor
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  _collisions = 0
 *  _total_records = 0
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void OpenHash<T>::clear() {
    if(_data)
        for(std::size_t i = 0; i < _table_size; ++i) _data[i] = T();

    _collisions = 0;
    _total_records = 0;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Insert templated item into _data table via its _key
 *
 * PRE-CONDITIONS:
 *  _total_records < _table_size
 *
 * POST-CONDITIONS:
 *  _collisions + 1 if entry's key is not hash value
 *  _total_records + 1
 *  item added into _data if success
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool OpenHash<T>::insert(const T& entry) {
    assert(entry._key >= 0);

    std::size_t i;

    if(!find_index(entry._key, i)) {  // <--- see find_index's notes!
        // EXIT and return false when hash key not found and FULL TABLE
        if(_total_records >= _table_size) return false;

        while(!is_vacant(i)) i = next_index(i);
        ++_total_records;
    }

    if(is_collision(entry._key, i)) ++_collisions;

    _data[i] = entry;

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
 *  _collisions - 1 if entry's key is not hash value
 *  _total_records - 1
 *  item removed from _data if success
 *
 * RETURN:
 *  bool
 ******************************************************************************/
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
    return key % _table_size;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Searches _data array with key until it finds KEY position or NEVER_USED
 *  position. Does NOT return PREVIOUSLY_USED position. If KEY position is
 *  PREVIOUSLY_USED (which is vacant), it will NOT return vacant position but
 *  the next cell marked NEVER_USED.
 *
 *  EX: Insert @ 0, [0] = OCCUPIED. Remove @ 0, [0] = PREVIOUSLY_USED.
 *      Find_index returns i = 1 because [1] = NEVER_USED.
 *
 * PRE-CONDITIONS:
 *  int key: >= 0
 *
 * POST-CONDITIONS:
 *  std::size_t& i: @NEVER_USED, @KEY's or @original hash if not found
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool OpenHash<T>::find_index(int key, std::size_t& i) const {
    std::size_t count = 0;
    i = hash(key);

    while(count < _table_size && !never_used(i) && _data[i]._key != key) {
        ++count;
        i = next_index(i);
    }

    return _data[i]._key == key;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Return's next hash position.
 *
 * PRE-CONDITIONS:
 *  std::size_t i: array index >= 0
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  std::size_t: array index
 ******************************************************************************/
template <typename T>
std::size_t OpenHash<T>::next_index(std::size_t i) const {
    return ++i % _table_size;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Check for array index has never been used.
 *
 * PRE-CONDITIONS:
 *  std::size_t i: array index >= 0
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool OpenHash<T>::never_used(std::size_t i) const {
    return _data[i]._key == NEVER_USED;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Check for array index is free / not occupied.
 *
 * PRE-CONDITIONS:
 *  std::size_t i: array index >= 0
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool OpenHash<T>::is_vacant(std::size_t i) const {
    return _data[i]._key <= NEVER_USED;
}
}  // namespace open_hash

#endif  // OPEN_HASH_H
