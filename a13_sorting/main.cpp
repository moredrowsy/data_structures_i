#include <algorithm>           // test std::sort()
#include <cstdlib>             // srand(), rand(), atoi()
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
    double std_diff;      // percent difference to STL sort
    std::string name;     // name of data
};

// main test wrappers
void verify(std::size_t iterations = 10);     // call test_sortedness
void benchmark(std::size_t iterations = 10);  // call test_timings

// test all sorting functions with sortedness verifications
void test_sortedness(std::size_t sample_size = 50000,
                     std::size_t iterations = 10);

// test timings the fastest sorting functions without sortedness verifications
void test_timings(std::size_t sample_size = 50000, std::size_t iterations = 10);

// test sort functions with lambda
template <typename F, typename T, typename C = bool (*)(T const &, T const &)>
SortData test_sort_func1(F *f, T *data, std::size_t size, std::string name,
                         C cmp, bool verify = true);

// test for sort functions WITHOUT lambda
template <typename F, typename T>
SortData test_sort_func2(F *f, T *data, std::size_t size, std::string name,
                         int cmp = sort::less, bool verify = true);

// print results
void print_data(std::string headers[], int h_size, SortData s_data[],
                int data_size, bool verify = true, bool std_diff = false);

int main(int argc, char *argv[]) {
    int iterations_verify = 10, iterations_timings = 50;

    // PROCESS ARGUMENT FLAGS
    if(argc == 1) {
        verify(iterations_verify);
        benchmark(iterations_timings);
    } else {
        for(int i = 1; i < argc; ++i) {
            if(std::string(argv[i]) == "-a" ||
               std::string(argv[i]) == "--all") {
                verify(iterations_verify);
                benchmark(iterations_timings);
            }

            if(std::string(argv[i]) == "-v" ||
               std::string(argv[i]) == "--verify") {
                if(i + 1 < argc) iterations_verify = std::atoi(argv[i + 1]);
                verify(iterations_verify);
            }

            if(std::string(argv[i]) == "-b" ||
               std::string(argv[i]) == "--benchmark") {
                if(i + 1 < argc) iterations_timings = std::atoi(argv[i + 1]);
                benchmark(iterations_timings);
            }
        }
    }

    return 0;
}

void verify(std::size_t iterations) {
    const int SIZE = 13;
    int sample_sizes[SIZE] = {0,   1,   2,   10,   11,    50,   51,
                              100, 200, 500, 1000, 10000, 50000};

    for(int i = 0; i < SIZE; ++i) {
        std::cout << "Sample size: " << std::setw(10) << std::left
                  << sample_sizes[i] << "Iterations: " << iterations << "\n";
        std::cout << std::string(80, '-') << std::endl;
        test_sortedness(sample_sizes[i], iterations);
        std::cout << std::endl;
    }
}

void benchmark(std::size_t iterations) {
    const int SIZE = 4;
    int sample_sizes[SIZE] = {10000, 50000, 100000, 1000000};

    for(int i = 2; i < SIZE; ++i) {
        std::cout << "Sample size: " << std::setw(10) << std::left
                  << sample_sizes[i] << "Iterations: " << iterations << "\n";
        std::cout << std::string(80, '-') << std::endl;
        test_timings(sample_sizes[i], iterations);
        std::cout << std::endl;
    }
}

void test_sortedness(std::size_t sample_size, std::size_t iterations) {
    const int FUNC_SIZE = 8, HEADER_SIZE = 4;

    // arrays of headers and names
    std::string headers[HEADER_SIZE] = {"    Sorting Algorithm", "CONTROL ",
                                        "RESULT ", "  Timings (ms)"};
    std::string names[FUNC_SIZE * 2] = {
        "Bubble w/lambda",    "Bubble",    "Selection w/lambda", "Selection",
        "Insertion w/lambda", "Insertion", "Merge w/lambda",     "Merge",
        "Quick_v1  w/lambda", "Quick_v1",  "Quick_v2 w/lambda",  "Quick_v2",
        "Heap w/lambda",      "Heap",      "Intro w/lambda",     "Intro"};

    // declare function pointer type for sorting with lambda
    typedef void (*fn_ptr1)(int *data, std::size_t size,
                            bool (*cmp)(const int &l, const int &r));

    // declare function pointer type for sorting normal
    typedef void (*fn_ptr2)(int *data, std::size_t size, int cmp);

    // array of function pointers to sorting with lambda
    fn_ptr1 fn_arr1[FUNC_SIZE] = {
        &sort::bubble_sort<int>,    &sort::selection_sort<int>,
        &sort::insertion_sort<int>, &sort::merge_sort<int>,
        &sort::quick_sort<int>,     &sort::quick_sort2<int>,
        &sort::heap_sort<int>,      &sort::intro_sort<int>};

    // array of function pointers for sorting normal
    fn_ptr2 fn_arr2[FUNC_SIZE] = {
        &sort::bubble_sort<int>,    &sort::selection_sort<int>,
        &sort::insertion_sort<int>, &sort::merge_sort<int>,
        &sort::quick_sort<int>,     &sort::quick_sort2<int>,
        &sort::heap_sort<int>,      &sort::intro_sort<int>};

    // data declaration and allocations
    SortData results1[FUNC_SIZE], results2[FUNC_SIZE];
    SortData final_results[FUNC_SIZE * 2];
    int *arr1 = nullptr, *arr2 = nullptr;
    arr1 = new int[sample_size];
    arr2 = new int[sample_size];

    // populate array 2
    for(std::size_t i = 0; i < sample_size; ++i) arr2[i] = rand();

    // array 1 will be copied from array 2 at every iterations
    for(std::size_t i = 0; i < iterations; ++i) {
        sort::shuffle(arr2, sample_size);  // shuffle array 2

        // iterate array of function pointeres to test all sort functions
        for(int j = 0; j < FUNC_SIZE; ++j) {
            SortData result;
            sort::copy_array(arr1, arr2, sample_size);  // restore arr1 w/ arr2

            // iterate array of funct pointeres to sort functions with lambda
            result = test_sort_func1(
                fn_arr1[j], arr1, sample_size, names[j * 2],
                [](const auto &a, const auto &b) { return a < b; });

            results1[j].name = result.name;
            results1[j].is_ctrl_sorted &= result.is_ctrl_sorted;  // control
            results1[j].is_rslt_sorted &= result.is_rslt_sorted;  // results
            results1[j].timings += result.timings;  // results timing
        }

        // iterate array of funct pointeres to sort functions WITHOUT lambda
        for(int j = 0; j < FUNC_SIZE; ++j) {
            SortData result;
            sort::copy_array(arr1, arr2, sample_size);  // restore arr1 w/ arr2

            // test merge and quick w/o comparison pointers with arr1
            result = test_sort_func2(fn_arr2[j], arr1, sample_size,
                                     names[j * 2 + 1], sort::less);

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

    // merge results1 and results2 to final results
    for(int i = 0; i < FUNC_SIZE * 2; ++i) {
        if(i % 2 == 0)
            final_results[i] = results1[i / 2];
        else
            final_results[i] = results2[i / 2];
    }

    print_data(headers, HEADER_SIZE, final_results, FUNC_SIZE * 2, true);

    // delete arrays
    delete[] arr1;
    delete[] arr2;
}

void test_timings(std::size_t sample_size, std::size_t iterations) {
    const int FUNC_SIZE = 5, HEADER_SIZE = 3;
    timer::ChronoTimer chrono_timer;

    // arrays of headers and names
    std::string headers[HEADER_SIZE] = {"    Sorting Algorithm",
                                        "% diff to STL", "Timings (ms)"};
    std::string names[FUNC_SIZE * 2 + 2] = {
        "Merge w/lambda",    "Merge",    "Quick_v1 w/lambda",   "Quick_v1",
        "Quick_v2 w/lambda", "Quick_v2", "Heap w/lambda",       "Heap",
        "Intro w/lambda",    "Intro",    "std::sort w/ lambda", "std::sort"};

    // declare function pointer type for sorting with lambda
    typedef void (*fn_ptr1)(int *data, std::size_t size,
                            bool (*cmp)(const int &l, const int &r));

    // declare function pointer type for sorting normal
    typedef void (*fn_ptr2)(int *data, std::size_t size, int cmp);

    // array of function pointers to sorting with lambda
    fn_ptr1 fn_arr1[FUNC_SIZE] = {
        &sort::merge_sort<int>, &sort::quick_sort<int>, &sort::quick_sort2<int>,
        &sort::heap_sort<int>, &sort::intro_sort<int>};

    // array of function pointers for sorting normal
    fn_ptr2 fn_arr2[FUNC_SIZE] = {
        &sort::merge_sort<int>, &sort::quick_sort<int>, &sort::quick_sort2<int>,
        &sort::heap_sort<int>, &sort::intro_sort<int>};

    // data declaration and allocations
    SortData results1[FUNC_SIZE], results2[FUNC_SIZE], std_results[2];
    SortData final_results[FUNC_SIZE * 2 + 2];
    int *arr1 = nullptr, *arr2 = nullptr;
    arr1 = new int[sample_size];
    arr2 = new int[sample_size];

    // populate array 2
    for(std::size_t i = 0; i < sample_size; ++i) arr2[i] = rand();

    // array 1 will be copied from array 2 at every iterations
    for(std::size_t i = 0; i < iterations; ++i) {
        sort::shuffle(arr2, sample_size);  // shuffle array 2

        // iterate array of funct pointeres to sort functions with lambda
        for(int j = 0; j < FUNC_SIZE; ++j) {
            SortData result;
            sort::copy_array(arr1, arr2, sample_size);  // restore arr1 w/ arr2

            // test sorting functions with lambda
            result = test_sort_func1(
                fn_arr1[j], arr1, sample_size, names[j * 2],
                [](const auto &a, const auto &b) { return a < b; }, false);

            results1[j].name = result.name;
            results1[j].timings += result.timings;  // results timing
        }

        // iterate array of funct pointeres to sort functions WITHOUT lambda
        for(int j = 0; j < FUNC_SIZE; ++j) {
            SortData result;
            sort::copy_array(arr1, arr2, sample_size);  // restore arr1 w/ arr2

            // test merge and quick w/o comparison pointers with arr1
            result = test_sort_func2(fn_arr2[j], arr1, sample_size,
                                     names[j * 2 + 1], sort::less, false);

            results2[j].name = result.name;
            results2[j].timings += result.timings;
        }

        sort::copy_array(arr1, arr2, sample_size);  // restore arr1 w/ arr2

        // time std::sort() with lambda
        chrono_timer.start();
        std::sort(arr1, arr1 + sample_size,
                  [](const auto &a, const auto &b) { return a < b; });
        chrono_timer.stop();
        std_results[0].name = names[FUNC_SIZE * 2];
        std_results[0].timings += chrono_timer.seconds();

        sort::copy_array(arr1, arr2, sample_size);  // restore arr1 w/ arr2

        // time std::sort() WITHOUT lambda
        chrono_timer.start();
        std::sort(arr1, arr1 + sample_size);
        chrono_timer.stop();
        std_results[1].name = names[FUNC_SIZE * 2 + 1];
        std_results[1].timings += chrono_timer.seconds();
    }

    // calculate average for std_results --> std::sort()
    for(int i = 0; i < 2; ++i) std_results[i].timings /= iterations;

    // find % diff of std::sort and std::sort with lambda
    std_results[0].std_diff =
        (std_results[0].timings - std_results[1].timings) /
        std_results[1].timings * 100;

    // calculate average for results1 and find % diff to std::sort
    for(int i = 0; i < FUNC_SIZE; ++i) {
        results1[i].timings /= iterations;
        results1[i].std_diff = (results1[i].timings - std_results[1].timings) /
                               std_results[1].timings * 100;
    }

    // calculate average for results2 and find % diff to std::sort
    for(int i = 0; i < FUNC_SIZE; ++i) {
        results2[i].timings /= iterations;
        results2[i].std_diff = (results2[i].timings - std_results[1].timings) /
                               std_results[1].timings * 100;
    }

    // merge results1 and results2 to final results
    for(int i = 0; i < FUNC_SIZE * 2; ++i) {
        if(i % 2 == 0)
            final_results[i] = results1[i / 2];
        else
            final_results[i] = results2[i / 2];
    }

    // merge std::sort results to final results
    for(int i = 0; i < 2; ++i)
        final_results[i + FUNC_SIZE * 2] = std_results[i];

    print_data(headers, HEADER_SIZE, final_results, FUNC_SIZE * 2 + 2, false,
               true);

    // delete arrays
    delete[] arr1;
    delete[] arr2;
}

template <typename F, typename T, typename C = bool (*)(T const &, T const &)>
SortData test_sort_func1(F *f, T *data, std::size_t size, std::string name,
                         C cmp, bool verify) {
    bool is_ctrl_sorted = true, is_rslt_sorted = false;
    timer::ChronoTimer chrono;

    // verify control data is not sorted
    if(verify) is_ctrl_sorted = sort::verify(data, size, cmp);

    // start timings
    chrono.start();
    f(data, size, cmp);
    chrono.stop();

    // verify result data is sorted
    if(verify) is_rslt_sorted = sort::verify(data, size, cmp);

    return SortData(name, is_ctrl_sorted, is_rslt_sorted, chrono.seconds());
}

template <typename F, typename T>
SortData test_sort_func2(F *f, T *data, std::size_t size, std::string name,
                         int cmp, bool verify) {
    bool is_ctrl_sorted = true, is_rslt_sorted = false;
    timer::ChronoTimer chrono;

    // verify control data is not sorted
    if(verify) is_ctrl_sorted = sort::verify(data, size);

    // start timings
    chrono.start();
    f(data, size, cmp);
    chrono.stop();

    // verify result data is sorted
    if(verify) is_rslt_sorted = sort::verify(data, size);

    return SortData(name, is_ctrl_sorted, is_rslt_sorted, chrono.seconds());
}

void print_data(std::string headers[], int h_size, SortData s_data[],
                int data_size, bool verify, bool std_diff) {
    // print headers
    for(int i = 0; i < h_size; ++i) {
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
    for(int i = 0; i < h_size; ++i) {
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
    for(int i = 0; i < data_size; ++i) {
        std::cout << std::right;
        std::cout << std::setw(headers[0].size()) << s_data[i].name << "  ";

        if(verify) {
            std::cout << std::left;
            std::cout << std::setw(headers[1].size() + 1);
            if(s_data[i].is_ctrl_sorted)
                std::cout << "SORTED";
            else
                std::cout << "!SORTED";

            std::cout << std::left;
            std::cout << std::setw(headers[2].size() + 1);
            if(s_data[i].is_rslt_sorted)
                std::cout << "SORTED";
            else
                std::cout << "!SORTED";
        }

        if(std_diff) {
            std::cout.setf(std::ios::right | std::ios::fixed |
                           std::ios::showpoint);
            std::cout << std::setw(headers[h_size - 2].size() - 2)
                      << std::setprecision(2) << s_data[i].std_diff << " % ";
        }

        std::cout.setf(std::ios::right | std::ios::fixed | std::ios::showpoint);
        std::cout << std::setw(headers[h_size - 1].size())
                  << std::setprecision(5) << s_data[i].timings * 1000
                  << std::endl;
    }
}
