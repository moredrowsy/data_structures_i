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

void interactive_tests();  // interactive test harness for PQueue class

int main() {
    interactive_tests();

    return 0;
}

void interactive_tests() {
    using namespace pqueue;

    bool is_inserted = false;
    char c;
    int i = 0, priority = 0;
    const int SIZE = 24;
    std::string item;
    Info<std::string> info;
    PQueue<std::string> pqueue;
    std::string items[SIZE] = {"Alpha",  "Beta", "Gamma", "Delta", "Epsilon",
                               "Zeta",   "Eta",  "Theta", "Iota",  "Kappa",
                               "Lambda", "Mu",   "Nu",    "Xi",    "Omicron",
                               "Pi",     "Rho",  "Sigma", "Tau",   "Upsilon",
                               "Phi",    "Chi",  "Psi",   "Omega"};
    int priorities[SIZE];
    for(int i = 0; i < SIZE; ++i) priorities[i] = i + 1;

    srand(time(nullptr));

    do {
        std::cout << "[R]andom  [I]nsert  [C]lear  [P]pop     "
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
