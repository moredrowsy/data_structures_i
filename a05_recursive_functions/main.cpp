/*******************************************************************************
 * AUTHOR        : Thuan Tang
 * ID            : 00991588
 * CLASS         : CS008
 * ASSIGNMENT 05 : Recursive Functions
 * DESCRIPTION   : This program tests various recursive functions to illustrate
 *      their mechanism. Additionally, you can call specific tests via command
 *      line arguments in any order.
 ******************************************************************************/
#include <cstring>
#include <iostream>
#include "../include/my_recursions.h"

void test_r1();
void test_r2();
void test_r3();
void test_r4();
void test_r5();
void test_r16();

int main(int argc, char* argv[]) {
    // declare array of function pointers
    const int FUNC_SIZE = 6;
    const char* args[FUNC_SIZE] = {"1", "2", "3", "4", "5", "16"};
    typedef void (*func_ptr)();
    func_ptr func_arr[FUNC_SIZE] = {&test_r1, &test_r2, &test_r3,
                                    &test_r4, &test_r5, &test_r16};

    // call functions by arguments
    if(argc > 1) {
        for(int i = 1; i < argc; ++i) {
            for(int j = 0; j < FUNC_SIZE; ++j) {
                if(!strcmp(argv[i], args[j])) {
                    func_arr[j]();
                    std::cout << std::endl << std::endl;
                }
            }
        }
    } else {  // call all functions in function array
        for(int i = 1; i <= FUNC_SIZE; ++i) {
            func_arr[i - 1]();
            std::cout << std::endl << std::endl;
        }
    }

    return 0;
}

void test_r1() {
    using namespace my_recursions;

    std::cout << std::string(80, '=') << std::endl;
    unsigned start_level = 1, end_level = 4;
    std::cout << "Recursion 1: Prints out levels before/after recursion:"
              << std::endl
              << "start_level = " << start_level
              << ", end_level = " << end_level << std::endl
              << std::string(80, '-') << std::endl;
    r1_levels(start_level, end_level);
    std::cout << std::string(80, '=') << std::endl;
}

void test_r2() {
    using namespace my_recursions;

    std::cout << std::string(80, '=') << std::endl;
    std::string prefix = "BOX:";
    unsigned section_level = 3;
    std::cout << "Recursion 2: Prints out prefix + successive levels of "
                 "sections: "
              << std::endl
              << "prefix = " << prefix << ", level = " << section_level
              << std::endl
              << std::string(80, '-') << std::endl;
    r2_box(prefix, section_level);
    std::cout << std::string(80, '=') << std::endl;
}

void test_r3() {
    using namespace my_recursions;

    std::string first = "CAT", second = "MAN";
    std::cout << std::string(80, '=') << std::endl;
    std::cout << "Recursion 3: Prints out permutations of the first word:"
              << std::endl
              << "first word = " << first << ", second word = " << second
              << std::endl
              << std::string(80, '-') << std::endl;
    r3_permutation(first, second);
    std::cout << std::string(80, '=') << std::endl;
}

void test_r4() {
    using namespace my_recursions;

    std::cout << std::string(80, '=') << std::endl;
    std::cout << "Recursion 4: Interactively ask user to pick a number to "
                 "open successive boxes."
              << std::endl
              << std::string(80, '-') << std::endl;
    double total_boxes = r4_i_box("", 3);
    std::cout << std::endl << "total_boxes = " << total_boxes << std::endl;
    std::cout << std::string(80, '=') << std::endl;
}

void test_r5() {
    using namespace my_recursions;

    std::cout << std::string(80, '=') << std::endl;
    unsigned reciprocal_level = 3;
    std::cout << "Recursion 5: Returns the sum of reciprocals of levels"
              << std::endl
              << "reciprocal level = " << reciprocal_level << std::endl
              << std::string(80, '-') << std::endl;
    double sumover = r5_sumover(reciprocal_level);
    std::cout << "sumover = " << sumover << std::endl;
    std::cout << std::string(80, '=') << std::endl;
}

void test_r16() {
    using namespace my_recursions;

    std::cout << std::string(80, '=') << std::endl;
    unsigned low = 1, high = 10;
    std::cout << "Recursion 16: Guess the user's number between " << low
              << " to " << high << "." << std::endl
              << std::string(80, '-') << std::endl;
    double guess = r16_guess(low, high);
    std::cout << "guess = " << guess << std::endl;
    std::cout << std::string(80, '=') << std::endl;
}
