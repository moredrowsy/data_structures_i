#include <cstdlib>             // srand(), rand()
#include <ctime>               // time()
#include <iomanip>             // setw()
#include <iostream>            // stram objects
#include "../include/sort.h"   // various sort functions
#include "../include/timer.h"  // ChronoTimer class

struct SortData {  // stores sortedness data
    SortData(std::string n = "", bool c = true, bool r = true, double t = 0)
        : is_ctrl_sorted(c), is_rslt_sorted(r), timings(t), name(n) {}
    bool is_ctrl_sorted;  // is control sorted?
    bool is_rslt_sorted;  // is result sorted?
    double timings;       // timings
    std::string name;     // name of data
};

void test_sorts_timings(std::size_t sample_size = 50000,
                        std::size_t iterations = 1000);

template <typename F, typename T, typename C>
SortData test_sort_function(F *f, T *data, std::size_t size, C *cmp,
                            std::string name);

template <typename F, typename T>
SortData test_sort_function(F *f, T *data, std::size_t size, std::string name);

int main() {
    int sample_size = 10, iterations = 15;

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
    const int FUNC_SIZE = 7, HEADER_SIZE = 4;
    timer::ChronoTimer chrono;

    // arrays of headers and names
    std::string headers[HEADER_SIZE] = {"    Sorting Algorithm", "CONTROL ",
                                        "RESULT ", "  Timings (ms)"};
    std::string names1[FUNC_SIZE] = {"Bubble", "Selection",  "Insertion",
                                     "Merge",  "Quick @mid", "Quick @front",
                                     "Heap"};
    std::string names2[FUNC_SIZE] = {
        "Bubble w/o cmp", "Selection w/o cmp",  "Insertion w/o cmp",
        "Merge w/o cmp",  "Quick @mid w/o cmp", "Quick @front w/o cmp",
        "Heap w/o cmp"};

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

    // array of function pointers to sort functions w/o cmp function pointer
    fn_ptr2 fn_arr2[FUNC_SIZE] = {
        &sort::bubble_sort<int>,    &sort::selection_sort<int>,
        &sort::insertion_sort<int>, &sort::merge_sort<int>,
        &sort::quick_sort<int>,     &sort::quick_sort2<int>,
        &sort::heap_sort<int>};

    // data declaration and allocations
    SortData results1[FUNC_SIZE], results2[FUNC_SIZE];
    int *arr1 = nullptr, *arr2 = nullptr;
    arr1 = new int[sample_size];
    arr2 = new int[sample_size];

    // populate array 2
    for(std::size_t i = 0; i < sample_size; ++i) arr2[i] = i;

    // array 1 will be copied from array 2 at every iterations
    for(std::size_t i = 0; i < iterations; ++i) {
        sort::shuffle(arr2, sample_size);  // shuffle array 2

        // iterate array of function pointeres to test all sort functions
        for(int j = 0; j < FUNC_SIZE; ++j) {
            SortData result;
            sort::copy_array(arr1, arr2, sample_size);  // copy arr2 to arr1

            // test functions with comparison pointers with arr1
            result = test_sort_function(fn_arr1[j], arr1, sample_size,
                                        sort::less<int>, names1[j]);

            results1[j].name = result.name;
            results1[j].is_ctrl_sorted &= result.is_ctrl_sorted;  // control
            results1[j].is_rslt_sorted &= result.is_rslt_sorted;  // results
            results1[j].timings += result.timings;  // results timing
        }

        // test functions sort functions w/o comparison functions
        for(int j = 0; j < FUNC_SIZE; ++j) {
            SortData result;
            sort::copy_array(arr1, arr2, sample_size);  // copy arr2 to arr1

            // test merge and quick w/o comparison pointers with arr1
            result = test_sort_function(fn_arr2[j % 7], arr1, sample_size,
                                        names2[j]);

            results2[j].name = result.name;
            results2[j].is_ctrl_sorted &= result.is_ctrl_sorted;
            results2[j].is_rslt_sorted &= result.is_rslt_sorted;
            results2[j].timings += result.timings;
        }
    }

    // calculate average for results1
    for(int i = 0; i < FUNC_SIZE; ++i) results1[i].timings /= iterations;

    // calculate average for results2
    for(int i = 0; i < FUNC_SIZE; ++i) results2[i].timings /= iterations;

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
        std::cout << std::setw(headers[0].size()) << results1[i].name << "  ";

        std::cout << std::left;
        std::cout << std::setw(headers[1].size() + 1);
        if(results1[i].is_ctrl_sorted)
            std::cout << "SORTED";
        else
            std::cout << "!SORTED";

        std::cout << std::left;
        std::cout << std::setw(headers[2].size() + 1);
        if(results1[i].is_rslt_sorted)
            std::cout << "SORTED";
        else
            std::cout << "!SORTED";

        std::cout.setf(std::ios::right | std::ios::fixed | std::ios::showpoint);
        std::cout << std::setw(headers[3].size()) << std::setprecision(5)
                  << results1[i].timings * 1000 << std::endl;

        // data for functions w/o comparison function pointer
        std::cout << std::right;
        std::cout << std::setw(headers[0].size()) << results2[i].name << "  ";

        std::cout << std::left;
        std::cout << std::setw(headers[1].size() + 1);
        if(results2[i].is_ctrl_sorted)
            std::cout << "SORTED";
        else
            std::cout << "!SORTED";

        std::cout << std::left;
        std::cout << std::setw(headers[2].size() + 1);
        if(results2[i].is_rslt_sorted)
            std::cout << "SORTED";
        else
            std::cout << "!SORTED";

        std::cout.setf(std::ios::right | std::ios::fixed | std::ios::showpoint);
        std::cout << std::setw(headers[3].size()) << std::setprecision(5)
                  << results2[i].timings * 1000 << std::endl;
    }

    // delete arrays
    delete[] arr1;
    delete[] arr2;
}

template <typename F, typename T, typename C>
SortData test_sort_function(F *f, T *data, std::size_t size, C *cmp,
                            std::string name) {
    bool is_ctrl_sorted = true, is_rslt_sorted = false;
    timer::ChronoTimer chrono;

    // verify control data is not sorted
    is_ctrl_sorted = sort::verify(data, size, cmp);

    // start timings
    chrono.start();
    f(data, size, cmp);
    chrono.stop();

    // verify result data is sorted
    is_rslt_sorted = sort::verify(data, size, cmp);

    return SortData(name, is_ctrl_sorted, is_rslt_sorted, chrono.seconds());
}

template <typename F, typename T>
SortData test_sort_function(F *f, T *data, std::size_t size, std::string name) {
    bool is_ctrl_sorted = true, is_rslt_sorted = false;
    timer::ChronoTimer chrono;

    // verify control data is not sorted
    is_ctrl_sorted = sort::verify(data, size);

    // start timings
    chrono.start();
    f(data, size);
    chrono.stop();

    // verify result data is sorted
    is_rslt_sorted = sort::verify(data, size);

    return SortData(name, is_ctrl_sorted, is_rslt_sorted, chrono.seconds());
}
