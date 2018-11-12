/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : pair
 * DESCRIPTION : This header provides a templated Pair and MPair (multi-pair).
 *          In Pair, the left hand side's value is set to right hand side's if
 *          adding the same key.
 *          In the MPair, the value list is a vector and the vectors are merged
 *          when adding the same key.
 *          Used in conjunction with Map/MMap respectively.
 ******************************************************************************/
#ifndef PAIR_H
#define PAIR_H

#include <cassert>         // assert()
#include "vector_utils.h"  // vector utilities

namespace pair {

template <typename K, typename V>
struct Pair {
    K key;
    V value;

    // CONSTRUCTOR
    Pair(const K& k = K(), const V& v = V()) : key(k), value(v) {}

    // FRIENDS
    friend std::ostream& operator<<(std::ostream& outs, const Pair<K, V>& p) {
        return outs << p.key << " : " << p.value;
    }

    friend bool operator==(const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
        return lhs.key == rhs.key;
    }

    friend bool operator<(const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
        return lhs.key < rhs.key;
    }

    friend bool operator<=(const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
        return lhs.key <= rhs.key;
    }

    friend bool operator>(const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
        return lhs.key > rhs.key;
    }

    friend bool operator>=(const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
        return lhs.key >= rhs.key;
    }

    friend Pair<K, V> operator+(const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
        assert(lhs.key == rhs.key);
        return Pair(lhs.key, rhs.value);
    }

    friend Pair<K, V>& operator+=(Pair<K, V>& lhs, const Pair<K, V>& rhs) {
        assert(lhs.key == rhs.key);
        lhs.value = rhs.value;  // on same key, lhs's value is set to rhs's
        return lhs;
    }
};

template <typename K, typename V>
struct MPair {
    K key;
    std::vector<V> value_list;

    // CONSTRUCTORS
    MPair(const K& k = K()) : key(k) {}
    MPair(const K& k, const V& v) : key(k) { value_list.push_back(v); }
    MPair(const K& k, const std::vector<V>& vlist)
        : key(k), value_list(vlist) {}

    // FRIENDS
    friend std::ostream& operator<<(std::ostream& outs, const MPair<K, V>& mp) {
        return outs << mp.key << " : " << mp.value_list;
    }

    friend bool operator==(const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        return lhs.key == rhs.key;
    }

    friend bool operator<(const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        return lhs.key < rhs.key;
    }

    friend bool operator<=(const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        return lhs.key <= rhs.key;
    }

    friend bool operator>(const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        return lhs.key > rhs.key;
    }

    friend bool operator>=(const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        return lhs.key >= rhs.key;
    }

    friend MPair<K, V> operator+(const MPair<K, V>& lhs,
                                 const MPair<K, V>& rhs) {
        assert(lhs.key == rhs.key);
        return MPair(lhs.key, lhs.value_list + rhs.value_list);
    }

    friend MPair<K, V>& operator+=(MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        assert(lhs.key == rhs.key);
        lhs.value_list += rhs.value_list;  // on same key, lists are merged
        return lhs;
    }
};

}  // namespace pair

#endif  // PAIR_H
