#include <iostream>             // stream objects
#include <string>               // string objects
#include "../include/bt_map.h"  // BTree version of Map class

void test_map_interactive();
void test_mmap_interactive();

int main() {
    test_map_interactive();
    test_mmap_interactive();

    return 0;
}

void test_map_interactive() {
    using namespace btree;
    typedef pair::Pair<std::string, std::string> Pair;

    std::cout << std::string(80, '-') << std::endl
              << "INTERACTIVE TESTS: Map" << std::endl
              << std::string(80, '-') << std::endl;

    const std::size_t SIZE = 24;
    bt_map::Map<std::string, std::string> map;
    bool is_inserted = false, is_found = false, is_removed = false;
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
                is_inserted = map.insert(key, value);

                std::cout << ">> Insert: " << Pair(key, value);

                if(is_inserted)
                    std::cout << " success.";
                else
                    std::cout << " failed. Duplicate?";

                std::cout << std::endl << std::endl;
                map.print_debug();
                std::cout << std::endl;

                break;
            case 'R':
            case 'r':
                key = keys[rand() % SIZE];
                value = values[rand() % SIZE];
                is_inserted = map.insert(key, value);

                std::cout << ">> Random insert: " << Pair(key, value);

                if(is_inserted)
                    std::cout << " success.";
                else
                    std::cout << " failed. Duplicate?";

                std::cout << std::endl << std::endl;
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
    using namespace btree;
    typedef pair::MPair<std::string, std::string> Pair;

    std::cout << std::string(80, '-') << std::endl
              << "INTERACTIVE TESTS: Multi-Map" << std::endl
              << std::string(80, '-') << std::endl;

    const std::size_t SIZE = 24;
    bt_map::MMap<std::string, std::string> mmap;
    bool is_inserted = false, is_found = false, is_removed = false;
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
                is_inserted = mmap.insert(key, value);

                std::cout << ">> Insert: " << Pair(key, value);

                if(is_inserted)
                    std::cout << " success.";
                else
                    std::cout << " failed. Duplicate?";

                std::cout << std::endl << std::endl;
                mmap.print_debug();
                std::cout << std::endl;

                break;
            case 'R':
            case 'r':
                key = keys[rand() % SIZE];
                value = values[rand() % SIZE];
                is_inserted = mmap.insert(key, value);

                std::cout << ">> Random insert: " << Pair(key, value);

                if(is_inserted)
                    std::cout << " success.";
                else
                    std::cout << " failed. Duplicate?";

                std::cout << std::endl << std::endl;
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
