/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : bpt_map
 * DESCRIPTION : This header provides a templated BPTree (B+Tree) version
 *          of the Map/MMap class, along with Pair/MPair as the
 *          const key/value(s) structure.
 *
 *          Map uses BPTree as base with Pair. It does not allow duplicate keys
 *          but allow value modification.
 *          MMap uses BPTree as base with MPair. It does not allow duplicate
 *          keys but allow duplicate values. The values are stored in
 *          key/vector structure.
 *
 *          Map::Iterator returns Pair, with operator-> access to key/value.
 *          MMap::Iterator returns MPair with operator-> access to key/value
 *          or key/values. Increment of iterators for MMap::Iterator cycles
 *          value per key and then increment to next key.
 ******************************************************************************/
#ifndef BPT_MAP_H
#define BPT_MAP_H

#include <vector>    // vector objects
#include "bptree.h"  // BPTree class
#include "pair.h"    // Pair struct

namespace bpt_map {

template <typename K, typename V>
class Map {
public:
    typedef pair::Pair<const K, V> Pair;
    typedef bptree::BPTree<Pair> MapBase;
    typedef typename bptree::BPTree<Pair>::Iterator MapBaseIter;

    class Iterator {
    public:
        friend class Map;

        // CONSTRUCTOR
        Iterator(MapBaseIter it = MapBaseIter(nullptr)) : _it(it) {}

        bool is_null() { return !_it; }
        explicit operator bool() { return (bool)_it; }

        Pair& operator*() { return *_it; }    // member access
        Pair* operator->() { return &*_it; }  // member access

        Iterator& operator++() {  // pre-inc
            ++_it;
            return *this;
        }

        Iterator operator++(int _u) {  // post-inc
            (void)_u;                  // suppress unused warning
            Iterator it = *this;       // make temp
            operator++();              // pre-inc
            return it;                 // return previous state
        }

        void next_key() {
            if(_it) _it.next_key();
        }

        // FRIENDS
        friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
            return lhs._it == rhs._it;
        }

        friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
            return lhs._it != rhs._it;
        }

    private:
        MapBaseIter _it;
    };

    // CONSTRUCTOR
    Map(std::size_t min = bptree::MINIMUM) : _map(true, min) {}

    // capacity
    std::size_t size() const;
    bool empty() const;

    // element access
    Iterator begin();
    Iterator end();
    Iterator find(const K& key);
    Pair& front();
    Pair& back();
    const V& operator[](const K& key) const;
    V& operator[](const K& key);
    const V& at(const K& key) const;
    V& at(const K& key);

    // modifiers
    bool insert(const K& k, const V& v);
    bool erase(const K& key);
    void clear();
    V& get(const K& key);

    // operations
    bool contains(const K& key) const;
    bool contains(const Pair& target) const;
    std::size_t count(const K& key) const;
    void print_debug() const;
    bool verify() const;

    friend std::ostream& operator<<(std::ostream& outs, const Map<K, V>& map) {
        return outs << map._map;
    }

private:
    MapBase _map;
};

template <typename K, typename V>
class MMap {
public:
    typedef pair::MPair<const K, V> MPair;
    typedef bptree::BPTree<MPair> MMapBase;
    typedef typename bptree::BPTree<MPair>::Iterator MMapBaseIter;

    class Iterator {
    public:
        friend class MMap;

        // CONSTRUCTOR
        Iterator(MMapBaseIter it = MMapBaseIter(nullptr)) : _it(it) {
            // Initialize MPair's vector iter to begin()
            if(_it) _it->value = _it->values.begin();
        }

        bool is_null() { return !_it; }
        explicit operator bool() { return (bool)_it; }

        MPair& operator*() { return *_it; }    // member access
        MPair* operator->() { return &*_it; }  // member access

        Iterator& operator++() {  // pre-inc
            // _it->value is values' iter; inc values' iter and cmp to end()
            if(++_it->value == _it->values.end()) {  // if end(), inc entire
                ++_it;                               // B+Tree's iter
                if(_it) _it->value = _it->values.begin();  // init values's iter
            }
            return *this;
        }

        Iterator operator++(int _u) {  // post-inc
            (void)_u;                  // suppress unused warning
            Iterator it = *this;       // make temp
            operator++();              // pre-inc
            return it;                 // return previous state
        }

        void next_key() {
            if(_it) {
                _it.next_key();
                _it->value = _it->values.begin();
            }
        }

        // FRIENDS
        friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
            return lhs._it == rhs._it;
        }

        friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
            return lhs._it != rhs._it;
        }

    private:
        MMapBaseIter _it;
    };

    MMap(std::size_t min = bptree::MINIMUM) : _mmap(true, min) {}

    // capacity
    std::size_t size() const;
    bool empty() const;

    // element access
    Iterator begin();
    Iterator end();
    Iterator find(const K& key);
    const std::vector<V>& operator[](const K& key) const;
    std::vector<V>& operator[](const K& key);
    const std::vector<V>& at(const K& key) const;
    std::vector<V>& at(const K& key);

    // modifiers
    bool insert(const K& k, const V& v);
    bool erase(const K& key);
    void clear();
    std::vector<V>& get(const K& key);

    // operations
    bool contains(const K& key) const;
    std::size_t count(const K& key) const;
    void print_debug() const;
    bool verify() const;

    friend std::ostream& operator<<(std::ostream& outs, const MMap<K, V>& map) {
        return outs << map._mmap;
    }

private:
    MMapBase _mmap;
};

// ----- MAP IMPLEMENTATIONS -----

/*******************************************************************************
 * DESCRIPTION:
 *  Returns the total keys in map.
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
template <typename K, typename V>
std::size_t Map<K, V>::size() const {
    return _map.size();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Checks if the map is empty.
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
template <typename K, typename V>
bool Map<K, V>::empty() const {
    return _map.empty();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Points to left most element in map.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  Map<K, V>::Iterator: points to left most element
 ******************************************************************************/
template <typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::begin() {
    return Map<K, V>::Iterator(_map.begin());
}

/*******************************************************************************
 * DESCRIPTION:
 *  Points to end of map, which is nullptr.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  Map<K, V>::Iterator: points to left most element
 ******************************************************************************/
template <typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::end() {
    return Map<K, V>::Iterator(_map.end());
}

/*******************************************************************************
 * DESCRIPTION:
 *  Return iterator that points to Pair that matches key.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  Map<K, V>::Iterator: points Pair that matches key
 ******************************************************************************/
template <typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::find(const K& key) {
    return Map<K, V>::Iterator(_map.find(Pair(key)));
}

/*******************************************************************************
 * DESCRIPTION:
 *  Points to end of map, which is nullptr.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  Map<K, V>::Iterator: points to left most element
 ******************************************************************************/
template <typename K, typename V>
typename Map<K, V>::Pair& Map<K, V>::front() {
    return _map.front();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Points to end of map, which is nullptr.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  Map<K, V>::Iterator: points to left most element
 ******************************************************************************/
template <typename K, typename V>
typename Map<K, V>::Pair& Map<K, V>::back() {
    return _map.back();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns the reference value at given key via subscript operator.
 *
 * PRE-CONDITIONS:
 *  const K& key: key to find Pair
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  const V&
 ******************************************************************************/
template <typename K, typename V>
const V& Map<K, V>::operator[](const K& key) const {
    return _map.get(Pair(key)).value;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns the reference value at given key via subscript operator.
 *
 * PRE-CONDITIONS:
 *  const K& key: key to find Pair
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  V&
 ******************************************************************************/
template <typename K, typename V>
V& Map<K, V>::operator[](const K& key) {
    return _map.get(Pair(key)).value;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns the reference value at given key.
 *
 * PRE-CONDITIONS:
 *  const K& key: key to find Pair
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  const V&
 ******************************************************************************/
template <typename K, typename V>
const V& Map<K, V>::at(const K& key) const {
    return _map.get(Pair(key)).value;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns the reference value at given key.
 *
 * PRE-CONDITIONS:
 *  const K& key: key to find Pair
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  V&
 ******************************************************************************/
template <typename K, typename V>
V& Map<K, V>::at(const K& key) {
    return _map.get(Pair(key)).value;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Inserts Pair into map.
 *
 * PRE-CONDITIONS:
 *  const K& k: key for Pair
 *  const V& v: value for pair
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename K, typename V>
bool Map<K, V>::insert(const K& k, const V& v) {
    return _map.insert(Pair(k, v));
}

/*******************************************************************************
 * DESCRIPTION:
 *  Erase Pair from map.
 *
 * PRE-CONDITIONS:
 *  const K& k: key for Pair
 *  const V& v: value for pair
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename K, typename V>
bool Map<K, V>::erase(const K& key) {
    return _map.remove(Pair(key));
}

/*******************************************************************************
 * DESCRIPTION:
 *  Remove all elements in map.
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
template <typename K, typename V>
void Map<K, V>::clear() {
    _map.clear();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns the reference value at given key.
 *
 * PRE-CONDITIONS:
 *  const K& key: key to find Pair
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  V&
 ******************************************************************************/
template <typename K, typename V>
V& Map<K, V>::get(const K& key) {
    return _map.get(Pair(key)).value;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Checks if key is contained in Map.
 *
 * PRE-CONDITIONS:
 *  const K& key: key to find Pair
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename K, typename V>
bool Map<K, V>::contains(const K& key) const {
    return _map.contains(Pair(key));
}

/*******************************************************************************
 * DESCRIPTION:
 *  Checks if Pair is contained in Map.
 *
 * PRE-CONDITIONS:
 *  const Pair& target: the Pair to find
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename K, typename V>
bool Map<K, V>::contains(const Pair& target) const {
    return _map.contains(target);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns the number of elements for given key.
 *
 * PRE-CONDITIONS:
 *  const K& key: key to find Pair
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  std::size_t
 ******************************************************************************/
template <typename K, typename V>
std::size_t Map<K, V>::count(const K& key) const {
    return _map.find(MPair(key)) ? 1 : 0;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Print MapBase with debug.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  void
 ******************************************************************************/
template <typename K, typename V>
void Map<K, V>::print_debug() const {
    _map.print(std::cout, true);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Checks if map base's structure is valid.
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
template <typename K, typename V>
bool Map<K, V>::verify() const {
    return _map.verify();
}

// ----- MMAP IMPLEMENTATIONS -----

/*******************************************************************************
 * DESCRIPTION:
 *  Returns the total keys in map.
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
template <typename K, typename V>
std::size_t MMap<K, V>::size() const {
    return _mmap.size();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Checks if the map is empty.
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
template <typename K, typename V>
bool MMap<K, V>::empty() const {
    return _mmap.empty();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Points to left most element in map.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  MMap<K, V>::Iterator: points to left most element
 ******************************************************************************/
template <typename K, typename V>
typename MMap<K, V>::Iterator MMap<K, V>::begin() {
    return MMap<K, V>::Iterator(_mmap.begin());
}

/*******************************************************************************
 * DESCRIPTION:
 *  Points to end of map, which is nullptr.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  MMap<K, V>::Iterator: points to left most element
 ******************************************************************************/
template <typename K, typename V>
typename MMap<K, V>::Iterator MMap<K, V>::end() {
    return MMap<K, V>::Iterator(_mmap.end());
}

/*******************************************************************************
 * DESCRIPTION:
 *  Return iterator that points to MPair that matches key.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  MMap<K, V>::Iterator: points MPair that matches key
 ******************************************************************************/
template <typename K, typename V>
typename MMap<K, V>::Iterator MMap<K, V>::find(const K& key) {
    return MMap<K, V>::Iterator(_mmap.find(MPair(key)));
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns the reference value list at given key via subscript operator.
 *
 * PRE-CONDITIONS:
 *  const K& key: key to find MPair
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  const std::vector<V>&
 ******************************************************************************/
template <typename K, typename V>
const std::vector<V>& MMap<K, V>::operator[](const K& key) const {
    return _mmap.get(MPair(key)).values;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns the reference value list at given key via subscript operator.
 *
 * PRE-CONDITIONS:
 *  const K& key: key to find MPair
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  std::vector<V>&
 ******************************************************************************/
template <typename K, typename V>
std::vector<V>& MMap<K, V>::operator[](const K& key) {
    return _mmap.get(MPair(key)).values;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns the reference value list at given key.
 *
 * PRE-CONDITIONS:
 *  const K& key: key to find MPair
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  const std::vector<V>&
 ******************************************************************************/
template <typename K, typename V>
const std::vector<V>& MMap<K, V>::at(const K& key) const {
    return _mmap.get(MPair(key)).values;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns the reference value list at given key.
 *
 * PRE-CONDITIONS:
 *  const K& key: key to find MPair
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  std::vector<V>&
 ******************************************************************************/
template <typename K, typename V>
std::vector<V>& MMap<K, V>::at(const K& key) {
    return _mmap.get(MPair(key)).values;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Inserts Pair into map.
 *
 * PRE-CONDITIONS:
 *  const K& k: key for MPair
 *  const V& v: value for MPair
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename K, typename V>
bool MMap<K, V>::insert(const K& k, const V& v) {
    return _mmap.insert(MPair(k, v));
}

/*******************************************************************************
 * DESCRIPTION:
 *  Erase MPair from map.
 *
 * PRE-CONDITIONS:
 *  const K& k: key for MPair
 *  const V& v: value for MPair
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename K, typename V>
bool MMap<K, V>::erase(const K& key) {
    return _mmap.remove(MPair(key));
}

/*******************************************************************************
 * DESCRIPTION:
 *  Remove all elements in map.
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
template <typename K, typename V>
void MMap<K, V>::clear() {
    _mmap.clear();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns the reference value list at given key.
 *
 * PRE-CONDITIONS:
 *  const K& key: key to find Pair
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  V&
 ******************************************************************************/
template <typename K, typename V>
std::vector<V>& MMap<K, V>::get(const K& key) {
    return _mmap.get(Pair(key)).values;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Checks if key is contained in MMap.
 *
 * PRE-CONDITIONS:
 *  const MPair& target: the MPair to find
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename K, typename V>
bool MMap<K, V>::contains(const K& key) const {
    return _mmap.contains(MPair(key));
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns the number of elements for given key.
 *
 * PRE-CONDITIONS:
 *  const K& key: key to find MPair
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  std::size_t
 ******************************************************************************/
template <typename K, typename V>
std::size_t MMap<K, V>::count(const K& key) const {
    return _mmap.get(MPair(key)).values.size();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Print MMapBase with debug.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  void
 ******************************************************************************/
template <typename K, typename V>
void MMap<K, V>::print_debug() const {
    _mmap.print(std::cout, true);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Checks if multi-map base's structure is valid.
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
template <typename K, typename V>
bool MMap<K, V>::verify() const {
    return _mmap.verify();
}

}  // namespace bpt_map

#endif  // BPT_MAP_H
