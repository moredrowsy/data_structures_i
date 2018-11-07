/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : array_utils
 * DESCRIPTION : This header provides tempalated vector utility functions.
 ******************************************************************************/
#ifndef VECTOR_UTILS_H
#define VECTOR_UTILS_H

#include <iostream>  // stream objects
#include <vector>    // vector objects

//-------------- std::Vector Extra operators: ---------------------
// print std::vector list
template <typename T>
std::ostream& operator<<(std::ostream& outs, const std::vector<T>& list);

// list.push_back add_me
template <typename T>
std::vector<T>& operator+=(std::vector<T>& list, const T& addme);

// append second rhs to lhs
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
    for(std::size_t i = 0; i < list.size(); ++i) {
        outs << list.at(i);
        if(i != list.size() - 1) outs << ' ';
    }
    return outs;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Adds item to vector.
 *
 * PRE-CONDITIONS:
 *  std::vector<T>& list: vector
 *  const T& addme            : item to add
 *
 * POST-CONDITIONS:
 *  item added and vector increases in size
 *
 * RETURN:
 *  std::vector<T>&
 ******************************************************************************/
template <typename T>
std::vector<T>& operator+=(std::vector<T>& list, const T& addme) {
    list.push_back(addme);
    return list;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Adds two vector together, effectively appending rhs to lhs.
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
