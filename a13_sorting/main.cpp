#include <cstdlib>   // srand(), rand()
#include <ctime>     // time()
#include <iomanip>   // setw()
#include <iostream>  // stram objects
#include "../include/sort.h"
#include "../include/timer.h"

struct SortData {
    SortData(bool c = true, bool r = true, double t = 0)
        : is_control_sorted(c), is_result_sorted(r), timings(t) {}
    bool is_control_sorted;
    bool is_result_sorted;
    double timings;
};

void test_sorts_timings(std::size_t sample_size = 50000,
                        std::size_t iterations = 1000);

template <typename F, typename T, typename C>
SortData test_sort_function(F *f, T *data, std::size_t size, C *cmp);

template <typename F, typename T>
SortData test_sort_function(F *f, T *data, std::size_t size);

int main() {
    int sample_size = 10, iterations = 10;

    // sample size 10
    std::cout << "Sample size: " << std::setw(10) << std::left << sample_size
              << "Iterations: " << iterations << "\n";
    std::cout << std::string(80, '-') << std::endl;
    test_sorts_timings(sample_size, iterations);
    std::cout << std::endl;

    // sample size 50
    sample_size = 50;
    std::cout << "Sample size: " << std::setw(10) << std::left << sample_size
              << "Iterations: " << iterations << "\n";
    std::cout << std::string(80, '-') << std::endl;
    test_sorts_timings(sample_size, iterations);
    std::cout << std::endl;

    // sample size 100
    sample_size = 100;
    std::cout << "Sample size: " << std::setw(10) << std::left << sample_size
              << "Iterations: " << iterations << "\n";
    std::cout << std::string(80, '-') << std::endl;
    test_sorts_timings(sample_size, iterations);
    std::cout << std::endl;

    // sample size 150
    sample_size = 150;
    std::cout << "Sample size: " << std::setw(10) << std::left << sample_size
              << "Iterations: " << iterations << "\n";
    std::cout << std::string(80, '-') << std::endl;
    test_sorts_timings(sample_size, iterations);
    std::cout << std::endl;

    // sample size 200
    sample_size = 200;
    std::cout << "Sample size: " << std::setw(10) << std::left << sample_size
              << "Iterations: " << iterations << "\n";
    std::cout << std::string(80, '-') << std::endl;
    test_sorts_timings(sample_size, iterations);
    std::cout << std::endl;

    // sample size 1000
    sample_size = 1000;
    std::cout << "Sample size: " << std::setw(10) << std::left << sample_size
              << "Iterations: " << iterations << "\n";
    std::cout << std::string(80, '-') << std::endl;
    test_sorts_timings(sample_size, iterations);
    std::cout << std::endl;

    // sample size 10000
    sample_size = 10000;
    std::cout << "Sample size: " << std::setw(10) << std::left << sample_size
              << "Iterations: " << iterations << "\n";
    std::cout << std::string(80, '-') << std::endl;
    test_sorts_timings(sample_size, iterations);
    std::cout << std::endl;

    // sample size 50000
    sample_size = 50000;
    std::cout << "Sample size: " << std::setw(10) << std::left << sample_size
              << "Iterations: " << iterations << "\n";
    std::cout << std::string(80, '-') << std::endl;
    test_sorts_timings(sample_size, iterations);

    return 0;
}

void test_sorts_timings(std::size_t sample_size, std::size_t iterations) {
    const int FUNC_SIZE = 9, HEADER_SIZE = 4;
    timer::ChronoTimer chrono;

    // arrays of headers and names
    std::string headers[HEADER_SIZE] = {"    Sorting Algorithm", "CONTROL ",
                                        "RESULT ", "  Timings (s)"};
    std::string names[FUNC_SIZE] = {
        "Bubble", "Selection",         "Insertion",
        "Merge",  "Quick @mid",        "Quick @front",
        "Heap",   "Merge w/o cmp ptr", "Quick w/o cmp ptr"};

    // declare function pointer type
    typedef void (*fn_ptr1)(int *data, std::size_t size,
                            bool (*cmp)(const int &l, const int &r));
    typedef void (*fn_ptr2)(int *data, std::size_t size);

    // array of function pointers to sort functions
    fn_ptr1 fn_arr1[FUNC_SIZE] = {
        &sort::bubble_sort<int>,    &sort::selection_sort<int>,
        &sort::insertion_sort<int>, &sort::merge_sort<int>,
        &sort::quick_sort<int>,     &sort::quick_sort2<int>,
        &sort::heap_sort<int>};

    fn_ptr2 fn_arr2[2] = {&sort::merge_sort2<int>, &sort::quick_sort3<int>};

    // data declaration and allocations
    SortData results[FUNC_SIZE];
    int *arr1 = nullptr, *arr2 = nullptr;
    arr1 = new int[sample_size];
    arr2 = new int[sample_size];

    // populate array
    for(std::size_t i = 0; i < sample_size; ++i) arr2[i] = i;

    for(std::size_t i = 0; i < iterations; ++i) {  // shuffle array
        sort::shuffle(arr2, sample_size);

        // iterate through array of function pointeres to test all sort
        // functions
        for(int j = 0; j < FUNC_SIZE; ++j) {
            SortData result;
            sort::copy_array(arr1, arr2, sample_size);

            if(j < 7)  // test functions with comparison pointers
                result = test_sort_function(fn_arr1[j], arr1, sample_size,
                                            sort::less<int>);
            else  // test merge and quick w/o comparison pointers
                result = test_sort_function(fn_arr2[j % 7], arr1, sample_size);

            results[j].is_control_sorted &= result.is_control_sorted;
            results[j].is_result_sorted &= result.is_result_sorted;
            results[j].timings += result.timings;
        }
    }

    // iterate through all sorting type and average timings by iterations
    for(int i = 0; i < FUNC_SIZE; ++i) results[i].timings /= iterations;

    // print headers
    for(int i = 0; i < HEADER_SIZE; ++i) {
        if(i == 0) {
            std::cout << std::right;
            std::cout << std::setw(headers[i].size());
            std::cout << headers[i] << "  ";
        } else {
            std::cout << std::left;
            std::cout << std::setw(headers[i].size());
            std::cout << headers[i] << " ";
        }
    }
    std::cout << std::endl;

    // print bars
    for(int i = 0; i < HEADER_SIZE; ++i) {
        if(i == 0) {
            std::cout << std::right;
            std::cout << std::string(headers[i].size(), '-') << "  ";
        } else if(i == 3) {
            std::cout << std::left;
            std::cout << std::string(headers[i].size(), '-');
        } else {
            std::cout << std::left;
            std::cout << std::string(headers[i].size(), '-') << " ";
        }
    }
    std::cout << std::endl;

    // print data
    for(int i = 0; i < FUNC_SIZE; ++i) {
        std::cout << std::right;
        std::cout << std::setw(headers[0].size()) << names[i] << "  ";

        std::cout << std::left;
        std::cout << std::setw(headers[1].size() + 1);
        if(results[i].is_control_sorted)
            std::cout << "SORTED";
        else
            std::cout << "!SORTED";

        std::cout << std::left;
        std::cout << std::setw(headers[2].size() + 1);
        if(results[i].is_result_sorted)
            std::cout << "SORTED";
        else
            std::cout << "!SORTED";

        std::cout.setf(std::ios::right | std::ios::fixed | std::ios::showpoint);
        std::cout << std::setw(headers[3].size()) << std::setprecision(8)
                  << results[i].timings << std::endl;
    }

    // delete arrays
    delete[] arr1;
    delete[] arr2;
}

template <typename F, typename T, typename C>
SortData test_sort_function(F *f, T *data, std::size_t size, C *cmp) {
    bool is_control_sorted = true, is_result_sorted = false;
    timer::ChronoTimer chrono;

    // verify control data is not sorted
    is_control_sorted = sort::verify(data, size, cmp);

    // start timings
    chrono.start();
    f(data, size, cmp);
    chrono.stop();

    // verify result data is sorted
    is_result_sorted = sort::verify(data, size, cmp);

    return SortData(is_control_sorted, is_result_sorted, chrono.seconds());
}

template <typename F, typename T>
SortData test_sort_function(F *f, T *data, std::size_t size) {
    bool is_control_sorted = true, is_result_sorted = false;
    timer::ChronoTimer chrono;

    // verify control data is not sorted
    is_control_sorted = sort::verify(data, size);

    // start timings
    chrono.start();
    f(data, size);
    chrono.stop();

    // verify result data is sorted
    is_result_sorted = sort::verify(data, size);

    return SortData(is_control_sorted, is_result_sorted, chrono.seconds());
}
