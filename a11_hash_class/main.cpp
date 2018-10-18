#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include "../include/chained_hash.h"
#include "../include/double_hash.h"
#include "../include/open_hash.h"
#include "../include/record.h"

using namespace record;
using namespace open_hash;
using namespace double_hash;
using namespace chained_hash;

// templated interactive hash table tests; takes in Hash type object pointer
template <typename O>
void test_hash_table_interactive(O* obj, std::string name);

// templated random tests for array of hash type objects
template <typename O, typename L>
void test_random_hash(O* obj, const L& list, unsigned sample_size,
                      int iterations, double& collisions, double& insert_time,
                      double& valid_time, double& valid_found,
                      double& invalid_time, double& invalid_found);

void test_collisions(unsigned iterations = 700);

int main() {
    using Record = record::Record<int>;
    using OpenHash = open_hash::OpenHash<Record>;
    using DoubleHash = double_hash::DoubleHash<Record>;
    using ChainedHash = chained_hash::ChainedHash<Record>;

    const bool RANDOM = true;
    const bool INTERACTIVE_OPEN = true;
    const bool INTERACTIVE_DOUBLE = true;
    const bool INTERACTIVE_CHAINED = true;

    OpenHash* open = nullptr;
    DoubleHash* doubleh = nullptr;
    ChainedHash* chained = nullptr;

    if(RANDOM) {
        test_collisions();
        std::cout << std::endl << std::endl;
    }

    if(INTERACTIVE_OPEN) {
        open = new OpenHash(17);
        test_hash_table_interactive(open, "open_hash_table");
        std::cout << std::endl << std::endl;
    }

    if(INTERACTIVE_DOUBLE) {
        doubleh = new DoubleHash(17);
        test_hash_table_interactive(doubleh, "double_hash_table");
        std::cout << std::endl << std::endl;
    }

    if(INTERACTIVE_CHAINED) {
        chained = new ChainedHash(17);
        test_hash_table_interactive(chained, "chained_hash_table");
    }

    delete open;
    delete doubleh;
    delete chained;

    return 0;
}

template <typename O, typename L>
void test_random_hash(O* obj, const L& list, unsigned sample_size,
                      int iterations, double& collisions, double& insert_time,
                      double& valid_time, double& valid_found,
                      double& invalid_time, double& invalid_found) {
    using Record = record::Record<int>;

    bool is_found = false;
    const unsigned SEARCH_SIZE = iterations / 2;
    clock_t clicks = 0, insert = 0, valid = 0, invalid = 0;
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
    insert_time = (double)insert / sample_size;
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
    valid_time = (double)valid / sample_size;

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
    invalid_time = (double)invalid / sample_size;
}

void test_collisions(unsigned iterations) {
    using Record = record::Record<int>;
    using OpenHash = open_hash::OpenHash<Record>;
    using DoubleHash = double_hash::DoubleHash<Record>;
    using ChainedHash = chained_hash::ChainedHash<Record>;

    // CONSTANTS
    const unsigned MAX_RANGE = 10001, SIZE = 1000, SEARCH_SIZE = iterations / 2;

    // RESULT VARIABLES
    double ohash_collisions = 0, ohash_insert = 0, ohash_valid = 0,
           ohash_found = 0, ohash_invalid = 0, ohash_not_found = 0;
    double dhash_collisions = 0, dhash_insert = 0, dhash_valid = 0,
           dhash_found = 0, dhash_invalid = 0, dhash_not_found = 0;
    double chash_collisions, chash_insert = 0, chash_valid = 0, chash_found = 0,
                             chash_invalid = 0, chash_not_found = 0;

    // LISTS
    OpenHash ohash[SIZE];
    DoubleHash dhash[SIZE];
    ChainedHash chash[SIZE];
    std::vector<Record> list[SIZE];

    // add random values to array of vector of Records
    for(unsigned i = 0; i < SIZE; ++i) {
        for(unsigned j = 0; j < iterations; ++j)
            list[i].push_back(Record(rand() % MAX_RANGE, rand() % MAX_RANGE));

        assert(list[i].size() == iterations);
    }

    // CALL RANDOM TESTS
    test_random_hash(ohash, list, SIZE, iterations, ohash_collisions,
                     ohash_insert, ohash_valid, ohash_found, ohash_invalid,
                     ohash_not_found);

    test_random_hash(dhash, list, SIZE, iterations, dhash_collisions,
                     dhash_insert, dhash_valid, dhash_found, dhash_invalid,
                     dhash_not_found);

    test_random_hash(chash, list, SIZE, iterations, chash_collisions,
                     chash_insert, chash_valid, chash_found, chash_invalid,
                     chash_not_found);

    // CREATE DATA ARRAYS FOR OUTPUT
    const int HEAD_SIZE = 7;
    double odata[HEAD_SIZE - 1] = {ohash_collisions, ohash_insert,
                                   ohash_valid,      ohash_found,
                                   ohash_invalid,    ohash_not_found};
    double ddata[HEAD_SIZE - 1] = {dhash_collisions, dhash_insert,
                                   dhash_valid,      dhash_found,
                                   dhash_invalid,    dhash_not_found};
    double cdata[HEAD_SIZE - 1] = {0,           chash_insert,  chash_valid,
                                   chash_found, chash_invalid, chash_not_found};
    std::string header[HEAD_SIZE] = {
        "            ", "COLLISIONS",   "INSERTS (t)", "VALIDS (t)",
        "FOUND",        "INVALIDS (t)", "NOT FOUND"};

    // TITLE INFO OUTPUT
    std::cout << std::string(80, '-') << std::endl
              << "COLLISTION TESTS" << std::endl
              << std::string(80, '-') << std::endl
              << std::setw(11) << "sample size"
              << " : " << SIZE << std::endl
              << std::setw(11) << "iterations"
              << " : " << iterations << std::endl
              << std::setw(11) << "search size"
              << " : " << SEARCH_SIZE << std::endl
              << std::setw(11) << "t"
              << " : "
              << "clock ticks" << std::endl
              << std::endl;

    // HEADER LABELS 1
    std::cout << std::setw(header[0].size()) << ""  //
              << std::setw(header[1].size() + 1) << ""
              << std::setw(header[2].size() + 1) << ""
              << std::setw(header[3].size() + 1) << std::left << "SEARCH"
              << std::setw(header[4].size() + 1) << ""
              << std::setw(header[5].size() + 1) << std::left << "SEARCH"
              << std::setw(header[6].size() + 1) << "" << std::endl;

    // HEADER LABELS 2
    std::cout << std::setw(header[0].size()) << header[0];
    for(unsigned i = 1; i < HEAD_SIZE; ++i) {
        std::cout << std::setw(header[i].size() + 1) << header[i];
    }
    std::cout << std::endl;

    // HORIZONTAL BARS
    std::cout << std::setw(header[0].size())
              << std::string(header[0].size(), ' ');
    for(unsigned i = 1; i < HEAD_SIZE; ++i) {
        std::cout << std::setw(header[i].size() + 1)
                  << std::string(header[i].size(), '-');
    }
    std::cout << std::endl;

    // OPEN HASH DATA
    std::cout << std::setw(header[0].size()) << "OpenHash";
    for(int i = 0; i < HEAD_SIZE - 1; ++i) {
        std::cout << std::setw(header[i + 1].size() + 1) << odata[i];
    }
    std::cout << std::endl;

    // DOUBLE HASH DATA
    std::cout << std::setw(header[0].size()) << "DoubleHash";
    for(int i = 0; i < HEAD_SIZE - 1; ++i) {
        std::cout << std::setw(header[i + 1].size() + 1) << ddata[i];
    }
    std::cout << std::endl;

    // CHAINED HASH DATA
    std::cout << std::setw(header[0].size()) << "ChainedHash";
    for(int i = 0; i < HEAD_SIZE - 1; ++i) {
        std::cout << std::setw(header[i + 1].size() + 1) << cdata[i];
    }
    std::cout << std::endl;
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
                std::cout << ">> Clear: " << std::endl
                          << *obj << std::endl
                          << std::endl;

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
