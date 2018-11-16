/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : bt_map
 * DESCRIPTION : This header provides a templated BTree (B-Tree) version
 *          of the Map/MMap class, along with Pair/MPair as the key/value(s)
 *          structure.
 *          Map uses BTree as base with Pair. It does not allow duplicate keys
 *          but allow value modification.
 *          MMap uses BTree as base with MPair. It does not allow duplicate
 *          keys but allow duplicate values. The values are stored in
 *          key/vector structure.
 ******************************************************************************/
#ifndef BT_MAP_H
#define BT_MAP_H

#include <vector>   // vector objects
#include "btree.h"  // BTree class
#include "pair.h"   // Pair struct

namespace bt_map {

template <typename K, typename V>
class Map {
public:
    typedef pair::Pair<K, V> Pair;
    typedef btree::BTree<Pair> MapBase;

    // CONSTRUCTOR
    Map() : _map(true) {}

    // capacity
    std::size_t size() const;
    bool empty() const;

    // element access
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
    // Iterator find(const K& key); // iterator not implemented
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
    typedef pair::MPair<K, V> MPair;
    typedef btree::BTree<MPair> MMapBase;

    MMap() : _mmap(true) {}

    // capacity
    std::size_t size() const;
    bool empty() const;

    // element access
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
    // Iterator find(const K& key); // iterator not implemented
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

}  // namespace bt_map

#endif  // BT_MAP_H
