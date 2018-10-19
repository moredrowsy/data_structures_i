#include <cstdlib>                         // rand(), srand()
#include <ctime>                           // time()
#include <iomanip>                         // setw()
#include <iostream>                        // stream objects
#include <string>                          // string objects
#include <vector>                          // vector objects
#include "../include/chained_avl_hash.h"   // BST AVL class
#include "../include/chained_list_hash.h"  // List class
#include "../include/double_hash.h"        // DoubleHash class
#include "../include/open_hash.h"          // OpenHash Class
#include "../include/record.h"             // Record class

using Record = record::Record<int>;
using OpenHash = open_hash::OpenHash<Record>;
using DoubleHash = double_hash::DoubleHash<Record>;
using ChainedAVLHash = chained_avl_hash::ChainedAVLHash<Record>;
using ChainedListHash = chained_list_hash::ChainedListHash<Record>;

// templated interactive hash table tests; takes in Hash type object pointer
template <typename O>
void test_hash_table_interactive(O* obj, std::string name);

// templated random tests for array of hash type objects
template <typename O, typename L>
double* test_random_hash(O* obj, const L* list, unsigned sample_size,
                         int iterations);

// test collisions, insert and search times for open, double, chained hash
void test_timing_hashes(unsigned table_size = 811, unsigned iterations = 700,
                        unsigned sample_size = 1000);

int main(int argc, char* argv[]) {
    const int BOOL_SIZE = 5;
    bool RANDOM = false;
    bool INTERACTIVE_OPEN = false;
    bool INTERACTIVE_DOUBLE = false;
    bool INTERACTIVE_CHAINED_AVL = false;
    bool INTERACTIVE_CHAINED_LIST = false;
    bool* options[] = {&RANDOM, &INTERACTIVE_OPEN, &INTERACTIVE_DOUBLE,
                       &INTERACTIVE_CHAINED_AVL, &INTERACTIVE_CHAINED_LIST};

    // PROCESS ARGUMENT FLAGS
    if(argc == 1) RANDOM = true;
    for(int i = 1; i < argc; ++i) {
        if(std::string(argv[i]) == "-all")
            for(int i = 0; i < BOOL_SIZE; ++i) *options[i] = true;

        if(std::string(argv[i]) == "-interactive-all")
            for(int i = 1; i < BOOL_SIZE; ++i) *options[i] = true;

        if(std::string(argv[i]) == "-random") RANDOM = true;

        if(std::string(argv[i]) == "-open") INTERACTIVE_OPEN = true;

        if(std::string(argv[i]) == "-double") INTERACTIVE_DOUBLE = true;

        if(std::string(argv[i]) == "-chained-avl")
            INTERACTIVE_CHAINED_AVL = true;

        if(std::string(argv[i]) == "-chained-list")
            INTERACTIVE_CHAINED_LIST = true;
    }

    OpenHash* open = nullptr;
    DoubleHash* doubleh = nullptr;
    ChainedAVLHash* chained_AVL = nullptr;
    ChainedListHash* chained_list = nullptr;

    if(RANDOM) {
        std::cout << std::endl;
        test_timing_hashes();
    }

    if(INTERACTIVE_OPEN) {
        std::cout << std::endl;
        open = new OpenHash(17);
        test_hash_table_interactive(open, "open_hash_table");
    }

    if(INTERACTIVE_DOUBLE) {
        std::cout << std::endl;
        doubleh = new DoubleHash(17);
        test_hash_table_interactive(doubleh, "double_hash_table");
    }

    if(INTERACTIVE_CHAINED_AVL) {
        std::cout << std::endl;
        chained_AVL = new ChainedAVLHash(17);
        test_hash_table_interactive(chained_AVL, "chained_avl_hash_table");
    }

    if(INTERACTIVE_CHAINED_LIST) {
        std::cout << std::endl;
        chained_list = new ChainedListHash(17);
        test_hash_table_interactive(chained_list, "chained_list_hash_table");
    }

    delete open;
    delete doubleh;
    delete chained_AVL;
    delete chained_list;

    return 0;
}

template <typename O, typename L>
double* test_random_hash(O* obj, const L* list, unsigned sample_size,
                         int iterations) {
    using Record = record::Record<int>;

    bool is_found = false;
    const unsigned SEARCH_SIZE = iterations / 2;
    clock_t clicks = 0, insert = 0, valid = 0, invalid = 0;
    double collisions = 0, insert_time = 0, valid_time = 0, valid_found = 0,
           invalid_time = 0, invalid_found = 0;
    Record result;

    srand(time(nullptr));

    // inserts
    for(unsigned i = 0; i < sample_size; ++i) {
        clicks = std::clock();
        for(auto& a : list[i]) obj[i].insert(a);
        insert += std::clock() - clicks;

        assert(obj[i].size() != 0);

        collisions += obj[i].collisions();
    }
    insert_time = (double)insert / sample_size / CLOCKS_PER_SEC;
    collisions /= sample_size;

    // search valid
    for(unsigned i = 0; i < sample_size; ++i) {
        // ensure that all searches are TRUE
        for(unsigned j = 0; j < SEARCH_SIZE; ++j) {
            is_found = obj[i].find(list[i].at(j)._key, result);
            if(is_found) valid_found += 1;
        }

        // start timing inserts
        clicks = std::clock();
        for(unsigned j = 0; j < SEARCH_SIZE; ++j)
            obj[i].find(list[i].at(j)._key, result);
        valid += std::clock() - clicks;
    }
    valid_found /= sample_size;
    valid_time = (double)valid / sample_size / CLOCKS_PER_SEC;

    // search invalid
    for(unsigned i = 0; i < sample_size; ++i) {
        // ensure that all searches are FALSE
        for(int j = -100; j > -350; --j) {
            is_found = obj[i].find(j, result);
            if(is_found) invalid_found += 1;
        }

        // start timing inserts
        clicks = std::clock();
        for(int j = -100; j > -350; --j) obj[i].find(j, result);
        invalid += std::clock() - clicks;
    }
    invalid_found /= sample_size;
    invalid_time = (double)invalid / sample_size / CLOCKS_PER_SEC;

    return new double[6]{collisions,  insert_time,  valid_time,
                         valid_found, invalid_time, invalid_found};
}

void test_timing_hashes(unsigned table_size, unsigned iterations,
                        unsigned sample_size) {
    assert(table_size >= iterations);
    assert(sample_size > 0);

    // CONSTANTS
    const unsigned MAX_RANGE = 10001, SEARCH_SIZE = iterations / 2,
                   HEAD_SIZE = 7, DATA_SIZE = 4, PREC = 8;

    // LISTS
    OpenHash* ohash = new OpenHash[sample_size];
    DoubleHash* dhash = new DoubleHash[sample_size];
    ChainedAVLHash* avl_hash = new ChainedAVLHash[sample_size];
    ChainedListHash* cl_hash = new ChainedListHash[sample_size];
    std::vector<Record>* list = new std::vector<Record>[sample_size];
    double* data[DATA_SIZE];

    srand(time(nullptr));

    // change array's hash's table size if not default
    if(table_size != 811) {
        for(unsigned i = 0; i < sample_size; ++i) {
            ohash[i] = OpenHash(table_size);
            dhash[i] = DoubleHash(table_size);
            avl_hash[i] = ChainedAVLHash(table_size);
            cl_hash[i] = ChainedListHash(table_size);
        }
    }

    // add random values to array of vector of Records
    for(unsigned i = 0; i < sample_size; ++i) {
        for(unsigned j = 0; j < iterations; ++j)
            list[i].push_back(Record(rand() % MAX_RANGE, rand() % MAX_RANGE));

        assert(list[i].size() == iterations);
    }

    // CALL RANDOM TESTS TO POPULATE DATA
    data[0] = test_random_hash(ohash, list, sample_size, iterations);
    data[1] = test_random_hash(dhash, list, sample_size, iterations);
    data[2] = test_random_hash(avl_hash, list, sample_size, iterations);
    data[3] = test_random_hash(cl_hash, list, sample_size, iterations);

    // CREATE DATA ARRAYS FOR OUTPUT
    std::string name_type = "TYPE";
    std::string names[DATA_SIZE] = {"OpenHash", "DoubleHash", "ChainedAVLHash",
                                    "ChainedListHash"};
    std::string header[HEAD_SIZE] = {
        name_type + std::string(names[3].size() - name_type.size(), ' '),
        "COLLISIONS",
        "INSERTS (t)",
        "VALIDS (t)",
        "FOUND",
        "INVALIDS (t)",
        "NOT FOUND"};

    // TITLE INFO OUTPUT
    std::cout << std::string(80, '-') << std::endl
              << "COLLISTION AND TIMING TESTS" << std::endl
              << std::string(80, '-') << std::endl
              << std::setw(11) << "sample size"
              << " : " << sample_size << std::endl
              << std::setw(11) << "table size"
              << " : " << table_size << std::endl
              << std::setw(11) << "iterations"
              << " : " << iterations << std::endl
              << std::setw(11) << "search size"
              << " : " << SEARCH_SIZE << std::endl
              << std::setw(11) << "t"
              << " : "
              << "clock ticks per second" << std::endl
              << std::endl
              << "* results are averaged by sample size" << std::endl
              << std::endl;

    // HEADER LABELS 1
    std::cout << std::setw(header[0].size() + 1) << ""  //
              << std::setw(header[1].size() + 1) << ""
              << std::setw(header[2].size() + 1) << ""
              << std::setw(header[3].size() + 1) << std::left << "SEARCH"
              << std::setw(header[4].size() + 1) << ""
              << std::setw(header[5].size() + 1) << std::left << "SEARCH"
              << std::setw(header[6].size() + 1) << "" << std::endl;

    // HEADER LABELS 2
    for(unsigned i = 0; i < HEAD_SIZE; ++i) {
        std::cout << std::setw(header[i].size() + 1) << header[i];
    }
    std::cout << std::endl;

    // HORIZONTAL BARS
    for(unsigned i = 0; i < HEAD_SIZE; ++i) {
        std::cout << std::setw(header[i].size() + 1)
                  << std::string(header[i].size(), '-');
    }
    std::cout << std::endl;

    // PRINT ALL HASH DATA
    for(unsigned i = 0; i < DATA_SIZE; ++i) {  // CHAINED LIST HASH DATA
        std::cout << std::setw(header[0].size() + 1) << names[i];
        for(unsigned j = 0; j < HEAD_SIZE - 1; ++j) {
            if(j == 1 || j == 2 || j == 4)
                std::cout << std::setprecision(PREC) << std::fixed;
            else
                std::cout << std::resetiosflags(std::ios::showbase)
                          << std::setprecision(0) << std::fixed;
            std::cout << std::setw(header[j + 1].size() + 1) << data[i][j];
        }
        std::cout << std::endl;
    }

    // DELETE ALL DATA
    for(unsigned i = 0; i < 4; ++i) delete[] data[i];
    delete[] ohash;
    delete[] dhash;
    delete[] avl_hash;
    delete[] cl_hash;
    delete[] list;
}

template <typename O>
void test_hash_table_interactive(O* obj, std::string name) {
    using Record = record::Record<int>;

    std::cout << std::string(80, '-') << std::endl
              << "INTERACTIVE TESTS: " << name << std::endl
              << std::string(80, '-') << std::endl;

    const unsigned MAX_RANGE = 10001;
    bool is_inserted = false, is_found = false, is_removed = false;
    char c;
    int key = 0, value = 0;
    Record record;
    srand(time(nullptr));

    do {
        std::cout << "[S]ize()  [R]andom  [I]nsert  [D]elete  [F]ind  [C]lear"
                     "     e[X]it: ";
        std::cin >> c;

        switch(c) {
            case 'X':
            case 'x':
                return;  // exit function
            case 'C':
            case 'c':
                obj->clear();
                std::cout << ">> Clear: " << std::endl << *obj << std::endl;

                break;
            case 'D':
            case 'd':
                std::cin >> key;
                is_removed = obj->remove(key);

                std::cout << ">> Deletion: " << key;

                if(is_removed)
                    std::cout << " removed";
                else
                    std::cout << " does not exist";

                std::cout << std::endl << *obj << std::endl;

                break;
            case 'F':
            case 'f':
                std::cin >> key;
                record = Record(key);
                is_found = obj->find(key, record);

                std::cout << ">> Find ";

                if(is_found)
                    std::cout << "success: " << record;
                else
                    std::cout << "failed: " << key << " does not exist";

                std::cout << std::endl << *obj << std::endl;

                break;
            case 'I':
            case 'i':
                std::cin >> key >> value;
                record = Record(key, value);
                is_inserted = obj->insert(record);

                std::cout << ">> Insert ";

                if(is_inserted)
                    std::cout << ": " << record;
                else
                    std::cout << "failed: full table";

                std::cout << std::endl << *obj << std::endl;

                break;
            case 'R':
            case 'r':
                record = Record(rand() % MAX_RANGE, rand() % MAX_RANGE);
                is_inserted = obj->insert(record);

                std::cout << ">> Random insert ";

                if(is_inserted)
                    std::cout << ": " << record;
                else
                    std::cout << "failed: full table";

                std::cout << std::endl << *obj << std::endl;

                break;
            case 'S':
            case 's':
                std::cout << ">> Size: " << obj->size() << std::endl
                          << std::endl;
                break;
            default:
                std::cout << "Invalid Choice" << std::endl << std::endl;
        }
    } while(c != 'X' && c != 'x');
}
