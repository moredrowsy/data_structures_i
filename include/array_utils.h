#ifndef ARRAY_UTILS_H
#define ARRAY_UTILS_H

#include <iomanip>   // setw()
#include <iostream>  // stream objects
#include <string>    // string objects
#include <vector>    // vector objects

namespace array_utils {

// return the larger of the two items
template <typename T>
inline T max(const T& a, const T& b);

template <typename T>
inline void swap(T& a, T& b);

// return index of the largest item in data
template <typename T>
std::size_t index_of_max(T* data, std::size_t size);

// insert entry into the sorted array data with length n
template <typename T>
void ordered_insert(T* data, std::size_t& size, const T& entry);

// return the first element in data that is not less than entry
template <typename T>
std::size_t first_ge(const T* data, std::size_t size, const T& entry);

// append entry to the right of data
template <typename T>
void attach_item(T* data, std::size_t& size, const T& entry);

// insert entry at index i in data
// pro: ensure data actual size is size+1 to shift right
template <typename T>
void insert_item(T* data, std::size_t i, std::size_t& size, const T& entry);

// remove the last element in data and place it in entry
template <typename T>
void detach_item(T* data, std::size_t& size, T& entry);

// delete item at index i and place it in entry
template <typename T>
void delete_item(T* data, std::size_t i, std::size_t& size, T& entry);

// append data2 to the right of data1
template <typename T>
void merge(T* data1, std::size_t& size1, T* data2, std::size_t& size2);

// move n/2 elements from the right of data1 and move to data2
// pre: size1 is entire array, data2 and size2 is empty
// post: elements left of mid in data1, right of mid in data2, return ref mid
template <typename T>
void split(T* src, std::size_t& src_size, T* dest, std::size_t& dest_size);

// copy *src into *dest
template <typename T>
void copy_array(T* dest, const T* src, std::size_t& dest_size,
                std::size_t src_size);

// print array with given index position
template <typename T>
void print_array(const T* data, std::size_t size, int pos = -1);

template <typename T>
bool is_gt(const T* data, std::size_t i, const T& item);  // item > all data[i]

template <typename T>
bool is_le(const T* data, std::size_t i, const T& item);  // item <= all data[i]

//-------------- std::Vector Extra operators: ---------------------

// print std::vector list
template <typename T>
std::ostream& operator<<(std::ostream& outs, const std::vector<T>& list);

// list.push_back add_me
template <typename T>
std::vector<T>& operator+=(std::vector<T>& list, const T& addme);

template <typename T>
T max(const T& a, const T& b) {
    return a < b ? b : a;
}

template <typename T>
void swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

template <typename T>
std::size_t index_of_max(T* data, std::size_t size) {
    std::size_t min = 0;

    for(std::size_t i = 1; i < size; ++i)
        if(data[i] > data[min]) min = i;

    return min;
}

// REQUIREMENT: index @ size must exist to shift right
template <typename T>
void ordered_insert(T* data, std::size_t& size, const T& entry) {
    std::size_t forward = 0;          // forward walker to insert @ index
    std::size_t backward = size + 1;  // backward walker to shift  right

    if(entry > data[size - 1])  // if greater than last, insert after last
        data[size] = entry;
    else {
        while(forward < size) {
            if(entry == data[forward]) {  // replace if same
                data[forward] = entry;
                break;
            }

            if(entry < data[forward]) {
                while(backward != forward) {
                    data[backward] = data[backward - 1];  // shift right
                    --backward;                           // walk backwards
                }
                data[forward] = entry;
                break;
            }

            ++forward;  // walk forward
        }
    }

    ++size;  // return new size to caller
}

template <typename T>
std::size_t first_ge(const T* data, std::size_t size, const T& entry) {
    std::size_t walker = 0;  // forward walker
    while(walker < size && data[walker] < entry) ++walker;

    return walker;
}

template <typename T>
void attach_item(T* data, std::size_t& size, const T& entry) {
    data[size++] = entry;
}

template <typename T>
void insert_item(T* data, std::size_t i, std::size_t& size, const T& entry) {
    std::size_t walker = size;

    while(walker != i) {
        data[walker] = data[walker - 1];  // shift right
        --walker;                         // walker backwards
    }
    data[walker] = entry;

    ++size;  // return new size to caller
}

template <typename T>
void detach_item(T* data, std::size_t& size, T& entry) {
    if(size) entry = data[--size];
}

template <typename T>
void delete_item(T* data, std::size_t i, std::size_t& size, T& entry) {
    if(i < size) {
        entry = data[i];                          // return T to caller
        while(++i < size) data[i - 1] = data[i];  // shift left
        --size;
    }
}

template <typename T>
void merge(T* data1, std::size_t& size1, T* data2, std::size_t& size2) {
    for(std::size_t i = 0; i < size2; ++i) data1[size1++] = data2[i];
    size2 = 0;
}

template <typename T>
void split(T* src, std::size_t& src_size, T* dest, std::size_t& dest_size) {
    std::size_t mid = (src_size + 1) / 2, walker = mid;

    while(walker < src_size) dest[dest_size++] = src[walker++];
    src_size = mid;
}

template <typename T>
void copy_array(T* dest, const T* src, std::size_t& dest_size,
                std::size_t src_size) {
    for(dest_size = 0; dest_size < src_size; ++dest_size)
        dest[dest_size] = src[dest_size];
}

template <typename T>
void print_array(const T* data, std::size_t size, int pos) {
    if(pos > -1) {
        int count = -1;

        for(int i = 0; i <= pos; ++i) {
            ++count += (std::to_string(data[i]).size());
            std::cout << data[i] << ' ';
        }
        std::cout << '\n'
                  << std::setw(count - std::to_string(data[pos]).size() / 2)
                  << '^';
    } else
        for(std::size_t i = 0; i < size; ++i) std::cout << data[i] << " ";

    std::cout << std::endl;
}

template <typename T>
bool is_gt(const T* data, std::size_t i, const T& item) {
    return item > data[i];
}

template <typename T>
bool is_le(const T* data, std::size_t i, const T& item) {
    return item <= data[i];
}

template <typename T>
std::ostream& operator<<(std::ostream& outs, const std::vector<T>& list) {
    for(const auto& v : list) outs << v << ' ';
    return outs;
}

template <typename T>
std::vector<T>& operator+=(std::vector<T>& list, const T& addme) {
    list.push_back(addme);
    return list;
}

}  // namespace array_utils

#endif  // ARRAY_UTILS_H
