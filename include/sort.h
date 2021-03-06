/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : sort
 * DESCRIPTION : This header defines a templated sorting functions.
 *      - Bubble sort, selection sort, insertion sort, merge sort, quick sort,
 *        heap sort, introspective sort.
 *      - Includes various helper functions to shuffle, copy and print arrays.
 ******************************************************************************/
#ifndef SORT_H
#define SORT_H

#include <cmath>    // log()
#include <cstdlib>  // rand(), srand()
#include <string>   // string objects
#include <utility>  // move()

namespace sort {

template <typename T>
auto compare = [](const T &l, const T &r) { return l < r; };

template <typename T>
inline void swap(T &a, T &b);

template <typename T>
void shuffle(T *data, std::size_t size);

template <typename T>
void copy_array(T *dest, const T *src, std::size_t size);

// verify data's sortedness
template <typename T, typename C = decltype(compare<T>)>
bool verify(const T *data, std::size_t size, C cmp = compare<T>);

// verify sortedness return "SORTED" or "NOT SORTED"
template <typename T, typename C = decltype(compare<T>)>
std::string verifystr(const T *data, std::size_t size, C cmp = compare<T>);

// print the array if status_only is true with info on sortedness of data[]
template <typename T, typename C = decltype(compare<T>)>
void print_array(const T *data, std::size_t size, C cmp = compare<T>,
                 bool status_only = false);

template <typename T>
void print_array_segment(T *data, std::size_t start, std::size_t end);

template <typename T, typename C = decltype(compare<T>)>
void bubble_sort(T *data, std::size_t size, C cmp = compare<T>);

template <typename T, typename C = decltype(compare<T>)>
void selection_sort(T *data, std::size_t size, C cmp = compare<T>);

template <typename T, typename C = decltype(compare<T>)>
void insertion_sort(T *data, std::size_t size, C cmp = compare<T>);

template <typename T, typename C = decltype(compare<T>)>
void merge_sort(T *data, std::size_t size, C cmp = compare<T>);

template <typename T, typename C = decltype(compare<T>)>
void quick_sort(T *data, std::size_t size, C cmp = compare<T>);

template <typename T, typename C = decltype(compare<T>)>
void quick2_sort(T *data, std::size_t size, C cmp = compare<T>);

template <typename T, typename C = decltype(compare<T>)>
void heap_sort(T *data, std::size_t size, C cmp = compare<T>);

template <typename T, typename C = decltype(compare<T>)>
void intro_sort(T *data, std::size_t size, C cmp = compare<T>);

template <typename T, typename C = decltype(compare<T>)>
void intro2_sort(T *data, std::size_t size, C cmp = compare<T>);

// INTERNAL DECLARATIONS
namespace internal {

template <typename T>
bool verify_less(const T *data, std::size_t size);

template <typename T>
bool verify_greater(const T *data, std::size_t size);

template <typename T, typename C = decltype(compare<T>)>
void merge(T *data, std::size_t size1, std::size_t size2, C cmp);

template <typename T, typename C = decltype(compare<T>)>
std::size_t partition(T *data, std::size_t size, C cmp);

template <typename T, typename C = decltype(compare<T>)>
void partition2(T *data, std::size_t size, std::size_t &pivot, C cmp);

template <typename T, typename C = decltype(compare<T>)>
void make_heap(T *data, std::size_t size, C cmp);

template <typename T, typename C = decltype(compare<T>)>
void heap_down(T *data, std::size_t size, C cmp);

inline std::size_t parent(std::size_t i);

inline std::size_t left_child(std::size_t i);

inline std::size_t right_child(std::size_t i);

template <typename T, typename C = decltype(compare<T>)>
void intro(T *data, std::size_t size, C cmp, std::size_t &limit,
           std::size_t depth);

template <typename T, typename C = decltype(compare<T>)>
void intro2(T *data, std::size_t size, C cmp, std::size_t &limit,
            std::size_t depth);

}  // namespace internal

/*******************************************************************************
 * DESCRIPTION:
 *  Swap two items via std::move
 *
 * PRE-CONDITIONS:
 *  T& a: templated item by reference
 *  T& b: templated item by reference
 *
 * POST-CONDITIONS:
 *  a and b are swapped.
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void swap(T &a, T &b) {
    T temp = std::move(a);
    a = std::move(b);
    b = std::move(temp);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Randomly shuffle contents in array using rand().
 *
 * PRE-CONDITIONS:
 *  T *data         : templated array
 *  std::size_t size: size of array
 *
 * POST-CONDITIONS:
 *  array shuffled
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void shuffle(T *data, std::size_t size) {
    if(size)
        while(--size) swap(data[size], data[rand() % size]);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Copy contents of one array to another.
 *
 * PRE-CONDITIONS:
 *  T *dest         : valid destination
 *  const T *src    : valid source
 *  std::size_t size: size of src == destination
 *
 * POST-CONDITIONS:
 *  T *dest: unique copy of src.
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void copy_array(T *dest, const T *src, std::size_t size) {
    for(std::size_t i = 0; i < size; ++i) dest[i] = src[i];
}

/*******************************************************************************
 * DESCRIPTION:
 *  Verify sortedness of the array, using specified relation order, cmp.
 *
 * PRE-CONDITIONS:
 *  T *data         : templated array
 *  std::size_t size: size of array
 *  bool (*cmp)     : comparision function
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  boolean
 ******************************************************************************/
template <typename T, typename C>
bool verify(const T *data, std::size_t size, C cmp) {
    bool is_sorted = true;

    if(size)
        while(--size) {
            if(cmp(data[size], data[size - 1])) {
                is_sorted = false;
                break;
            }
        }

    return is_sorted;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Verify sortedness of the array, using specified relation order, cmp.
 *
 * PRE-CONDITIONS:
 *  T *data         : templated array
 *  std::size_t size: size of array
 *  bool (*cmp)     : comparision function
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  std::string: "SORTED" or "NOT SORTED"
 ******************************************************************************/
template <typename T, typename C>
std::string verifystr(const T *data, std::size_t size, C cmp) {
    return verify(data, size, cmp) ? "SORTED" : "NOT SORTED";
}

/*******************************************************************************
 * DESCRIPTION:
 *  Print the array along with sortedness information.
 *
 * PRE-CONDITIONS:
 *  T *data         : templated array
 *  std::size_t size: size of array
 *  bool cmp        : enum of sort::less, or sort::greater
 *  bool (*cmp)     : comparision function
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename C>
void print_array(const T *data, std::size_t size, C cmp, bool status_only) {
    if(status_only)
        std::cout << verifystr(data, size, cmp) << std::endl;
    else {
        for(std::size_t i = 0; i < size; ++i) std::cout << data[i] << ' ';
        std::cout << " --> " << verifystr(data, size, cmp) << std::endl;
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Print from segment [start, end).
 *
 * PRE-CONDITIONS:
 *  T *data          : templated array
 *  std::size_t start: inclusive start point
 *  std::size_t end  : exclusive endpoint
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void print_array_segment(T *data, std::size_t start, std::size_t end) {
    while(start != end) std::cout << data[start++] << " ";
    std::cout << std::endl;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Bubble sort algorithm, of comparision function cmp.
 *  Slowest.
 *
 *  Time complexity : O(n2)
 *  Space complexity: O(1)
 *
 * PRE-CONDITIONS:
 *  T *data         : templated array
 *  std::size_t size: size of array
 *  bool (*cmp)     : comparision function
 *
 * POST-CONDITIONS:
 *  T * data: sorted by relation order, cmp
 *
 * RETURN:
 *  none
 ******************************************************************************/
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

/*******************************************************************************
 * DESCRIPTION:
 *  Selection sort algorithm, of comparision function cmp. Slowest.
 *  Slightly faster than bubble.
 *
 *  Time complexity : O(n2)
 *  Space complexity: O(1)
 *
 * PRE-CONDITIONS:
 *  T *data         : templated array
 *  std::size_t size: size of array
 *  bool (*cmp)     : comparision function
 *
 * POST-CONDITIONS:
 *  T * data: sorted by relation order, cmp
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename C>
void selection_sort(T *data, std::size_t size, C cmp) {
    if(size) {
        T *min = nullptr;

        for(std::size_t i = 0; i < size - 1; ++i) {  // don't need to swap last
            min = &data[i];

            for(std::size_t j = i + 1; j < size; ++j)  // iterate second set
                if(cmp(data[j], *min)) min = &data[j];

            if(data[i] != *min) swap(data[i], *min);
        }
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Insertion sort algorithm, of comparision function cmp.
 *  Fastest on small arrays of size 1 to ~64.
 *
 *  Time complexity : O(n2)
 *  Space complexity: O(1)
 *
 * PRE-CONDITIONS:
 *  T *data         : templated array
 *  std::size_t size: size of array
 *  bool (*cmp)     : comparision function
 *
 * POST-CONDITIONS:
 *  T * data: sorted by relation order, cmp
 *
 * RETURN:
 *  none
 ******************************************************************************/
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
            data[j] = std::move(data[j - 1]);
            --j;
        }

        data[j] = std::move(fixed);  // restore fixed back to shifted position
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Merge sort algorithm, of comparision function cmp.
 *
 *  On large arrays, faster than bubble, selection, insertion. Has better
 *  worst case than quick, heap and introspective sort. However, space
 *  complexity is undesired because it requires more memory to allocate
 *  new array for merging partitions together, which also incur overhead.
 *
 *  Time complexity : O(n log n) for worst, avg, and best
 *  Space complexity: O(n)
 *
 * PRE-CONDITIONS:
 *  T *data         : templated array
 *  std::size_t size: size of array
 *  bool (*cmp)     : comparision function
 *
 * POST-CONDITIONS:
 *  T * data: sorted by relation order, cmp
 *
 * RETURN:
 *  none
 ******************************************************************************/
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

/*******************************************************************************
 * DESCRIPTION:
 *  Quick sort algorithm, of comparision function cmp.
 *  This version uses median of front, mid, and end as pivot.
 *
 *  On very large arrays, this has second fastest. However, worst case time
 *  complexity is slowest of O(n2) if the data is nearly sorted.
 *
 *  Time complexity : O(n log n) avg, and best but O(n2) for worst.
 *  Space complexity: O(1)
 *
 * PRE-CONDITIONS:
 *  T *data         : templated array
 *  std::size_t size: size of array
 *  bool (*cmp)     : comparision function
 *
 * POST-CONDITIONS:
 *  T * data: sorted by relation order, cmp
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename C>
void quick_sort(T *data, std::size_t size, C cmp) {
    if(size) {
        std::size_t pivot = internal::partition(data, size, cmp);
        quick_sort(data, pivot, cmp);  // recurse b4 pivot
        quick_sort(data + pivot + 1, size - pivot - 1, cmp);  // after
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Quick sort algorithm, of comparision function cmp.
 *  This version uses front as pivot. Slightly slower than version 1 on some
 *  data sets that's somewhat sorted.
 *
 *  On very large arrays, this has second fastest. However, worst case time
 *  complexity is slowest of O(n2) if the data is nearly sorted.
 *
 *  Time complexity : O(n log n) avg, and best but O(n2) for worst.
 *  Space complexity: O(1)
 *
 * PRE-CONDITIONS:
 *  T *data         : templated array
 *  std::size_t size: size of array
 *  bool (*cmp)     : comparision function
 *
 * POST-CONDITIONS:
 *  T * data: sorted by relation order, cmp
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename C>
void quick2_sort(T *data, std::size_t size, C cmp) {
    if(size) {
        std::size_t pivot = 0;  // pivot @ front

        // partition data and find
        internal::partition2(data, size, pivot, cmp);
        std::size_t size1 = pivot;
        std::size_t size2 = size - size1 - 1;

        quick2_sort(data, size1, cmp);              // recurse before pivot
        quick2_sort(data + pivot + 1, size2, cmp);  // recurse after pivot
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Heap sort algorithm, of comparision function cmp.
 *  Faster than merge but slower than quick and introspection.
 *
 *  On very large arrays, this has third fastest speed. Even though the time
 *  complexity, O(2n log n), is theoretically slower than merge, in practice
 *  it is faster because merge has overhead allocating space to merge
 *  partitions.
 *
 *  Time complexity : O(n log n) for worst, avg, and best
 *  Space complexity: O(1)
 *
 * PRE-CONDITIONS:
 *  T *data         : templated array
 *  std::size_t size: size of array
 *  bool (*cmp)     : comparision function
 *
 * POST-CONDITIONS:
 *  T * data: sorted by relation order, cmp
 *
 * RETURN:
 *  none
 ******************************************************************************/
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

/*******************************************************************************
 * DESCRIPTION:
 *  Heap sort algorithm, of comparision function cmp.
 *  This version uses median of front, mid, and end as pivot.
 *  Fastest.
 *
 *  On very large arrays, fastest algorithm in pratice. It combines the
 *  benefits using insertion on fastest sort on SMALL arrays, and heap sort
 *  when recursion goes too deep and else uses quick sort to divide and
 *  conquer.
 *
 *  Time complexity : O(n log n) avg, and best but O(n2) for worst.
 *  Space complexity: O(1)
 *
 * PRE-CONDITIONS:
 *  T *data         : templated array
 *  std::size_t size: size of array
 *  bool (*cmp)     : comparision function
 *
 * POST-CONDITIONS:
 *  T * data: sorted by relation order, cmp
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename C>
void intro_sort(T *data, std::size_t size, C cmp) {
    std::size_t limit = 2 * (std::size_t)log(size);
    internal::intro(data, size, cmp, limit, 0);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Heap sort algorithm, of comparision function cmp.
 *  This version uses front as pivot. Slightly slower than version 1 on some
 *  data sets that's somewhat sorted.
 *  Fast but slightly slower than version 1.
 *
 *  On very large arrays, fastest algorithm in pratice. It combines the
 *  benefits using insertion on fastest sort on SMALL arrays, and heap sort
 *  when recursion goes too deep and else uses quick sort to divide and
 *  conquer.
 *
 *  Time complexity : O(n log n) avg, and best but O(n2) for worst.
 *  Space complexity: O(1)
 *
 * PRE-CONDITIONS:
 *  T *data         : templated array
 *  std::size_t size: size of array
 *  bool (*cmp)     : comparision function
 *
 * POST-CONDITIONS:
 *  T * data: sorted by relation order, cmp
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename C>
void intro2_sort(T *data, std::size_t size, C cmp) {
    std::size_t limit = 2 * (std::size_t)log(size);
    internal::intro2(data, size, cmp, limit, 0);
}

// INTERNAL FUNCTIONS
namespace internal {

/*******************************************************************************
 * DESCRIPTION:
 *  Internal function to verify data in ascending order.
 *
 * PRE-CONDITIONS:
 *  T *data         : templated array
 *  std::size_t size: size of array
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  boolean
 ******************************************************************************/
template <typename T>
bool verify_less(const T *data, std::size_t size) {
    bool is_sorted = true;

    if(size)
        while(--size) {
            if(data[size] < data[size - 1]) {
                is_sorted = false;
                break;
            }
        }

    return is_sorted;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Internal function to verify data in descending order.
 *
 * PRE-CONDITIONS:
 *  T *data         : templated array
 *  std::size_t size: size of array
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  boolean
 ******************************************************************************/
template <typename T>
bool verify_greater(const T *data, std::size_t size) {
    bool is_sorted = true;

    if(size)
        while(--size) {
            if(data[size] > data[size - 1]) {
                is_sorted = false;
                break;
            }
        }

    return is_sorted;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Internal function to merge two partitions using comparision function.
 *
 * PRE-CONDITIONS:
 *  T *data          : templated array
 *  std::size_t size1: first partition size, ie midpoint of entire array
 *  std::size_t size2: second partition size, ie endpoint of entire array
 *  bool (*cmp)     : comparison function
 *
 * POST-CONDITIONS:
 *  T *data: two partitions of *data is merged in sorted order.
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename C>
void merge(T *data, std::size_t size1, std::size_t size2, C cmp) {
    std::size_t cpy = 0, cpy1 = 0, cpy2 = 0;
    T *buffer = new T[size1 + size2];  // temporary array

    while((cpy1 < size1) && (cpy2 < size2)) {  // merge elements to buffer
        if(cmp(data[cpy1], (data + size1)[cpy2]))
            buffer[cpy++] = std::move(data[cpy1++]);
        else
            buffer[cpy++] = std::move((data + size1)[cpy2++]);
    }

    // copy untouched but already sorted half to buffer
    while(cpy1 < size1) buffer[cpy++] = std::move(data[cpy1++]);
    while(cpy2 < size2) buffer[cpy++] = std::move((data + size1)[cpy2++]);

    // copy buffer back to data
    for(std::size_t i = 0; i < cpy; ++i) data[i] = std::move(buffer[i]);

    delete[] buffer;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Internal function to partition data using comparison function.
 *  Partition data by pivot: median of front, mid, and end. Used for quick
 *  sort version 1.
 *
 * PRE-CONDITIONS:
 *  T *data         : templated array
 *  std::size_t size: size of array
 *  bool (*cmp)     : comparison function
 *
 * POST-CONDITIONS:
 *  T *data: all data moved left/right of pivot
 *
 * RETURN:
 *  std::size_t: pivot index
 ******************************************************************************/
template <typename T, typename C>
std::size_t partition(T *data, std::size_t size, C cmp) {
    std::size_t count = 0;   // counter to remember last swapped position
    std::size_t walker = 0;  // walks through list

    if(size) {
        std::size_t mid = size / 2;  // find midpoint
        std::size_t pivot;           // pivot

        // get pivot from median of front, middle, end
        if(data[0] > data[mid]) {
            if(data[mid] > data[size - 1])
                pivot = mid;
            else if(data[0] > data[size - 1])
                pivot = size - 1;
            else
                pivot = 0;
        } else {
            if(data[0] > data[size - 1])
                pivot = 0;
            else if(data[mid] > data[size - 1])
                pivot = size - 1;
            else
                pivot = mid;
        }

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

/*******************************************************************************
 * DESCRIPTION:
 *  Internal function to partition data using comparision function.
 *  Partition data by pivot @ front. Used for quick sort version 2.
 *
 * PRE-CONDITIONS:
 *  T *data         : templated array
 *  std::size_t size: size of array
 *  bool (*cmp)     : comparison function
 *
 * POST-CONDITIONS:
 *  T *data    : all data moved left/right of pivot
 *  std::size_t: pivot index by reference
 *
 * RETURN:
 *  none
 ******************************************************************************/
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

/*******************************************************************************
 * DESCRIPTION:
 *  Internal function to convert unsorted data into a binary heap structure,
 *  using comparision function.
 *  Data is still unsorted but has correct parent and children node indices.
 *
 * PRE-CONDITIONS:
 *  T *data         : templated array
 *  std::size_t size: size of array
 *  bool (*cmp)     : comparison function
 *
 * POST-CONDITIONS:
 *  T *data: unsorted but with corect parent/children indices placements
 *
 * RETURN:
 *  none
 ******************************************************************************/
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

/*******************************************************************************
 * DESCRIPTION:
 *  Internal function to move incorrect parent's order downwards through the
 *  binary heap structure, in comparision function.
 *
 * PRE-CONDITIONS:
 *  T *data         : templated array
 *  std::size_t size: size of array
 *  bool (*cmp)     : comparison function
 *
 * POST-CONDITIONS:
 *  T *data: unsorted, but with parent in correct place.
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename C>
void heap_down(T *data, std::size_t size, C cmp) {
    std::size_t parent = 0;
    std::size_t child;
    bool is_heap = false;

    // walk down still parent still has left child or until heap
    while(!is_heap && left_child(parent) < size) {
        // find child index
        if(right_child(parent) >= size)  // if no right, then must be left
            child = left_child(parent);
        else
            child = cmp(data[left_child(parent)], data[right_child(parent)])
                        ? right_child(parent)
                        : left_child(parent);

        if(cmp(data[parent], data[child])) {  // if parent/child is out of order
            swap(data[parent], data[child]);
            parent = child;
        } else
            is_heap = true;
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Internal function to find the parent's index with child.
 *
 * PRE-CONDITIONS:
 *  std::size_t i: child index
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  std::size_t: parent's index
 ******************************************************************************/
std::size_t parent(std::size_t i) { return (i - 1) / 2; }

/*******************************************************************************
 * DESCRIPTION:
 *  Internal function to find the left child's index with parent's index.
 *
 * PRE-CONDITIONS:
 *  std::size_t i: parent index
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  std::size_t: left child's index
 ******************************************************************************/
std::size_t left_child(std::size_t i) { return 2 * i + 1; }

/*******************************************************************************
 * DESCRIPTION:
 *  Internal function to find the right child's index with parent's index.
 *
 * PRE-CONDITIONS:
 *  std::size_t i: parent index
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  std::size_t: right child's index
 ******************************************************************************/
std::size_t right_child(std::size_t i) { return 2 * i + 2; }

/*******************************************************************************
 * DESCRIPTION:
 *  Internal function to introspectively sort data in comparison function. This
 *  algorithm a hybrid of insertion sort, heap sort and quick sort. When data
 *  size is small, it uses insertion. When recursion depth goes too deep, it
 *  switches to heap sort. Otherwise, it uses quick sort to divide and
 *  conquer. This version uses quick sort version 1.
 *
 * PRE-CONDITIONS:
 *  T *data           : templated array
 *  std::size_t size  : size of array
 *  bool (*cmp)       : comparison function
 *  std::size_t &limit: recursion limit
 *  td::size_t depth  : recursion depth
 *
 * POST-CONDITIONS:
 *  T *data: sorted
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename C>
void intro(T *data, std::size_t size, C cmp, std::size_t &limit,
           std::size_t depth) {
    if(size) {
        if(size < 64)  // *research* says that < 16 is optimal for insertion
            insertion_sort(data, size, cmp);
        else if(depth > limit)  // *research* says > 62 is optimal for heap
            heap_sort(data, size, cmp);
        else {  // else use quicksort
            std::size_t pivot = partition(data, size, cmp);
            intro(data, pivot, cmp, limit, depth + 1);  // recurse b4 pivot
            intro(data + pivot + 1, size - pivot - 1, cmp, limit,
                  depth + 1);  // after pivot
        }
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Internal function to introspectively sort data in comparison function. This
 *  algorithm a hybrid of insertion sort, heap sort and quick sort. When data
 *  size is small, it uses insertion. When recursion depth goes too deep, it
 *  switches to heap sort. Otherwise, it uses quick sort to divide and
 *  conquer. This version uses quick sort version 2.
 *
 * PRE-CONDITIONS:
 *  T *data           : templated array
 *  std::size_t size  : size of array
 *  bool (*cmp)       : comparison function
 *  std::size_t &limit: recursion limit
 *  td::size_t depth  : recursion depth
 *
 * POST-CONDITIONS:
 *  T *data: sorted
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T, typename C>
void intro2(T *data, std::size_t size, C cmp, std::size_t &limit,
            std::size_t depth) {
    if(size) {
        if(size < 64)  // *research* says that < 16 is optimal for insertion
            insertion_sort(data, size, cmp);
        else if(depth > limit)  // *research* says > 62 is optimal for heap
            heap_sort(data, size, cmp);
        else {
            std::size_t pivot = 0;  // pivot @ front

            // partition data and find
            internal::partition2(data, size, pivot, cmp);
            std::size_t size1 = pivot;
            std::size_t size2 = size - size1 - 1;

            intro2(data, size1, cmp, limit, depth);  // recurse before pivot
            intro2(data + pivot + 1, size2, cmp, limit, depth);  // after pivot
        }
    }
}

}  // namespace internal

}  // namespace sort

#endif  // SORT_H
