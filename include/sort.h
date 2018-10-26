#ifndef SORT_H
#define SORT_H

#include <cstdlib>            // rand(), srand()
#include <string>             // string objects
#include "../include/heap.h"  // Heap class

namespace sort {

template <typename T>  // less than comparison
static inline bool less(const T &left, const T &right);

template <typename T>  // greater than comparison
static inline bool greater(const T &left, const T &right);

template <typename T>
inline void swap(T &left, T &right);

template <typename T>
void shuffle(T *data, std::size_t size);

template <typename T>
void copy_array(T *dest, const T *src, std::size_t size);

// verify data's sortedness
template <typename T, typename C = bool (*)(T const &, T const &)>
bool verify(const T *data, std::size_t size, C cmp = &less<T>);

// verify sortedness return "SORTED" or "NOT SORTED"
template <typename T, typename C = bool (*)(T const &, T const &)>
std::string verifystr(const T *data, std::size_t size, C cmp = &less<T>);

// print the array if status_only is true with info on sortedness of data[]
template <typename T, typename C = bool (*)(T const &, T const &)>
void print_array(const T *data, std::size_t size, bool status_only = false,
                 C cmp = &less<T>);

template <typename T>
void print_array_segment(T *data, std::size_t start, std::size_t end);

template <typename T>
void bubble_sort(T *data, std::size_t size);

template <typename T, typename C>
void bubble_sort(T *data, std::size_t size, C cmp);

template <typename T>
void selection_sort(T *data, std::size_t size);

template <typename T, typename C>
void selection_sort(T *data, std::size_t size, C cmp);

template <typename T>
void insertion_sort(T *data, std::size_t size);

template <typename T, typename C>
void insertion_sort(T *data, std::size_t size, C cmp);

template <typename T>
void merge_sort(T *data, std::size_t size);

template <typename T, typename C>
void merge_sort(T *data, std::size_t size, C cmp);

template <typename T>
void quick_sort(T *data, std::size_t size);

template <typename T, typename C>
void quick_sort(T *data, std::size_t size, C cmp);

template <typename T>
void quick_sort2(T *data, std::size_t size);

template <typename T, typename C>
void quick_sort2(T *data, std::size_t size, C cmp);

template <typename T>
void heap_sort(T *data, std::size_t size);

template <typename T, typename C>
void heap_sort(T *data, std::size_t size, C cmp);

// INTERNAL DECLARATIONS
namespace internal {

template <typename T>
void merge(T *data, std::size_t size1, std::size_t size2);

template <typename T, typename C>
void merge(T *data, std::size_t size1, std::size_t size2, C cmp);

template <typename T>
std::size_t partition(T *data, std::size_t size);

template <typename T, typename C>
std::size_t partition(T *data, std::size_t size, C cmp);

template <typename T>
void partition2(T *data, std::size_t size, std::size_t &pivot);

template <typename T, typename C>
void partition2(T *data, std::size_t size, std::size_t &pivot, C cmp);

template <typename T>
void make_heap(T *data, std::size_t size);

template <typename T, typename C>
void make_heap(T *data, std::size_t size, C cmp);

template <typename T>
void heap_down(T *data, std::size_t size);

template <typename T, typename C>
void heap_down(T *data, std::size_t size, C cmp);

inline std::size_t parent(std::size_t i);

inline std::size_t left_child(std::size_t i);

inline std::size_t right_child(std::size_t i);

}  // namespace internal

template <typename T>
static bool less(const T &left, const T &right) {
    return left < right;
}

template <typename T>
static bool greater(const T &left, const T &right) {
    return left > right;
}

template <typename T>
void swap(T &left, T &right) {
    T temp = left;
    left = right;
    right = temp;
}

template <typename T>
void shuffle(T *data, std::size_t size) {
    while(size) {
        swap(data[size - 1], data[rand() % size]);
        --size;
    }
}

template <typename T>
void copy_array(T dest[], const T src[], std::size_t size) {
    for(std::size_t i = 0; i < size; ++i) dest[i] = src[i];
}

template <typename T, typename C>
bool verify(const T *data, std::size_t size, C cmp) {
    bool is_sorted = true;
    if(size > 1) {
        for(std::size_t i = 1; i < size; ++i) {
            if(cmp(data[i], data[i - 1])) {
                is_sorted = false;
                break;
            }
        }
    }

    return is_sorted;
}

template <typename T, typename C = bool (*)(T const &, T const &)>
std::string verifystr(const T *data, std::size_t size, C cmp) {
    bool is_sorted = true;
    if(size > 1) {
        for(std::size_t i = 1; i < size; ++i) {
            if(cmp(data[i], data[i - 1])) {
                is_sorted = false;
                break;
            }
        }
    }

    return is_sorted ? "SORTED" : "NOT SORTED";
}

template <typename T, typename C = bool (*)(T const &, T const &)>
void print_array(const T *data, std::size_t size, bool status_only, C cmp) {
    if(status_only)
        std::cout << verifystr(data, size, cmp) << std::endl;
    else {
        for(std::size_t i = 0; i < size; ++i) std::cout << data[i] << ' ';
        std::cout << '\n' << verifystr(data, size, cmp) << std::endl;
    }
}

template <typename T>
void print_array_segment(T *data, std::size_t start, std::size_t end) {
    while(start != end) std::cout << data[start++] << " ";
    std::cout << std::endl;
}

template <typename T>
void bubble_sort(T *data, std::size_t size) {
    bool swapped = false;

    do {
        swapped = false;

        for(std::size_t i = 1; i < size; ++i) {
            if(data[i] < data[i - 1]) {  // compare current vs previous
                swap(data[i], data[i - 1]);
                swapped = true;
            }
        }

        --size;  // every iteration sink the unwanted item to the end
    } while(swapped);
}

template <typename T, typename C>
void bubble_sort(T *data, std::size_t size, C cmp) {
    bool swapped = false;

    do {
        swapped = false;

        for(std::size_t i = 1; i < size; ++i) {
            if(cmp(data[i], data[i - 1])) {  // compare current vs previous
                swap(data[i], data[i - 1]);
                swapped = true;
            }
        }

        --size;  // every iteration sink the unwanted item to the end
    } while(swapped);
}

template <typename T>
void selection_sort(T *data, std::size_t size) {
    T *min = nullptr;

    for(std::size_t i = 0; i < size - 1; ++i) {  // don't need to swap last
        min = &data[i];

        for(std::size_t j = i + 1; j < size; ++j)  // iterate second set
            if(data[j] < *min) min = &data[j];

        if(data[i] != *min) swap(data[i], *min);
    }
}

template <typename T, typename C>
void selection_sort(T *data, std::size_t size, C cmp) {
    T *min = nullptr;

    for(std::size_t i = 0; i < size - 1; ++i) {  // don't need to swap last
        min = &data[i];

        for(std::size_t j = i + 1; j < size; ++j)  // iterate second set
            if(cmp(data[j], *min)) min = &data[j];

        if(data[i] != *min) swap(data[i], *min);
    }
}

template <typename T>
void insertion_sort(T *data, std::size_t size) {
    int j = 0;  // walker
    T fixed;

    // print_array_segment(data, 0, size);

    for(std::size_t i = 1; i < size; ++i) {
        fixed = data[i];
        j = i;  // set walker to every iteration

        // walker backwards and shift values until fixed is not less than prev
        while(j > 0 && fixed < data[j - 1]) {
            data[j] = data[j - 1];
            --j;
        }

        data[j] = fixed;  // restore fixed back to shifted position
    }
}

template <typename T, typename C>
void insertion_sort(T *data, std::size_t size, C cmp) {
    int j = 0;  // walker
    T fixed;

    // print_array_segment(data, 0, size);

    for(std::size_t i = 1; i < size; ++i) {
        fixed = data[i];
        j = i;  // set walker to every iteration

        // walker backwards and shift values until fixed is not less than prev
        while(j > 0 && cmp(fixed, data[j - 1])) {
            data[j] = data[j - 1];
            --j;
        }

        data[j] = fixed;  // restore fixed back to shifted position
    }
}

template <typename T>
void merge_sort(T *data, std::size_t size) {
    if(size > 1) {  // merge sort when 2 or more elements
        std::size_t size1 = size / 2;
        std::size_t size2 = size - size1;

        merge_sort(data, size1);              // recurse left half
        merge_sort(data + size1, size2);      // recurse right half
        internal::merge(data, size1, size2);  // merge both halves
    }
}

template <typename T, typename C>
void merge_sort(T *data, std::size_t size, C cmp) {
    if(size > 1) {  // merge sort when 2 or more elements
        std::size_t size1 = size / 2;
        std::size_t size2 = size - size1;

        merge_sort(data, size1, cmp);              // recurse left half
        merge_sort(data + size1, size2, cmp);      // recurse right half
        internal::merge(data, size1, size2, cmp);  // merge both halves
    }
}

template <typename T>
void quick_sort(T *data, std::size_t size) {
    if(size > 1) {
        std::size_t pivot = internal::partition(data, size);
        quick_sort(data, pivot);                         // recurse b4 pivot
        quick_sort(data + pivot + 1, size - pivot - 1);  // after
    }
}

template <typename T, typename C>
void quick_sort(T *data, std::size_t size, C cmp) {
    if(size > 1) {
        std::size_t pivot = internal::partition(data, size, cmp);
        quick_sort(data, pivot, cmp);  // recurse b4 pivot
        quick_sort(data + pivot + 1, size - pivot - 1, cmp);  // after
    }
}

template <typename T>
void quick_sort2(T *data, std::size_t size) {
    if(size > 1) {
        std::size_t pivot = 0;  // pivot @ index 0

        // partition data and find
        internal::partition2(data, size, pivot);
        std::size_t size1 = pivot;
        std::size_t size2 = size - size1 - 1;

        quick_sort2(data, size1);              // recurse before pivot
        quick_sort2(data + pivot + 1, size2);  // recurse at pivot and after
    }
}

template <typename T, typename C>
void quick_sort2(T *data, std::size_t size, C cmp) {
    if(size > 1) {
        std::size_t pivot = 0;  // pivot @ front

        // partition data and find
        internal::partition2(data, size, pivot, cmp);
        std::size_t size1 = pivot;
        std::size_t size2 = size - size1 - 1;

        quick_sort2(data, size1, cmp);  // recurse before pivot
        quick_sort2(data + pivot + 1, size2,
                    cmp);  // recurse at pivot and after
    }
}

template <typename T>
void heap_sort(T *data, std::size_t size) {
    std::size_t unsorted = size;

    internal::make_heap(data, size);

    while(unsorted > 1) {
        --unsorted;
        swap(data[0], data[unsorted]);
        internal::heap_down(data, unsorted);
    }
}

template <typename T, typename C>
void heap_sort(T *data, std::size_t size, C cmp) {
    std::size_t unsorted = size;

    internal::make_heap(data, size, cmp);

    while(unsorted > 1) {
        --unsorted;
        swap(data[0], data[unsorted]);
        internal::heap_down(data, unsorted, cmp);
    }
}

// INTERNAL FUNCTIONS
namespace internal {

template <typename T>
void merge(T *data, std::size_t size1, std::size_t size2) {
    std::size_t copied = 0, copied1 = 0, copied2 = 0;
    T *buffer = new T[size1 + size2];  // temporary array

    while((copied1 < size1) && (copied2 < size2)) {  // merge elements to buffer
        if(data[copied1] < (data + size1)[copied2])
            buffer[copied++] = data[copied1++];
        else
            buffer[copied++] = (data + size1)[copied2++];
    }

    // copy untouched but already sorted half to buffer
    while(copied1 < size1) buffer[copied++] = data[copied1++];
    while(copied2 < size2) buffer[copied++] = (data + size1)[copied2++];

    // copy buffer back to data
    for(std::size_t i = 0; i < copied; ++i) data[i] = buffer[i];

    delete[] buffer;
}

template <typename T, typename C>
void merge(T *data, std::size_t size1, std::size_t size2, C cmp) {
    std::size_t copied = 0, copied1 = 0, copied2 = 0;
    T *buffer = new T[size1 + size2];  // temporary array

    while((copied1 < size1) && (copied2 < size2)) {  // merge elements to buffer
        if(cmp(data[copied1], (data + size1)[copied2]))
            buffer[copied++] = data[copied1++];
        else
            buffer[copied++] = (data + size1)[copied2++];
    }

    // copy untouched but already sorted half to buffer
    while(copied1 < size1) buffer[copied++] = data[copied1++];
    while(copied2 < size2) buffer[copied++] = (data + size1)[copied2++];

    // copy buffer back to data
    for(std::size_t i = 0; i < copied; ++i) data[i] = buffer[i];

    delete[] buffer;
}

template <typename T>
std::size_t partition(T *data, std::size_t size) {
    std::size_t pivot = size / 2;  // choose pivot in middle
    std::size_t count = 0;         // counter to remember last swapped position
    std::size_t walker = 0;        // walks through list

    if(size > 0) {
        swap(data[pivot], data[size - 1]);  // swap pivot to end

        while(walker < size - 1) {
            if(data[walker] < data[size - 1]) {
                swap(data[count], data[walker]);
                ++count;
            }
            ++walker;
        }

        swap(data[count], data[size - 1]);  // swap pivot back with counter
    }

    return count;
}

template <typename T, typename C>
std::size_t partition(T *data, std::size_t size, C cmp) {
    std::size_t pivot = size / 2;  // choose pivot in middle
    std::size_t count = 0;         // counter to remember last swapped position
    std::size_t walker = 0;        // walks through list

    if(size > 0) {
        swap(data[pivot], data[size - 1]);  // swap pivot to end

        while(walker < size - 1) {
            if(cmp(data[walker], data[size - 1])) {
                swap(data[count], data[walker]);
                ++count;
            }
            ++walker;
        }

        swap(data[count], data[size - 1]);  // swap pivot back with counter
    }

    return count;
}

template <typename T>
void partition2(T *data, std::size_t size, std::size_t &pivot) {
    std::size_t big = 1;
    std::size_t small = size - 1;

    while(big <= small) {
        while(big < size && data[big] <= data[pivot]) ++big;  // bigger than piv
        while(data[small] > data[pivot]) --small;  // find smaller than pivot

        // swap big/small elements if big/small indices are not crossed over
        if(big < small) swap(data[big], data[small]);
    }

    swap(data[pivot], data[small]);  // swap final small element with pivot's
    pivot = small;                   // return by ref to pivot's new index
}

template <typename T, typename C>
void partition2(T *data, std::size_t size, std::size_t &pivot, C cmp) {
    std::size_t big = 1;
    std::size_t small = size - 1;

    while(big <= small) {
        while(big < size &&
              (cmp(data[big], data[pivot]) || data[big] == data[pivot]))
            ++big;  // bigger than piv
        while(cmp(data[pivot], data[small]))
            --small;  // find smaller than pivot

        // swap big/small elements if big/small indices are not crossed over
        if(big < small) swap(data[big], data[small]);
    }

    swap(data[pivot], data[small]);  // swap final small element with pivot's
    pivot = small;                   // return by ref to pivot's new index
}

template <typename T>
void make_heap(T *data, std::size_t size) {
    std::size_t i;  // index of next element to be added to heap
    std::size_t k;  // index of newly added element pushed up through heap

    for(i = 1; i < size; ++i) {
        k = i;  // k is now child to be pushed up through heap

        // push through while child is greater than parent's
        while(k > 0 && data[parent(k)] < data[k]) {
            swap(data[k], data[parent(k)]);  // swap child and parent
            k = parent(k);  // get new parent's index from child
        }
    }
}

template <typename T, typename C>
void make_heap(T *data, std::size_t size, C cmp) {
    std::size_t i;  // index of next element to be added to heap
    std::size_t k;  // index of newly added element pushed up through heap

    for(i = 1; i < size; ++i) {
        k = i;  // k is now child to be pushed up through heap

        // push through while child is greater than parent's
        while(k > 0 && cmp(data[parent(k)], data[k])) {
            swap(data[k], data[parent(k)]);  // swap child and parent
            k = parent(k);  // get new parent's index from child
        }
    }
}

template <typename T>
void heap_down(T *data, std::size_t size) {
    std::size_t parent = 0;
    std::size_t child;
    bool is_heap = false;

    // walk down still parent still has left child or until heap
    while(!is_heap && left_child(parent) < size) {
        // find larger of child index
        if(right_child(parent) >= size)
            // There is no right child, so left child must be largest
            child = left_child(parent);
        else
            child = data[left_child(parent)] < data[right_child(parent)]
                        ? right_child(parent)
                        : left_child(parent);

        if(data[parent] < data[child]) {  // if parent/child is out of order
            swap(data[parent], data[child]);
            parent = child;
        } else
            is_heap = true;
    }
}

template <typename T, typename C>
void heap_down(T *data, std::size_t size, C cmp) {
    std::size_t parent = 0;
    std::size_t child;
    bool is_heap = false;

    // walk down still parent still has left child or until heap
    while(!is_heap && left_child(parent) < size) {
        // find child index
        if(cmp(0, 1)) {                      // path for less than comparison
            if(right_child(parent) >= size)  // if no right, then must be left
                child = left_child(parent);
            else  // find largest child
                child = cmp(data[left_child(parent)], data[right_child(parent)])
                            ? right_child(parent)
                            : left_child(parent);
        } else {                             // path for greater than comparison
            if(right_child(parent) >= size)  // if no right, then must be left
                child = left_child(parent);
            else  // find smallest child
                child = cmp(data[right_child(parent)], data[left_child(parent)])
                            ? left_child(parent)
                            : right_child(parent);
        }

        if(cmp(data[parent], data[child])) {  // if parent/child is out of order
            swap(data[parent], data[child]);
            parent = child;
        } else
            is_heap = true;
    }
}

std::size_t parent(std::size_t i) { return (i - 1) / 2; }

std::size_t left_child(std::size_t i) { return 2 * i + 1; }

std::size_t right_child(std::size_t i) { return 2 * i + 2; }

}  // namespace internal

}  // namespace sort

#endif  // SORT_H
