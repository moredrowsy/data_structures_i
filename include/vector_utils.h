/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : vector_utils
 * DESCRIPTION : This header provides tempalated vector utility functions.
 ******************************************************************************/
#ifndef VECTOR_UTILS_H
#define VECTOR_UTILS_H

#include <iostream>  // stream objects
#include <vector>    // vector objects

// print std::vector list
template <typename T>
std::ostream& operator<<(std::ostream& outs, const std::vector<T>& list);

// list.push_back(item)
template <typename T>
std::vector<T>& operator+=(std::vector<T>& list, const T& item);

template <typename T, typename U>
std::vector<T>& operator+=(std::vector<T>& list, const U& item);

// add two vectors
template <typename T>
std::vector<T> operator+(const std::vector<T>& lhs, const std::vector<T>& rhs);

template <typename T>
std::vector<T>& operator+=(std::vector<T>& lhs, const std::vector<T>& rhs);

/*******************************************************************************
 * DESCRIPTION:
 *  Prints vector list
 *
 * PRE-CONDITIONS:
 *  std::ostream& outs        : ostream by ref
 *  const std::vector<T>& list: vector
 *
 * POST-CONDITIONS:
 *  prints
 *
 * RETURN:
 *  std::ostream&
 ******************************************************************************/
template <typename T>
std::ostream& operator<<(std::ostream& outs, const std::vector<T>& list) {
    for(const auto& a : list) {
        outs << a;
        if(&a != &list.back()) outs << ' ';
    }
    return outs;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Adds item to vector.
 *
 * PRE-CONDITIONS:
 *  std::vector<T>& list: vector
 *  const T& item       : item to add
 *
 * POST-CONDITIONS:
 *  item added and vector increases in size
 *
 * RETURN:
 *  std::vector<T>&
 ******************************************************************************/
template <typename T>
std::vector<T>& operator+=(std::vector<T>& list, const T& item) {
    list.push_back(item);
    return list;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Adds type U item to type T vector, which will rely on implicit conversion
 *  of U to T.
 *
 * PRE-CONDITIONS:
 *  std::vector<T>& list: vector
 *  const T& item       : item to add
 *
 * POST-CONDITIONS:
 *  item added and vector increases in size
 *
 * RETURN:
 *  std::vector<T>&
 ******************************************************************************/
template <typename T, typename U>
std::vector<T>& operator+=(std::vector<T>& list, const U& item) {
    list.push_back(item);
    return list;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Adds two vectors together.
 *
 * PRE-CONDITIONS:
 *  const std::vector<T>& lhs: vector
 *  const std::vector<T>& rhs: vector
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  std::vector<T>&
 ******************************************************************************/
template <typename T>
std::vector<T> operator+(const std::vector<T>& lhs, const std::vector<T>& rhs) {
    std::vector<T> v = lhs;
    for(const auto& a : rhs) v.push_back(a);
    return v;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Adds two vectors together, effectively appending rhs to lhs.
 *
 * PRE-CONDITIONS:
 *  std::vector<T>&       lhs: vector
 *  const std::vector<T>& rhs: vector
 *
 * POST-CONDITIONS:
 *  lsh merged with rhs
 *
 * RETURN:
 *  std::vector<T>&
 ******************************************************************************/
template <typename T>
std::vector<T>& operator+=(std::vector<T>& lhs, const std::vector<T>& rhs) {
    for(const auto& a : rhs) lhs.push_back(a);
    return lhs;
}

#endif  // VECTOR_UTILS_H
