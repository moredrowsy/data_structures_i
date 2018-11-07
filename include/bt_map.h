#ifndef BT_MAP_H
#define BT_MAP_H

#include "vector_utils.h"  // vector utilities, include first

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
    Map() {}

    // capacity
    std::size_t size() const;
    bool empty() const;

    // element access
    V& operator[](const K& key);
    V& at(const K& key);
    const V& at(const K& key) const;

    // modifiers
    bool insert(const K& k, const V& v);
    bool erase(const K& key);
    void clear();
    V& get(const K& key);

    // operations
    V* find(const K& key);
    bool contains(const Pair& target) const;

    // I have not written these yet, but we will need them:
    //    int count(const K& key);
    //    lower_bound
    //    upper_bound
    //    equal_range:

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
    // why don't we have a at() for MMap?

    // modifiers
    bool insert(const K& k, const V& v);
    bool erase(const K& key);
    void clear();

    // operations
    bool contains(const K& key) const;
    std::vector<V>& get(const K& key);

    // Iterator find(const K& key);
    int count(const K& key);
    // I have not writtent hese yet, but we will need them:
    //    int count(const K& key);
    //    lower_bound
    //    upper_bound
    //    equal_range:

    bool verify() const;

    friend std::ostream& operator<<(std::ostream& outs, const MMap<K, V>& map) {
        return outs << map._mmap;
    }

private:
    MMapBase _mmap;
};

// MAP IMPLEMENTATIONS

template <typename K, typename V>
std::size_t Map<K, V>::size() const {
    return _map.size();
}

template <typename K, typename V>
bool Map<K, V>::empty() const {
    return _map.empty();
}

template <typename K, typename V>
V& Map<K, V>::operator[](const K& key) {
    return _map.get(Pair(key)).value;
}

template <typename K, typename V>
V& Map<K, V>::at(const K& key) {
    return _map.get(Pair(key)).value;
}

template <typename K, typename V>
const V& Map<K, V>::at(const K& key) const {
    return _map.get(Pair(key)).value;
}

template <typename K, typename V>
bool Map<K, V>::insert(const K& k, const V& v) {
    return _map.insert(Pair(k, v));
}

template <typename K, typename V>
bool Map<K, V>::erase(const K& key) {
    return _map.remove(Pair(key));
}

template <typename K, typename V>
void Map<K, V>::clear() {
    _map.clear();
}

template <typename K, typename V>
V& Map<K, V>::get(const K& key) {
    return _map.get(Pair(key)).value;
}

template <typename K, typename V>
V* Map<K, V>::find(const K& key) {
    return _map.find(Pair(key));
}

template <typename K, typename V>
bool Map<K, V>::contains(const Pair& target) const {
    return _map.contains(target);
}

template <typename K, typename V>
bool Map<K, V>::verify() const {
    return _map.verify();
}

// MMAP IMPLEMENTATIONS

template <typename K, typename V>
std::size_t MMap<K, V>::size() const {
    return _mmap.size();
}

template <typename K, typename V>
bool MMap<K, V>::empty() const {
    return _mmap.empty();
}

template <typename K, typename V>
const std::vector<V>& MMap<K, V>::operator[](const K& key) const {
    return _mmap.get(MPair(key)).value_list;
}

template <typename K, typename V>
std::vector<V>& MMap<K, V>::operator[](const K& key) {
    return _mmap.get(MPair(key)).value_list;
}

template <typename K, typename V>
bool MMap<K, V>::insert(const K& k, const V& v) {
    return _mmap.insert(MPair(k, v));
}

template <typename K, typename V>
bool MMap<K, V>::erase(const K& key) {
    return _mmap.remove(MPair(key));
}

template <typename K, typename V>
void MMap<K, V>::clear() {
    _mmap.clear();
}

template <typename K, typename V>
bool MMap<K, V>::contains(const K& key) const {
    return _mmap.contains(MPair(key));
}

template <typename K, typename V>
std::vector<V>& MMap<K, V>::get(const K& key) {
    return _mmap.get(Pair(key)).value_list;
}

template <typename K, typename V>
bool MMap<K, V>::verify() const {
    return _mmap.verify();
}

}  // namespace bt_map

#endif  // BT_MAP_H
