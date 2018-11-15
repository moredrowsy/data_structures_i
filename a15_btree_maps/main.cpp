#include <cstdlib>              // srand(), rand()
#include <iostream>             // stream objects
#include <string>               // string objects
#include "../include/bt_map.h"  // BTree version of Map class

bool test_map_and_mmap_auto();
void test_map_interactive();
void test_mmap_interactive();

int main() {
    test_map_interactive();
    std::cout << std::endl;

    test_mmap_interactive();
    std::cout << std::endl;

    bool is_passed = true;
    for(int i = 0; i < 100; ++i) is_passed &= test_map_and_mmap_auto();

    std::cout << std::string(80, '-') << std::endl
              << "MAP and MMAP AUTO TESTS ";
    if(is_passed)
        std::cout << "PASSED" << std::endl;
    else
        std::cout << "FAILED" << std::endl;
    std::cout << std::string(80, '-') << std::endl;

    return 0;
}

bool test_map_and_mmap_auto() {
    using namespace bt_map;
    using namespace pair;

    const int MAX = 1000;

    Map<int, int> map;
    MMap<int, int> mmap;
    int keys[MAX];
    int values[MAX];

    // populate keys/values
    for(int i = 0; i < MAX; ++i) {
        keys[i] = i;
        values[i] = i;
    }

    // insert keys/values to map and mmap
    for(int i = 0; i < MAX; ++i) {
        map[keys[i]] = values[i];
        mmap[keys[i]] += values[i];

        if(!map.verify() || !mmap.verify()) {
            std::cout << "I N S E R T I O N  F A I L E D" << std::endl;
            return false;
        }

        if(!map.contains(Pair<int, int>(keys[i], values[i])) ||
           !mmap.contains(keys[i])) {
            std::cout << "I N S E R T  C O N T A I N S  F A I L E D"
                      << std::endl;
            return false;
        }
    }

    // remove keys from map and mmap
    for(int i = 0; i < MAX; ++i) {
        map.erase(keys[i]);
        mmap.erase(keys[i]);

        if(!map.verify() || !mmap.verify()) {
            std::cout << "E R A S E  F A I L E D" << std::endl;
            return false;
        }

        if(map.contains(Pair<int, int>(keys[i], values[i])) ||
           mmap.contains(keys[i])) {
            std::cout << "E R A S E D  C O N T A I N S  F A I L E D"
                      << std::endl;
            return false;
        }
    }

    return true;
}

void test_map_interactive() {
    using namespace bt_map;
    typedef pair::Pair<std::string, std::string> Pair;

    std::cout << std::string(80, '-') << std::endl
              << "INTERACTIVE TESTS: Map" << std::endl
              << std::string(80, '-') << std::endl;

    const std::size_t SIZE = 24;
    Map<std::string, std::string> map;
    bool is_found = false, is_removed = false;
    char c;
    std::string key, value;

    std::string keys[SIZE] = {"Aleph",  "Beth", "Gimel", "Daleth", "He",
                              "Zayin",  "Heth", "Teth",  "Yodh",   "Kaph",
                              "Lamedh", "Mem",  "Nun",   "Samekh", "Ayin",
                              "Pe",     "Res",  "Sin",   "Taw",    "Waw",
                              "Sade",   "Qoph", "Koppa", "Sampi"};

    std::string values[SIZE] = {"Alpha",  "Beta", "Gamma", "Delta", "Epsilon",
                                "Zeta",   "Eta",  "Theta", "Iota",  "Kappa",
                                "Lambda", "Mu",   "Nu",    "Xi",    "Omicron",
                                "Pi",     "Rho",  "Sigma", "Tau",   "Upsilon",
                                "Phi",    "Chi",  "Psi",   "Omega"};

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
                map.clear();
                std::cout << ">> Clear: " << std::endl << map << std::endl;

                break;
            case 'D':
            case 'd':
                std::cin >> key;
                is_removed = map.erase(key);

                std::cout << ">> Deletion: " << key;

                if(is_removed)
                    std::cout << " removed";
                else
                    std::cout << " does not exist";

                std::cout << std::endl << std::endl;
                map.print_debug();
                std::cout << std::endl;

                break;
            case 'F':
            case 'f':
                std::cin >> key;
                is_found = map.contains(key);

                std::cout << ">> Find: " << key;

                if(is_found)
                    std::cout << " success.";
                else
                    std::cout << " failed. Does not exist";

                std::cout << std::endl << std::endl;
                map.print_debug();
                std::cout << std::endl;

                break;
            case 'I':
            case 'i':
                std::cin >> key >> value;
                map[key] = value;

                std::cout << ">> Insert: " << Pair(key, value) << std::endl
                          << std::endl;
                map.print_debug();
                std::cout << std::endl;

                break;
            case 'R':
            case 'r':
                key = keys[rand() % SIZE];
                value = values[rand() % SIZE];
                map[key] = value;

                std::cout << ">> Random insert: " << Pair(key, value)
                          << std::endl
                          << std::endl;
                map.print_debug();
                std::cout << std::endl;

                break;
            case 'S':
            case 's':
                std::cout << ">> Size: " << map.size() << std::endl
                          << std::endl;
                break;
            default:
                std::cout << "Invalid Choice" << std::endl << std::endl;
        }
    } while(c != 'X' && c != 'x');
}

void test_mmap_interactive() {
    using namespace bt_map;
    typedef pair::MPair<std::string, std::string> Pair;

    std::cout << std::string(80, '-') << std::endl
              << "INTERACTIVE TESTS: Multi-Map" << std::endl
              << std::string(80, '-') << std::endl;

    const std::size_t SIZE = 24;
    MMap<std::string, std::string> mmap;
    bool is_found = false, is_removed = false;
    char c;
    std::string key, value;

    std::string keys[SIZE] = {"Aleph",  "Beth", "Gimel", "Daleth", "He",
                              "Zayin",  "Heth", "Teth",  "Yodh",   "Kaph",
                              "Lamedh", "Mem",  "Nun",   "Samekh", "Ayin",
                              "Pe",     "Res",  "Sin",   "Taw",    "Waw",
                              "Sade",   "Qoph", "Koppa", "Sampi"};

    std::string values[SIZE] = {"Alpha",  "Beta", "Gamma", "Delta", "Epsilon",
                                "Zeta",   "Eta",  "Theta", "Iota",  "Kappa",
                                "Lambda", "Mu",   "Nu",    "Xi",    "Omicron",
                                "Pi",     "Rho",  "Sigma", "Tau",   "Upsilon",
                                "Phi",    "Chi",  "Psi",   "Omega"};

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
                mmap.clear();
                std::cout << ">> Clear: " << std::endl << mmap << std::endl;

                break;
            case 'D':
            case 'd':
                std::cin >> key;
                is_removed = mmap.erase(key);

                std::cout << ">> Deletion: " << key;

                if(is_removed)
                    std::cout << " removed";
                else
                    std::cout << " does not exist";

                std::cout << std::endl << std::endl;
                mmap.print_debug();
                std::cout << std::endl;

                break;
            case 'F':
            case 'f':
                std::cin >> key;
                is_found = mmap.contains(key);

                std::cout << ">> Find: " << key;

                if(is_found)
                    std::cout << " success.";
                else
                    std::cout << " failed. Does not exist";

                std::cout << std::endl << std::endl;
                mmap.print_debug();
                std::cout << std::endl;

                break;
            case 'I':
            case 'i':
                std::cin >> key >> value;
                mmap[key] += value;

                std::cout << ">> Insert: " << Pair(key, value) << std::endl
                          << std::endl;
                mmap.print_debug();
                std::cout << std::endl;

                break;
            case 'R':
            case 'r':
                key = keys[rand() % SIZE];
                value = values[rand() % SIZE];
                mmap[key] += value;

                std::cout << ">> Random insert: " << Pair(key, value)
                          << std::endl
                          << std::endl;
                mmap.print_debug();
                std::cout << std::endl;

                break;
            case 'S':
            case 's':
                std::cout << ">> Size: " << mmap.size() << std::endl
                          << std::endl;
                break;
            default:
                std::cout << "Invalid Choice" << std::endl << std::endl;
        }
    } while(c != 'X' && c != 'x');
}
