/*******************************************************************************
 * AUTHOR        : Thuan Tang
 * ID            : 00991588
 * CLASS         : CS008
 * ASSIGNMENT 09 : Heap Class
 * DESCRIPTION   : This program is an interactive test harness for the Heap
 *      class (max binary heap tree), which will insert/pop maximum item on top.
 ******************************************************************************/
#include <cstdlib>            // rand(), srand()
#include <ctime>              // time()
#include <iostream>           // stream objects
#include "../include/heap.h"  // Heap class

void interactive_tests();  // interactive test harness for Heap class

int main() {
    interactive_tests();

    return 0;
}

void interactive_tests() {
    using namespace heap;

    bool is_inserted = false;
    char c;
    int i = 0;
    Heap<int> heap;
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
                heap.clear();
                std::cout << heap << std::endl;

                break;
            case 'I':
            case 'i':
                std::cout << "-- Inserting: ";
                std::cin >> i;
                is_inserted = heap.insert(i);

                if(!is_inserted) std::cout << "Insertion failed." << std::endl;

                std::cout << heap << std::endl;

                break;
            case 'P':
            case 'p':
                std::cout << "-- Popping: ";
                if(!heap.empty()) {
                    i = heap.pop();
                    std::cout << i << std::endl;
                } else {
                    std::cout << "cannot pop empty heap" << std::endl;
                }

                std::cout << heap << std::endl;

                break;
            case 'R':
            case 'r':
                std::cout << "-- Inserting random: ";
                i = rand() % 100;
                is_inserted = heap.insert(i);
                std::cout << i << std::endl;

                if(!is_inserted)
                    std::cout
                        << "Insertion failed. No more unique random numbers"
                        << std::endl;

                std::cout << heap << std::endl;

                break;
            default:
                std::cout << "Invalid Choice" << std::endl << std::endl;
        }

        std::cout << std::string(80, '-') << std::endl;

    } while(c != 'X' && c != 'x');
}
