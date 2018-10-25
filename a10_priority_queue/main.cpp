/*******************************************************************************
 * AUTHOR        : Thuan Tang
 * ID            : 00991588
 * CLASS         : CS008
 * ASSIGNMENT 10 : PQueue Class
 * DESCRIPTION   : This program is an interactive test harness for the PQueue
 *      (Priority Queue) class. Insertion of items will insert Info struct into
 *      the PQueue's heap. Insertions/pops will always place highest priority
 *      item on top of the queue. Items with equal priorities will insert
 *      correctly but pop order is undefined.
 ******************************************************************************/
#include <cstdlib>              // rand(), srand()
#include <ctime>                // time()
#include <iostream>             // stream objects
#include <string>               // string objects
#include "../include/pqueue.h"  // Info struct, PQueue class

void interactive_tests(bool reverse = true);
void test_random_items(bool reverse = true);  // test with 100 random items

int main() {
    interactive_tests();
    std::cout << std::endl;
    test_random_items();

    return 0;
}

void interactive_tests(bool reverse) {
    using namespace pqueue;

    bool is_inserted = false;
    char c;
    int i = 0, priority = 0;
    const int SIZE = 24;
    std::string item;
    Info<std::string> info;
    PQueue<std::string> pqueue(reverse);
    std::string items[SIZE] = {"Alpha",  "Beta", "Gamma", "Delta", "Epsilon",
                               "Zeta",   "Eta",  "Theta", "Iota",  "Kappa",
                               "Lambda", "Mu",   "Nu",    "Xi",    "Omicron",
                               "Pi",     "Rho",  "Sigma", "Tau",   "Upsilon",
                               "Phi",    "Chi",  "Psi",   "Omega"};
    int priorities[SIZE];
    for(int i = 0; i < SIZE; ++i) priorities[i] = i + 1;

    srand(time(nullptr));

    do {
        std::cout << "[R]andom  [I]nsert  [C]lear  [P]op     "
                     "e[X]it: ";
        std::cin >> c;

        switch(c) {
            case 'X':
            case 'x':
                return;  // exit function
            case 'C':
            case 'c':
                std::cout << "-- Clearing tree" << std::endl;
                pqueue.clear();
                std::cout << pqueue << std::endl;

                break;
            case 'I':
            case 'i':
                std::cout << "-- Inserting: ";
                std::cin >> item >> priority;
                info = Info<std::string>(item, priority);
                is_inserted = pqueue.insert(info);

                if(is_inserted)
                    std::cout << info << std::endl;
                else
                    std::cout << "Insertion failed." << std::endl;

                std::cout << pqueue << std::endl;

                break;
            case 'P':
            case 'p':
                std::cout << "-- Popping: ";
                if(!pqueue.empty()) {
                    item = pqueue.pop();
                    std::cout << item << std::endl;
                } else {
                    std::cout << "cannot pop empty pqueue" << std::endl;
                }

                std::cout << pqueue << std::endl;

                break;
            case 'R':
            case 'r':
                std::cout << "-- Inserting random: ";
                i = rand() % SIZE;
                info = Info<std::string>(items[i], priorities[i]);
                is_inserted = pqueue.insert(info);
                std::cout << info << std::endl;

                if(!is_inserted)
                    std::cout
                        << "Insertion failed. No more unique random numbers"
                        << std::endl;

                std::cout << pqueue << std::endl;

                break;
            default:
                std::cout << "Invalid Choice" << std::endl << std::endl;
        }

        std::cout << std::string(80, '-') << std::endl;

    } while(c != 'X' && c != 'x');
}

void test_random_items(bool reverse) {
    using namespace pqueue;

    std::cout << std::string(80, '-') << std::endl
              << "Testing 100 insertions/pops of random items" << std::endl
              << std::string(80, '-') << std::endl;

    // insert random items into pqueue
    std::cout << "Making pqueue with 100 random inserts" << std::endl;
    PQueue<int> pqueue(reverse);
    for(int i = 0; i < 100; ++i) pqueue.insert(rand() % 1000, rand() % 10);

    // create pqueue_copy
    std::cout << "Making pqueue_copy(pqueue) for debug" << std::endl;
    PQueue<int> pqueue_copy(pqueue);

    std::cout << std::endl;

    std::cout << "Popping all of pqueue" << std::endl;
    while(!pqueue.empty()) std::cout << pqueue.pop() << std::endl;

    std::cout << std::endl;

    std::cout << "Popping all of pqueue_copy with debug" << std::endl;
    while(!pqueue_copy.empty())
        std::cout << pqueue_copy.pop_debug() << std::endl;
}
