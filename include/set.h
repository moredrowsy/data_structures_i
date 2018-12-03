/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : bpt_set
 * DESCRIPTION : This header provides a templated Set based on the B+Tree data
 *          structure. The templated item can not be modified but can be
 *          removed.
 ******************************************************************************/
#ifndef SET_H
#define SET_H

#include <initializer_list>  // initializer list
#include "bptree.h"          // BPTree class

namespace set {

template <typename T>
class Set {
public:
    typedef bptree::BPTree<T> SetBase;
    typedef typename bptree::BPTree<T>::Iterator SetBaseIter;

    class Iterator {
    public:
        friend class Set;

        // CONSTRUCTOR
        Iterator(SetBaseIter it = SetBaseIter(nullptr)) : _it(it) {}

        bool is_null() { return !_it; }
        explicit operator bool() { return (bool)_it; }

        const T& operator*() { return *_it; }    // member access
        T const* operator->() { return &*_it; }  // member access

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

        // FRIENDS
        friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
            return lhs._it == rhs._it;
        }

        friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
            return lhs._it != rhs._it;
        }

    private:
        SetBaseIter _it;
    };

    // CONSTRUCTOR
    Set(std::size_t min = bptree::MINIMUM) : _set(false, min) {}
    Set(const std::initializer_list<T>& l, std::size_t min = bptree::MINIMUM);

    // capacity
    std::size_t size() const;
    bool empty() const;

    // element access
    Iterator begin() const;
    Iterator end() const;
    Iterator find(const T& item) const;
    const T& front();
    const T& back();
    const T& operator[](const T& item);
    const T& at(const T& item);

    // modifiers

    bool insert(const T& item);
    bool erase(const T& item);
    void intersect(const Set<T>& rhs, Set<T>& result);
    void clear();
    const T& get(const T& item);

    // operations
    bool contains(const T& item) const;
    std::size_t count(const T& item) const;
    void print_debug() const;
    bool verify() const;

    friend std::ostream& operator<<(std::ostream& outs, const Set<T>& set) {
        return outs << set._set;
    }

    friend Set<T>& operator+=(Set<T>& lhs, const Set<T>& rhs) {
        for(const auto& a : rhs) lhs.insert(a);
        return lhs;
    }

    friend Set<T> operator+(Set<T>& lhs, const Set<T>& rhs) {
        Set<T> temp = lhs;
        for(const auto& a : rhs) temp.insert(a);
        return temp;
    }

    friend Set<T>& operator-=(Set<T>& lhs, const Set<T>& rhs) {
        for(const auto& a : rhs) lhs.erase(a);
        return lhs;
    }

    friend Set<T> operator-(Set<T>& lhs, const Set<T>& rhs) {
        Set<T> temp = lhs;
        for(const auto& a : rhs) temp.erase(a);
        return temp;
    }

private:
    SetBase _set;
};

// ----- SET IMPLEMENTATIONS -----

/*******************************************************************************
 * DESCRIPTION:
 *  Constructs Set with on item.
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
Set<T>::Set(const std::initializer_list<T>& l, std::size_t min)
    : _set(false, min) {
    for(const auto& a : l) _set.insert(a);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns the total items in set.
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
std::size_t Set<T>::size() const {
    return _set.size();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Checks if the set is empty.
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
bool Set<T>::empty() const {
    return _set.empty();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Points to left most element in set.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  Set<T>::Iterator: points to left most element
 ******************************************************************************/
template <typename T>
typename Set<T>::Iterator Set<T>::begin() const {
    return Set<T>::Iterator(_set.begin());
}

/*******************************************************************************
 * DESCRIPTION:
 *  Points to end of set, which is nullptr.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  Set<T>::Iterator: points to left most element
 ******************************************************************************/
template <typename T>
typename Set<T>::Iterator Set<T>::end() const {
    return Set<T>::Iterator(_set.end());
}

/*******************************************************************************
 * DESCRIPTION:
 *  Return iterator that points to T that matches item.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  Set<T>::Iterator: points T that matches item
 ******************************************************************************/
template <typename T>
typename Set<T>::Iterator Set<T>::find(const T& item) const {
    return Set<T>::Iterator(_set.find(item));
}

/*******************************************************************************
 * DESCRIPTION:
 *  Points to end of set, which is nullptr.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  Set<T>::Iterator: points to left most element
 ******************************************************************************/
template <typename T>
const T& Set<T>::front() {
    return _set.front();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Points to end of set, which is nullptr.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  Set<T>::Iterator: points to left most element
 ******************************************************************************/
template <typename T>
const T& Set<T>::back() {
    return _set.back();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns the reference item at given item via subscript operator.
 *
 * PRE-CONDITIONS:
 *  const T& item: item to find T
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  const T&
 ******************************************************************************/
template <typename T>
const T& Set<T>::operator[](const T& item) {
    return _set.get(item);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns the reference item at given item.
 *
 * PRE-CONDITIONS:
 *  const T& item: item to find T
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  const T&
 ******************************************************************************/
template <typename T>
const T& Set<T>::at(const T& item) {
    return _set.get(item);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Inserts T into set.
 *
 * PRE-CONDITIONS:
 *  const T& item: templated item
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool Set<T>::insert(const T& item) {
    return _set.insert(item);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Erase T from set.
 *
 * PRE-CONDITIONS:
 *  const T& item: templated item
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool Set<T>::erase(const T& item) {
    return _set.remove(item);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Erase T from set.
 *
 * PRE-CONDITIONS:
 *  const Set<T>& rhs: right hand side Set
 *  Set<T>& result   : result Set
 *
 * POST-CONDITIONS:
 *  Set<T>& result: populated with data if successful
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void Set<T>::intersect(const Set<T>& rhs, Set<T>& result) {
    for(const auto& a : rhs)
        if(contains(a)) result.insert(a);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Remove all elements in set.
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
void Set<T>::clear() {
    _set.clear();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns the reference item at given item.
 *
 * PRE-CONDITIONS:
 *  const T& item: item to find T
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  T&
 ******************************************************************************/
template <typename T>
const T& Set<T>::get(const T& item) {
    return _set.get(item);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Checks if item is contained in Set.
 *
 * PRE-CONDITIONS:
 *  const T& item: item to find T
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool Set<T>::contains(const T& item) const {
    return _set.contains(item);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns the number of elements for given item.
 *
 * PRE-CONDITIONS:
 *  const T& item: item to find T
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  std::size_t
 ******************************************************************************/
template <typename T>
std::size_t Set<T>::count(const T& item) const {
    return _set.find(MPair(item)) ? 1 : 0;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Print SetBase with debug.
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
template <typename T>
void Set<T>::print_debug() const {
    _set.print(std::cout, true);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Checks if set base's structure is valid.
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
bool Set<T>::verify() const {
    return _set.verify();
}

}  // namespace set

#endif  // SET_H
