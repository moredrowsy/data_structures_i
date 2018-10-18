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

void test_collisions(unsigned iterations) {
    using Record = record::Record<int>;
    using OpenHash = open_hash::OpenHash<Record>;
    using DoubleHash = double_hash::DoubleHash<Record>;
    using ChainedHash = chained_hash::ChainedHash<Record>;

    // CONSTANTS
    const unsigned MAX_RANGE = 10001, SIZE = 1000, SEARCH_SIZE = iterations / 2;

    // RESULT VARIABLES
    bool is_found = false;
    int ohash_found = 0, dhash_found = 0, chash_found = 0, ohash_not_found = 0,
        dhash_not_found = 0, chash_not_found = 0;
    clock_t clicks = 0, ohash_insert = 0, ohash_valid = 0, ohash_invalid = 0,
            dhash_insert = 0, dhash_valid = 0, dhash_invalid = 0,
            chash_insert = 0, chash_valid = 0, chash_invalid = 0;
    double ohash_collisions = 0, dhash_collisions = 0;
    Record result;

    // LISTs
    OpenHash ohash[SIZE];
    DoubleHash dhash[SIZE];
    ChainedHash chash[SIZE];

    std::vector<Record> list[SIZE];

    srand(time(nullptr));

    // add random values to array of vector of Records
    for(unsigned i = 0; i < SIZE; ++i) {
        for(unsigned j = 0; j < iterations; ++j)
            list[i].push_back(Record(rand() % MAX_RANGE, rand() % MAX_RANGE));

        assert(list[i].size() == iterations);
    }

    // inserts: OpenHash
    for(unsigned i = 0; i < SIZE; ++i) {
        clicks = std::clock();
        for(auto& a : list[i]) ohash[i].insert(a);
        ohash_insert += std::clock() - clicks;

        assert(ohash[i].size() != 0);

        ohash_collisions += ohash[i].collisions();
    }

    // inserts: DoubleHash
    for(unsigned i = 0; i < SIZE; ++i) {
        clicks = std::clock();
        for(auto& a : list[i]) dhash[i].insert(a);
        dhash_insert += std::clock() - clicks;

        assert(dhash[i].size() != 0);

        dhash_collisions += dhash[i].collisions();
    }

    // inserts: ChainedHash
    for(unsigned i = 0; i < SIZE; ++i) {
        clicks = std::clock();
        for(auto& a : list[i]) chash[i].insert(a);
        chash_insert += std::clock() - clicks;

        assert(chash[i].size() != 0);
    }

    // search valid : OpenHash
    for(unsigned i = 0; i < SIZE; ++i) {
        // ensure that all searches are TRUE
        for(unsigned j = 0; j < SEARCH_SIZE; ++j) {
            is_found = ohash[i].find(list[i].at(j)._key, result);
            if(is_found) ohash_found += 1;
        }

        // start timing inserts
        clicks = std::clock();
        for(unsigned j = 0; j < SEARCH_SIZE; ++j)
            ohash[i].find(list[i].at(j)._key, result);
        ohash_valid += std::clock() - clicks;
    }

    // search valid : DoubleHash
    for(unsigned i = 0; i < SIZE; ++i) {
        // ensure that all searches are TRUE
        for(unsigned j = 0; j < SEARCH_SIZE; ++j) {
            is_found = dhash[i].find(list[i].at(j)._key, result);
            if(is_found) dhash_found += 1;
        }

        // start timing inserts
        clicks = std::clock();
        for(unsigned j = 0; j < SEARCH_SIZE; ++j)
            dhash[i].find(list[i].at(j)._key, result);
        dhash_valid += std::clock() - clicks;
    }

    // search valid : ChainedHash
    for(unsigned i = 0; i < SIZE; ++i) {
        // ensure that all searches are TRUE
        for(unsigned j = 0; j < SEARCH_SIZE; ++j) {
            is_found = chash[i].find(list[i].at(j)._key, result);
            if(is_found) chash_found += 1;
        }

        // start timing inserts
        clicks = std::clock();
        for(unsigned j = 0; j < SEARCH_SIZE; ++j)
            chash[i].find(list[i].at(j)._key, result);
        chash_valid += std::clock() - clicks;
    }

    // search invalid: OpenHash
    for(unsigned i = 0; i < SIZE; ++i) {
        // ensure that all searches are FALSE
        for(int j = -100; j > -350; --j) {
            is_found = ohash[i].find(j, result);
            if(is_found) ohash_not_found += 1;
        }

        // start timing inserts
        clicks = std::clock();
        for(int j = -100; j > -350; --j) ohash[i].find(j, result);
        ohash_invalid += std::clock() - clicks;
    }

    // search invalid: DoubleHash
    for(unsigned i = 0; i < SIZE; ++i) {
        // ensure that all searches are FALSE
        for(int j = -100; j > -350; --j) {
            is_found = dhash[i].find(j, result);
            if(is_found) dhash_not_found += 1;
        }

        // start timing inserts
        clicks = std::clock();
        for(int j = -100; j > -350; --j) dhash[i].find(j, result);
        dhash_invalid += std::clock() - clicks;
    }

    // search invalid: ChainedHash
    for(unsigned i = 0; i < SIZE; ++i) {
        // ensure that all searches are FALSE
        for(int j = -100; j > -350; --j) {
            is_found = chash[i].find(j, result);
            if(is_found) chash_not_found += 1;
        }

        // start timing inserts
        clicks = std::clock();
        for(int j = -100; j > -350; --j) chash[i].find(j, result);
        chash_invalid += std::clock() - clicks;
    }

    const int HEAD_SIZE = 7;
    std::string header[HEAD_SIZE] = {
        "            ", "COLLISIONS",   "INSERTS (t)", "VALIDS (t)",
        "FOUND",        "INVALIDS (t)", "NOT FOUND"};

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

    std::cout
        // header labels 1
        << std::setw(header[0].size()) << ""  //
        << std::setw(header[1].size() + 1) << ""
        << std::setw(header[2].size() + 1) << ""
        << std::setw(header[3].size() + 1) << std::left << "SEARCH"
        << std::setw(header[4].size() + 1) << ""
        << std::setw(header[5].size() + 1) << std::left << "SEARCH"
        << std::setw(header[6].size() + 1) << ""
        << std::endl
        // header labels 2
        << std::setw(header[0].size()) << header[0]
        << std::setw(header[1].size() + 1) << header[1]
        << std::setw(header[2].size() + 1) << header[2]
        << std::setw(header[3].size() + 1) << header[3]
        << std::setw(header[4].size() + 1) << header[4]
        << std::setw(header[5].size() + 1) << header[5]
        << std::setw(header[6].size() + 1) << header[6]
        << std::endl
        // horizontal bars
        << std::setw(header[0].size()) << std::string(header[0].size(), ' ')
        << std::setw(header[1].size() + 1) << std::string(header[1].size(), '-')
        << std::setw(header[2].size() + 1) << std::string(header[2].size(), '-')
        << std::setw(header[3].size() + 1) << std::string(header[3].size(), '-')
        << std::setw(header[4].size() + 1) << std::string(header[4].size(), '-')
        << std::setw(header[5].size() + 1) << std::string(header[5].size(), '-')
        << std::setw(header[6].size() + 1) << std::string(header[6].size(), '-')
        << std::endl
        // OpenHash data
        << std::setw(header[0].size()) << "OpenHash"
        << std::setw(header[1].size() + 1) << ohash_collisions / SIZE
        << std::setw(header[2].size() + 1) << (float)ohash_insert / SIZE
        << std::setw(header[3].size() + 1) << (float)ohash_valid / SIZE
        << std::setw(header[4].size() + 1) << (float)ohash_found / SIZE
        << std::setw(header[5].size() + 1) << (float)ohash_invalid / SIZE
        << std::setw(header[6].size() + 1) << (float)ohash_not_found / SIZE
        << std::endl
        // DoubleHash data
        << std::setw(header[0].size()) << "DoubleHash"
        << std::setw(header[1].size() + 1) << dhash_collisions / SIZE
        << std::setw(header[2].size() + 1) << (float)dhash_insert / SIZE
        << std::setw(header[3].size() + 1) << (float)dhash_valid / SIZE
        << std::setw(header[4].size() + 1) << (float)dhash_found / SIZE
        << std::setw(header[5].size() + 1) << (float)dhash_invalid / SIZE
        << std::setw(header[6].size() + 1) << (float)dhash_not_found / SIZE
        << std::endl
        // ChainedHash data
        << std::setw(header[0].size()) << "ChainedHash"
        << std::setw(header[1].size() + 1) << "N/A"
        << std::setw(header[2].size() + 1) << (float)chash_insert / SIZE
        << std::setw(header[3].size() + 1) << (float)chash_valid / SIZE
        << std::setw(header[4].size() + 1) << (float)chash_found / SIZE
        << std::setw(header[5].size() + 1) << (float)chash_invalid / SIZE
        << std::setw(header[6].size() + 1) << (float)chash_not_found / SIZE
        << std::endl;
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
