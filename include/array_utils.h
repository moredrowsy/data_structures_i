/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : array_utils
 * DESCRIPTION : This header provides tempalated array utility functions to
 *      manipulate arrays.
 ******************************************************************************/
#ifndef ARRAY_UTILS_H
#define ARRAY_UTILS_H

#include <iomanip>   // setw()
#include <iostream>  // stream objects
#include <string>    // string objects
#include <vector>    // vector objects

namespace array_utils {

// return the larger of the two items
template <typename T>
inline T& max(const T& a, const T& b);

template <typename T>
inline void swap(T& a, T& b);

// return index of the largest item in data
template <typename T>
std::size_t index_of_max(T* data, std::size_t size);

// insert entry into the sorted array data with length n
template <typename T>
void ordered_insert(T* data, std::size_t& size, const T& entry,
                    bool replace = false);

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

// delete item @ index i
template <typename T>
void delete_item(T* data, std::size_t i, std::size_t& size);

// delete item @ index i and place it in entry
template <typename T>
void delete_item(T* data, std::size_t i, std::size_t& size, T& entry);

// append src to right to dest
template <typename T>
void merge(T* src, std::size_t& src_size, T* dest, std::size_t& dest_size);

// transfer n/2 elements from the right of src to dest
// pre: src_size is entire array
template <typename T>
void split(T* src, std::size_t& src_size, T* dest, std::size_t& dest_size);

// copy *src into *dest
template <typename T>
void copy_array(const T* src, std::size_t src_size, T* dest,
                std::size_t& dest_size);

// transfer *src to *dest
template <typename T>
void transfer_array(T* src, std::size_t& src_size, T* dest,
                    std::size_t& dest_size);

// print array with given index position
template <typename T>
void print_array(const T* data, std::size_t size, int pos = -1);

template <typename T>
bool is_gt(const T* data, std::size_t size, const T& item);  // item > data

template <typename T>
bool is_lt(const T* data, std::size_t size, const T& item);  // item < data

template <typename T>
bool is_le(const T* data, std::size_t size, const T& item);  // item <= data

/*******************************************************************************
 * DESCRIPTION:
 *  Returns the larger item.
 *
 * PRE-CONDITIONS:
 *  const T& a: templated item by reference
 *  const T& b: templated item by reference
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  T
 ******************************************************************************/
template <typename T>
T& max(const T& a, const T& b) {
    return a < b ? b : a;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Swap two items.
 *
 * PRE-CONDITIONS:
 *  const T& a: templated item by reference
 *  const T& b: templated item by reference
 *
 * POST-CONDITIONS:
 *  a and b are swapped.
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Finds the index of the largest value in data.
 *
 * PRE-CONDITIONS:
 *  T* data         : templated array
 *  std::size_t size: array size
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  std::size_t: index
 ******************************************************************************/
template <typename T>
std::size_t index_of_max(T* data, std::size_t size) {
    std::size_t min = 0;

    for(std::size_t i = 1; i < size; ++i)
        if(data[i] > data[min]) min = i;

    return min;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Insert templated entry into array by sorted order. No duplicates but allow
 *  replacing same item if flag is set.
 *
 * PRE-CONDITIONS:
 *  T* data          : templated array
 *  std::size_t& size: size array + 1 MUST be valid for shifting values to right
 *  const T& entry   : entry item to be inserted
 *  bool replace     : allows replacing the same item.
 *
 * POST-CONDITIONS:
 *  std::size_t size: increases by 1
 *  T entry         : inserted into array
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void ordered_insert(T* data, std::size_t& size, const T& entry, bool replace) {
    std::size_t forward = 0;          // forward walker to insert @ index
    std::size_t backward = size + 1;  // backward walker to shift  right

    if(!size)
        data[size] = entry;
    else if(entry > data[size - 1])  // if greater than last, insert after last
        data[size] = entry;
    else {
        while(forward < size) {
            if(entry == data[forward]) {  // replace if same
                if(replace) data[forward] = entry;
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

/*******************************************************************************
 * DESCRIPTION:
 *  Find the index of the first value that's greater than or equal to entry.
 *
 * PRE-CONDITIONS:
 *  const T* data   : templated array
 *  std::size_t size: array size
 *  const T& entry  : entry item to compare
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  std::size_t: index
 ******************************************************************************/
template <typename T>
std::size_t first_ge(const T* data, std::size_t size, const T& entry) {
    std::size_t walker = 0;  // forward walker
    while(walker < size && data[walker] < entry) ++walker;

    return walker;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Inserts entry at the end of the array.
 *
 * PRE-CONDITIONS:
 *  T* data           : templated array
 *  std::size_t & size: size + 1 MUST be valid for shifting values to right
 *  const T& entry    : entry item to be inserted
 *
 * POST-CONDITIONS:
 *  std::size_t size: increases by 1
 *  T entry         : inserted into array
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void attach_item(T* data, std::size_t& size, const T& entry) {
    data[size++] = entry;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Inserts entry at position i into array.
 *
 * PRE-CONDITIONS:
 *  T* data          : templated array
 *  std::size_t i    : position to insert
 *  std::size_t& size: size + 1 MUST be valid for shifting values to right
 *  const T& entry   : entry item to be inserted
 *
 * POST-CONDITIONS:
 *  std::size_t size: increases by 1
 *  T entry         : inserted into array
 *
 * RETURN:
 *  none
 ******************************************************************************/
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

/*******************************************************************************
 * DESCRIPTION:
 *  Removes the last item in the array.
 *
 * PRE-CONDITIONS:
 *  T* data          : templated array
 *  std::size_t& size: size array
 *  T& entry         : entry item to hold detached item
 *
 * POST-CONDITIONS:
 *  std::size_t size: decreases by 1
 *  T& entry        : entry item set to detached item
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void detach_item(T* data, std::size_t& size, T& entry) {
    if(size) entry = data[--size];
}

/*******************************************************************************
 * DESCRIPTION:
 *  Deletes an item at position i.
 *
 * PRE-CONDITIONS:
 *  T* data          : templated array
 *  std::size_t i    : position to delete
 *  std::size_t& size: size array
 *
 * POST-CONDITIONS:
 *  std::size_t size: decreases by 1
 *  T               : removed at position i
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void delete_item(T* data, std::size_t i, std::size_t& size) {
    if(i < size) {
        while(++i < size) data[i - 1] = data[i];  // shift left
        --size;
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Deletes an item at position i.
 *
 * PRE-CONDITIONS:
 *  T* data          : templated array
 *  std::size_t i    : position to delete
 *  std::size_t& size: size array
 *  T& entry         : entry item to hold deleted item
 *
 * POST-CONDITIONS:
 *  std::size_t size: decreases by 1
 *  T               : removed at position i
 *  T& entry        : entry item set to deleted item
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void delete_item(T* data, std::size_t i, std::size_t& size, T& entry) {
    if(i < size) {
        entry = data[i];                          // return T to caller
        while(++i < size) data[i - 1] = data[i];  // shift left
        --size;
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Merges two array, effectively appending source to destination and resetting
 *  source's size to 0.
 *
 * PRE-CONDITIONS:
 *  T* src                : source array
 *  std::size_t& src_size : source size
 *  T* dest               : destination array
 *  std::size_t& dest_size: destination size
 *
 * POST-CONDITIONS:
 *  std::size_t& src_size : set to 0
 *  T* dest               : items added
 *  std::size_t& dest_size: size increases by source size
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void merge(T* src, std::size_t& src_size, T* dest, std::size_t& dest_size) {
    for(std::size_t i = 0; i < src_size; ++i) dest[dest_size++] = src[i];
    src_size = 0;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Splits source array by (n+1)/2 to destination array. At odd size, splits
 *  after middle. At even size, splits at middle.
 *
 * PRE-CONDITIONS:
 *  T* src                : source array
 *  std::size_t& src_size : source size
 *  T* dest               : destination array
 *  std::size_t& dest_size: destination size
 *
 * POST-CONDITIONS:
 *  T* src                : items removed
 *  std::size_t& src_size : size decreases
 *  T* dest               : items added
 *  std::size_t& dest_size: size increases
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void split(T* src, std::size_t& src_size, T* dest, std::size_t& dest_size) {
    std::size_t mid = (src_size + 1) / 2, walker = mid;

    while(walker < src_size) dest[dest_size++] = src[walker++];
    src_size = mid;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Shallow copy data from source to destination.
 *
 * PRE-CONDITIONS:
 *  const T* src          : source array
 *  std::size_t src_size  : source size
 *  T* dest               : destination array
 *  std::size_t& dest_size: destination size
 *
 * POST-CONDITIONS:
 *  T* dest               : items added
 *  std::size_t& dest_size: set to src_size
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void copy_array(const T* src, std::size_t src_size, T* dest,
                std::size_t& dest_size) {
    for(dest_size = 0; dest_size < src_size; ++dest_size)
        dest[dest_size] = src[dest_size];
}

/*******************************************************************************
 * DESCRIPTION:
 *  Shallow transfer data from source to destination.
 *
 * PRE-CONDITIONS:
 *  T* src                : source array
 *  std::size_t& src_size : source size
 *  T* dest               : destination array
 *  std::size_t& dest_size: destination size
 *
 * POST-CONDITIONS:
 *  std::size_t& src_size : set to 0
 *  T* dest               : items added
 *  std::size_t& dest_size: set to original src_size
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void transfer_array(T* src, std::size_t& src_size, T* dest,
                    std::size_t& dest_size) {
    for(dest_size = 0; dest_size < src_size; ++dest_size)
        dest[dest_size] = src[dest_size];
    src_size = 0;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Prints array with carat pointed to i.
 *
 * PRE-CONDITIONS:
 *  const T* data   : templated array
 *  std::size_t size: array size
 *  int pos         : position of carat
 *
 * POST-CONDITIONS:
 *  prints
 *
 * RETURN:
 *  none
 ******************************************************************************/
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

/*******************************************************************************
 * DESCRIPTION:
 *  Verify templated item is greater than all values in data.
 *
 * PRE-CONDITIONS:
 *  const T* data   : templated array
 *  std::size_t size: array size
 *  const T& item   : item to compare
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool is_gt(const T* data, std::size_t size, const T& item) {
    for(std::size_t j = 0; j < size; ++j) {
        if(item <= data[j]) return false;
    }

    return true;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Verify templated item is less than all values in data.
 *
 * PRE-CONDITIONS:
 *  const T* data   : templated array
 *  std::size_t size: array size
 *  const T& item   : item to compare
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool is_lt(const T* data, std::size_t size, const T& item) {
    for(std::size_t j = 0; j < size; ++j)
        if(item >= data[j]) return false;

    return true;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Verify templated item is less than or equal to all values in data.
 *
 * PRE-CONDITIONS:
 *  const T* data   : templated array
 *  std::size_t size: array size
 *  const T& item   : item to compare
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool is_le(const T* data, std::size_t size, const T& item) {
    for(std::size_t j = 0; j < size; ++j)
        if(item > data[j]) return false;

    return true;
}

}  // namespace array_utils

#endif  // ARRAY_UTILS_H
