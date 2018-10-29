/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : sort
 * DESCRIPTION : This program is the main test harness for the various sorting
 *      functions. It includes a simple test to visually print before and after
 *      sorted array. It also has deeper tests to perform large samples to
 *      test the sorting functions. Finally, it bench marks the fast sorting
 *      algorithms and display their times against STL's sort algorithm.
 ******************************************************************************/
#include <algorithm>           // test std::sort()
#include <cstdlib>             // srand(), rand(), atoi()
#include <ctime>               // time()
#include <iomanip>             // setw()
#include <iostream>            // stram objects
#include <thread>              // thread objects
#include "../include/sort.h"   // various sort functions
#include "../include/timer.h"  // ChronoTimer class

struct SortData {  // stores sortedness data
    SortData(std::string n = "", bool c = true, bool r = true, double t = 0,
             double d = 0)
        : is_ctrl_sorted(c),
          is_rslt_sorted(r),
          timings(t),
          stl_diff(d),
          name(n) {}

    SortData &operator+=(const SortData &rhs) {
        this->name = rhs.name;
        this->is_ctrl_sorted &= rhs.is_ctrl_sorted;
        this->is_rslt_sorted &= rhs.is_rslt_sorted;
        this->timings += rhs.timings;
        this->stl_diff += rhs.stl_diff;
        return *this;
    }

    bool is_ctrl_sorted;  // is control sorted?
    bool is_rslt_sorted;  // is result sorted?
    double timings;       // timings
    double stl_diff;      // percent difference to STL sort
    std::string name;     // name of data
};

// main test wrappers
void simple_tests();                      // test sorts and prints
void verify(std::size_t sample = 10);     // call test_sortedness
void benchmark(std::size_t sample = 10);  // call test_timings

// test all sorting functions with sortedness verifications
void test_sortedness(std::size_t array_size = 50000, std::size_t sample = 10);

// test timings the fastest sorting functions without sortedness verifications
void test_timings(std::size_t array_size = 50000, std::size_t sample = 10);

// test sort functions with lambda
template <typename F, typename T, typename C = bool (*)(T const &, T const &)>
SortData test_sort_func1(F *f, T *data, std::size_t size, std::string name,
                         C cmp, bool verify = true);

// test for sort functions WITHOUT lambda
template <typename F, typename T>
SortData test_sort_func2(F *f, T *data, std::size_t size, std::string name,
                         bool cmp = sort::less, bool verify = true);

// test for sort functions and print array elements
template <typename F, typename T>
void test_sorts_and_print(F *f, T *data, std::size_t size,
                          bool cmp = sort::less);

// print results
void print_data(std::string headers[], int h_size, SortData s_data[],
                int data_size, bool verify = true, bool stl_diff = false);

int main(int argc, char *argv[]) {
    int sample_verify = 10, sample_bench = 50;

    // PROCESS ARGUMENT FLAGS
    if(argc == 1) {
        simple_tests();
    } else {
        for(int i = 1; i < argc; ++i) {
            if(std::string(argv[i]) == "-a" ||
               std::string(argv[i]) == "--all") {
                verify(sample_verify);
                benchmark(sample_bench);
            }

            if(std::string(argv[i]) == "-v" ||
               std::string(argv[i]) == "--verify") {
                if(i + 1 < argc) sample_verify = std::atoi(argv[i + 1]);
                verify(sample_verify);
            }

            if(std::string(argv[i]) == "-b" ||
               std::string(argv[i]) == "--benchmark") {
                if(i + 1 < argc) sample_bench = std::atoi(argv[i + 1]);
                benchmark(sample_bench);
            }
        }
    }

    return 0;
}

void simple_tests() {
    const int FUNC_SIZE = 9, SIZE = 4, ARRAY_SZIE_1 = 20, ARRAY_SZIE_2 = 27;
    int *arrays[SIZE];

    std::string names[FUNC_SIZE] = {"BUBBLE", "SELECTION", "INSERTION",
                                    "MERGE",  "QUICK V1",  "QUICK V2",
                                    "HEAP",   "INTRO",     "INTRO V2"};

    // declare function pointer type for sorting normal
    typedef void (*fn_ptr2)(int *data, std::size_t size, bool cmp);

    // array of function pointers for sorting normal
    fn_ptr2 fn_arr2[FUNC_SIZE] = {
        &sort::bubble_sort<int>,    &sort::selection_sort<int>,
        &sort::insertion_sort<int>, &sort::merge_sort<int>,
        &sort::quick_sort<int>,     &sort::quick2_sort<int>,
        &sort::heap_sort<int>,      &sort::intro_sort<int>,
        &sort::intro2_sort<int>};

    arrays[0] = new int[ARRAY_SZIE_1];
    arrays[1] = new int[ARRAY_SZIE_2];
    arrays[2] = new int[ARRAY_SZIE_1];
    arrays[3] = new int[ARRAY_SZIE_2];

    // populate arrays[0] and arrays[1] with random numbers as CONTROL
    for(int i = 0; i < ARRAY_SZIE_1; ++i) arrays[0][i] = rand() % 10;
    for(int i = 0; i < ARRAY_SZIE_2; ++i) arrays[1][i] = rand() % 10;

    for(int i = 0; i < FUNC_SIZE; ++i) {
        // copy control arrays to test arrays[2] and arrays[3]
        sort::copy_array(arrays[2], arrays[0], ARRAY_SZIE_1);
        sort::copy_array(arrays[3], arrays[1], ARRAY_SZIE_2);

        std::cout << std::string(80, '-') << std::endl;
        std::cout << "ALGORITHM: " << names[i] << std::endl;
        std::cout << std::string(80, '-') << std::endl;
        test_sorts_and_print(fn_arr2[i], arrays[2], ARRAY_SZIE_1);
        std::cout << std::endl;
        test_sorts_and_print(fn_arr2[i], arrays[3], ARRAY_SZIE_2);

        if(i != FUNC_SIZE - 1) std::cout << std::endl << std::endl;
    }

    // delete arrays
    for(int i = 0; i < SIZE; ++i) delete[] arrays[i];
}

void verify(std::size_t sample) {
    const int SIZE = 12;
    int ARRAY_SZIEs[SIZE] = {0,  1,  2,     3,     10,    11,
                             50, 51, 10000, 10001, 50000, 50001};

    for(int i = 0; i < SIZE; ++i) {
        std::cout << "ARRAY SIZE: " << std::setw(11) << std::left
                  << ARRAY_SZIEs[i] << "SAMPLE: " << sample << "\n";
        std::cout << std::string(80, '-') << std::endl;
        test_sortedness(ARRAY_SZIEs[i], sample);
        if(i != SIZE - 1) std::cout << std::endl;
    }
}

void benchmark(std::size_t sample) {
    const int SIZE = 5;
    int ARRAY_SZIEs[SIZE] = {10000, 50000, 100000, 1000000, 10000000};

    for(int i = 0; i < SIZE; ++i) {
        std::cout << "ARRAY SIZE: " << std::setw(11) << std::left
                  << ARRAY_SZIEs[i] << "SAMPLE: " << sample << "\n";
        std::cout << std::string(80, '-') << std::endl;
        test_timings(ARRAY_SZIEs[i], sample);
        if(i != SIZE - 1) std::cout << std::endl;
    }
}

void test_sortedness(std::size_t array_size, std::size_t sample) {
    const int FUNC_SIZE = 9, HEADER_SIZE = 4, MAX_THREADS = 4,
              TOTAL_SIZE = FUNC_SIZE * 2;

    // arrays of headers and names
    std::string headers[HEADER_SIZE] = {"    Sorting Algorithm", "CONTROL ",
                                        "RESULT ", "Timings (ms)"};
    std::string names[TOTAL_SIZE] = {
        "Bubble w/lambda",    "Bubble",    "Selection w/lambda", "Selection",
        "Insertion w/lambda", "Insertion", "Merge w/lambda",     "Merge",
        "Quick1 w/lambda",    "Quick1",    "Quick2 w/lambda",    "Quick2",
        "Heap w/lambda",      "Heap",      "Intro w/lambda",     "Intro",
        "Intro2 w/lambda",    "Intro2"};

    // declare function pointer type for sorting with lambda
    typedef void (*fn_ptr1)(int *data, std::size_t size,
                            bool (*cmp)(const int &l, const int &r));

    // declare function pointer type for sorting normal
    typedef void (*fn_ptr2)(int *data, std::size_t size, bool cmp);

    // array of function pointers to sorting with lambda
    fn_ptr1 fn_arr1[FUNC_SIZE] = {
        &sort::bubble_sort<int>,    &sort::selection_sort<int>,
        &sort::insertion_sort<int>, &sort::merge_sort<int>,
        &sort::quick_sort<int>,     &sort::quick2_sort<int>,
        &sort::heap_sort<int>,      &sort::intro_sort<int>,
        &sort::intro2_sort<int>};

    // array of function pointers for sorting normal
    fn_ptr2 fn_arr2[FUNC_SIZE] = {
        &sort::bubble_sort<int>,    &sort::selection_sort<int>,
        &sort::insertion_sort<int>, &sort::merge_sort<int>,
        &sort::quick_sort<int>,     &sort::quick2_sort<int>,
        &sort::heap_sort<int>,      &sort::intro_sort<int>,
        &sort::intro2_sort<int>};

    // data declaration and allocations
    SortData results1[FUNC_SIZE], results2[FUNC_SIZE];
    SortData final_results[TOTAL_SIZE];
    std::thread *threads[MAX_THREADS];
    int *arrays[TOTAL_SIZE];
    int *control = nullptr;
    control = new int[array_size];

    // srand(time(nullptr));

    // allocate new memory for array of arrays
    for(int i = 0; i < TOTAL_SIZE; ++i) arrays[i] = new int[array_size];

    // populate control with random numbers
    for(std::size_t i = 0; i < array_size; ++i) control[i] = rand();

    // testing every sort functions with same copies of arr using arrays[]
    for(std::size_t i = 0; i < sample; ++i) {
        sort::shuffle(control, array_size);  // shuffle control

        // copy control to arrays[] with MAX_THREADS at each iteration
        int threads_repeat = TOTAL_SIZE / MAX_THREADS;
        for(int j = 0; j < threads_repeat; ++j) {
            for(int k = 0; k < MAX_THREADS; ++k)
                threads[k] = new std::thread(sort::copy_array<int>,
                                             arrays[k + j * MAX_THREADS],
                                             control, array_size);

            for(int k = 0; k < MAX_THREADS; ++k) {
                threads[k]->join();
                delete threads[k];
            }
        }

        // copy control to left over arrays[] with x threads under max
        int threads_left_over = TOTAL_SIZE - MAX_THREADS * threads_repeat;
        for(int i = 0; i < threads_left_over; ++i)
            threads[i] = new std::thread(sort::copy_array<int>,
                                         arrays[i + 4 * MAX_THREADS], control,
                                         array_size);
        for(int i = 0; i < threads_left_over; ++i) {
            threads[i]->join();
            delete threads[i];
        }

        // iterate array of function pointeres to test all sort functions
        for(int j = 0; j < FUNC_SIZE; ++j) {
            // iterate array of funct pointeres to sort functions with lambda
            results1[j] += test_sort_func1(
                fn_arr1[j], arrays[j], array_size, names[j * 2],
                [](const auto &a, const auto &b) { return a < b; });
        }

        // iterate array of funct pointeres to sort functions WITHOUT lambda
        for(int j = 0; j < FUNC_SIZE; ++j) {
            // test merge and quick w/o comparison pointers with arr1
            results2[j] +=
                test_sort_func2(fn_arr2[j], arrays[j + 9], array_size,
                                names[j * 2 + 1], sort::less);
        }
    }

    // calculate average for results1
    for(int i = 0; i < FUNC_SIZE; ++i) results1[i].timings /= sample;

    // calculate average for results2
    for(int i = 0; i < FUNC_SIZE; ++i) results2[i].timings /= sample;

    // merge results1 and results2 to final results
    for(int i = 0; i < TOTAL_SIZE; ++i) {
        if(i % 2 == 0)
            final_results[i] = results1[i / 2];
        else
            final_results[i] = results2[i / 2];
    }

    print_data(headers, HEADER_SIZE, final_results, TOTAL_SIZE, true);

    // delete arrays
    delete[] control;
    for(int i = 0; i < TOTAL_SIZE; ++i) delete[] arrays[i];
}

void test_timings(std::size_t array_size, std::size_t sample) {
    const int FUNC_SIZE = 6, HEADER_SIZE = 3, TOTAL_SIZE = FUNC_SIZE * 2 + 2,
              MAX_THREADS = 4;
    timer::ChronoTimer chrono_timer;

    // arrays of headers and names
    std::string headers[HEADER_SIZE] = {"    Sorting Algorithm",
                                        "% diff to STL", "Timings (ms)"};
    std::string names[TOTAL_SIZE] = {
        "Merge w/lambda",      "Merge",    "Quick1 w/lambda", "Quick1",
        "Quick2 w/lambda",     "Quick2",   "Heap w/lambda",   "Heap",
        "Intro w/lambda",      "Intro",    "Intro2 w/lambda", "Intro2",
        "std::sort w/ lambda", "std::sort"};

    // declare function pointer type for sorting with lambda
    typedef void (*fn_ptr1)(int *data, std::size_t size,
                            bool (*cmp)(const int &l, const int &r));

    // declare function pointer type for sorting normal
    typedef void (*fn_ptr2)(int *data, std::size_t size, bool cmp);

    // array of function pointers to sorting with lambda
    fn_ptr1 fn_arr1[FUNC_SIZE] = {
        &sort::merge_sort<int>,  &sort::quick_sort<int>,
        &sort::quick2_sort<int>, &sort::heap_sort<int>,
        &sort::intro_sort<int>,  &sort::intro2_sort<int>};

    // array of function pointers for sorting normal
    fn_ptr2 fn_arr2[FUNC_SIZE] = {
        &sort::merge_sort<int>,  &sort::quick_sort<int>,
        &sort::quick2_sort<int>, &sort::heap_sort<int>,
        &sort::intro_sort<int>,  &sort::intro2_sort<int>};

    // data declaration and allocations
    SortData results1[FUNC_SIZE], results2[FUNC_SIZE], std_results[2];
    SortData final_results[TOTAL_SIZE];  // add 2 for two std::sort()
    std::thread *threads[MAX_THREADS];
    int *arrays[TOTAL_SIZE];
    int *control = nullptr;
    control = new int[array_size];

    // srand(time(nullptr));

    // allocate new memory for array of arrays
    for(int i = 0; i < TOTAL_SIZE; ++i) arrays[i] = new int[array_size];

    // populate control with random numbers
    for(std::size_t i = 0; i < array_size; ++i) control[i] = rand();

    // testing every sort functions with same copies of arr using arrays[]
    for(std::size_t i = 0; i < sample; ++i) {
        sort::shuffle(control, array_size);  // shuffle control

        // copy control to arrays[] with MAX_THREADS at each iteration
        int threads_repeat = TOTAL_SIZE / MAX_THREADS;
        for(int j = 0; j < threads_repeat; ++j) {
            for(int k = 0; k < MAX_THREADS; ++k)
                threads[k] = new std::thread(sort::copy_array<int>,
                                             arrays[k + j * MAX_THREADS],
                                             control, array_size);

            for(int k = 0; k < MAX_THREADS; ++k) {
                threads[k]->join();
                delete threads[k];
            }
        }

        // copy control to left over arrays[] with x threads under max
        int threads_left_over = TOTAL_SIZE - MAX_THREADS * threads_repeat;
        for(int i = 0; i < threads_left_over; ++i)
            threads[i] = new std::thread(sort::copy_array<int>,
                                         arrays[i + 3 * MAX_THREADS], control,
                                         array_size);
        for(int i = 0; i < threads_left_over; ++i) {
            threads[i]->join();
            delete threads[i];
        }

        // iterate array of funct pointeres to sort functions with lambda
        for(int j = 0; j < FUNC_SIZE; ++j) {
            // test sorting functions with lambda
            results1[j] += test_sort_func1(
                fn_arr1[j], arrays[j], array_size, names[j * 2],
                [](const auto &a, const auto &b) { return a < b; }, false);
        }

        // iterate array of funct pointeres to sort functions WITHOUT lambda
        for(int j = 0; j < FUNC_SIZE; ++j) {
            // test merge and quick w/o comparison pointers with arr1
            results2[j] +=
                test_sort_func2(fn_arr2[j], arrays[j + 6], array_size,
                                names[j * 2 + 1], sort::less, false);
        }

        // time std::sort() with lambda
        chrono_timer.start();
        std::sort(arrays[FUNC_SIZE * 2], arrays[FUNC_SIZE * 2] + array_size,
                  [](const auto &a, const auto &b) { return a < b; });
        chrono_timer.stop();
        std_results[0].name = names[FUNC_SIZE * 2];
        std_results[0].timings += chrono_timer.seconds();

        // time std::sort() WITHOUT lambda
        chrono_timer.start();
        std::sort(arrays[FUNC_SIZE * 2 + 1],
                  arrays[FUNC_SIZE * 2 + 1] + array_size);
        chrono_timer.stop();
        std_results[1].name = names[FUNC_SIZE * 2 + 1];
        std_results[1].timings += chrono_timer.seconds();
    }

    // calculate average for std_results --> std::sort()
    for(int i = 0; i < 2; ++i) std_results[i].timings /= sample;

    // find % diff of std::sort and std::sort with lambda
    std_results[0].stl_diff =
        (std_results[0].timings - std_results[1].timings) /
        std_results[1].timings * 100;

    // calculate average for results1 and find % diff to std::sort
    for(int i = 0; i < FUNC_SIZE; ++i) {
        results1[i].timings /= sample;
        results1[i].stl_diff = (results1[i].timings - std_results[1].timings) /
                               std_results[1].timings * 100;
    }

    // calculate average for results2 and find % diff to std::sort
    for(int i = 0; i < FUNC_SIZE; ++i) {
        results2[i].timings /= sample;
        results2[i].stl_diff = (results2[i].timings - std_results[1].timings) /
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

    print_data(headers, HEADER_SIZE, final_results, TOTAL_SIZE, false, true);

    // delete arrays
    delete[] control;
    for(int i = 0; i < TOTAL_SIZE; ++i) delete[] arrays[i];
}

template <typename F, typename T, typename C>
SortData test_sort_func1(F *f, T *data, std::size_t size, std::string name,
                         C cmp, bool verify) {
    bool is_ctrl_sorted = true, is_rslt_sorted = false;
    timer::ChronoTimer chrono;

    // verify control data is not sorted
    if(verify) is_ctrl_sorted = sort::verify<T>(data, size, cmp);

    // start timings
    chrono.start();
    f(data, size, cmp);
    chrono.stop();

    // verify result data is sorted
    if(verify) is_rslt_sorted = sort::verify<T>(data, size, cmp);

    return SortData(name, is_ctrl_sorted, is_rslt_sorted, chrono.seconds());
}

template <typename F, typename T>
SortData test_sort_func2(F *f, T *data, std::size_t size, std::string name,
                         bool cmp, bool verify) {
    bool is_ctrl_sorted = true, is_rslt_sorted = false;
    timer::ChronoTimer chrono;

    // verify control data is not sorted
    if(verify) is_ctrl_sorted = sort::verify<T>(data, size);

    // start timings
    chrono.start();
    f(data, size, cmp);
    chrono.stop();

    // verify result data is sorted
    if(verify) is_rslt_sorted = sort::verify<T>(data, size);

    return SortData(name, is_ctrl_sorted, is_rslt_sorted, chrono.seconds());
}

template <typename F, typename T>
void test_sorts_and_print(F *f, T *data, std::size_t size, bool cmp) {
    std::cout << "ARRAY SIZE: " << size << std::endl;
    std::cout << "\nCONTROL: ";
    sort::print_array<T>(data, size);
    f(data, size, cmp);
    std::cout << "\nRESULTS: ";
    sort::print_array<T>(data, size);
}

void print_data(std::string headers[], int h_size, SortData s_data[],
                int data_size, bool verify, bool stl_diff) {
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

        if(stl_diff) {
            std::cout.setf(std::ios::right | std::ios::fixed |
                           std::ios::showpoint);
            std::cout << std::setw(headers[h_size - 2].size() - 2)
                      << std::setprecision(2) << s_data[i].stl_diff << " % ";
        }

        std::cout.setf(std::ios::right | std::ios::fixed | std::ios::showpoint);
        std::cout << std::setw(headers[h_size - 1].size())
                  << std::setprecision(5) << s_data[i].timings * 1000
                  << std::endl;
    }
}
