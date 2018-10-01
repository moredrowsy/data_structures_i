/*******************************************************************************
 * AUTHOR        : Thuan Tang
 * ID            : 00991588
 * CLASS         : CS008
 * ASSIGNMENT 05 : Recursive Functions
 * DESCRIPTION   : This program tests various recursive functions to illustrate
 *      their mechanism. Additionally, you can call specific tests via command
 *      line arguments in any order by the test number.
 ******************************************************************************/
#include <cstring>
#include <iostream>
#include "../include/my_recursions.h"

void test_r1();   // prints recursion levels
void test_r2();   // prints prefix + section levels
void test_r3();   // prints permutation of first string
void test_r4();   // prints recursive interactions
void test_r5();   // prints sum of reciprocal levels
void test_r16();  // prints recursive guesses

int main(int argc, char* argv[]) {
    // declare array of function pointers
    const int FN_SIZE = 6;
    const char* args[FN_SIZE] = {"1", "2", "3", "4", "5", "16"};  // arguments
    typedef void (*fn_ptr)();  // declare function pointer type
    fn_ptr fn_arr[FN_SIZE] = {&test_r1, &test_r2, &test_r3,
                              &test_r4, &test_r5, &test_r16};

    // call functions by arguments
    if(argc > 1) {
        for(int i = 1; i < argc; ++i) {  // skip argv[0]
            for(int j = 0; j < FN_SIZE; ++j) {
                if(!strcmp(argv[i], args[j])) {  // when argv == args is true
                    fn_arr[j]();                 // call function pointer
                    std::cout << std::endl << std::endl;
                }
            }
        }
    } else {  // call all functions in function array
        for(int i = 0; i < FN_SIZE; ++i) {
            fn_arr[i]();
            std::cout << std::endl << std::endl;
        }
    }

    return 0;
}

void test_r1() {
    using namespace my_recursions;

    unsigned start_level = -1, end_level = -1;

    std::cout << std::string(80, '=') << std::endl;
    std::cout << "Recursion 1: Prints out levels before/after recursion:"
              << std::endl;

    std::cout << std::endl;

    // test case 1
    start_level = 0, end_level = 0;
    std::cout << "start_level = " << start_level
              << ", end_level = " << end_level << std::endl
              << std::string(80, '-') << std::endl;
    r1_levels(start_level, end_level);

    std::cout << std::endl;

    // test case 2
    start_level = 0, end_level = 1;
    std::cout << "start_level = " << start_level
              << ", end_level = " << end_level << std::endl
              << std::string(80, '-') << std::endl;
    r1_levels(start_level, end_level);

    std::cout << std::endl;

    // test case 3
    start_level = 0, end_level = 4;
    std::cout << "start_level = " << start_level
              << ", end_level = " << end_level << std::endl
              << std::string(80, '-') << std::endl;
    r1_levels(start_level, end_level);

    std::cout << std::string(80, '=') << std::endl;
}

void test_r2() {
    using namespace my_recursions;

    std::string prefix = "BOX:";
    unsigned section_level = -1;

    std::cout << std::string(80, '=') << std::endl;
    std::cout << "Recursion 2: Prints out prefix + successive levels of "
                 "sections: "
              << std::endl;

    std::cout << std::endl;

    // test case 1
    section_level = 1;
    std::cout << "prefix = " << prefix << ", level = " << section_level
              << std::endl
              << std::string(80, '-') << std::endl;
    r2_box(prefix, section_level);

    std::cout << std::endl;

    // test case 2
    section_level = 2;
    std::cout << "prefix = " << prefix << ", level = " << section_level
              << std::endl
              << std::string(80, '-') << std::endl;
    r2_box(prefix, section_level);

    std::cout << std::endl;

    // test case 3
    section_level = 3;
    std::cout << "prefix = " << prefix << ", level = " << section_level
              << std::endl
              << std::string(80, '-') << std::endl;
    r2_box(prefix, section_level);

    std::cout << std::string(80, '=') << std::endl;
}

void test_r3() {
    using namespace my_recursions;

    std::string first, second;

    std::cout << std::string(80, '=') << std::endl;
    std::cout << "Recursion 3: Prints out permutations of the first word:"
              << std::endl;

    std::cout << std::endl;

    // test case 1
    first = "C", second = "MAN";
    std::cout << "first word = " << first << ", second word = " << second
              << std::endl
              << std::string(80, '-') << std::endl;
    r3_permutation(first, second);

    std::cout << std::endl;

    // test case 2
    first = "CA", second = "MAN";
    std::cout << "first word = " << first << ", second word = " << second
              << std::endl
              << std::string(80, '-') << std::endl;
    r3_permutation(first, second);

    std::cout << std::endl;

    // test case 3
    first = "CAT", second = "MAN";
    std::cout << "first word = " << first << ", second word = " << second
              << std::endl
              << std::string(80, '-') << std::endl;
    r3_permutation(first, second);

    std::cout << std::endl;

    // test case 4
    first = "CATS", second = "MAN";
    std::cout << "first word = " << first << ", second word = " << second
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

    // test case 1
    double total_boxes = r4_i_box("");
    std::cout << std::endl << "total_boxes = " << total_boxes << std::endl;
    std::cout << std::string(80, '=') << std::endl;
}

void test_r5() {
    using namespace my_recursions;

    double sumover = -1;
    unsigned reciprocal_level = 0;

    std::cout << std::string(80, '=') << std::endl;
    std::cout << "Recursion 5: Returns the sum of reciprocals of levels"
              << std::endl;

    std::cout << std::endl;

    // test case 1
    reciprocal_level = 0;
    std::cout << "reciprocal level = " << reciprocal_level << std::endl
              << std::string(80, '-') << std::endl;
    sumover = r5_sumover(reciprocal_level);
    std::cout << "sumover = " << sumover << std::endl;

    std::cout << std::endl;

    // test case 2
    reciprocal_level = 1;
    std::cout << "reciprocal level = " << reciprocal_level << std::endl
              << std::string(80, '-') << std::endl;
    sumover = r5_sumover(reciprocal_level);
    std::cout << "sumover = " << sumover << std::endl;

    std::cout << std::endl;

    // test case 3
    reciprocal_level = 2;
    std::cout << "reciprocal level = " << reciprocal_level << std::endl
              << std::string(80, '-') << std::endl;
    sumover = r5_sumover(reciprocal_level);
    std::cout << "sumover = " << sumover << std::endl;

    std::cout << std::endl;

    // test case 4
    reciprocal_level = 3;
    std::cout << "reciprocal level = " << reciprocal_level << std::endl
              << std::string(80, '-') << std::endl;
    sumover = r5_sumover(reciprocal_level);
    std::cout << "sumover = " << sumover << std::endl;

    std::cout << std::endl;

    // test case 5
    reciprocal_level = 4;
    std::cout << "reciprocal level = " << reciprocal_level << std::endl
              << std::string(80, '-') << std::endl;
    sumover = r5_sumover(reciprocal_level);
    std::cout << "sumover = " << sumover << std::endl;

    std::cout << std::endl;

    // test case 6
    reciprocal_level = 5;
    std::cout << "reciprocal level = " << reciprocal_level << std::endl
              << std::string(80, '-') << std::endl;
    sumover = r5_sumover(reciprocal_level);
    std::cout << "sumover = " << sumover << std::endl;

    std::cout << std::string(80, '=') << std::endl;
}

void test_r16() {
    using namespace my_recursions;

    std::cout << std::string(80, '=') << std::endl;
    unsigned low = 1, high = 1000000;
    std::cout << "Recursion 16: Guess the user's number between " << low
              << " to " << high << "." << std::endl
              << std::string(80, '-') << std::endl;

    // test case 1
    double guess = r16_guess(low, high);
    std::cout << "guess = " << guess << std::endl;
    std::cout << std::string(80, '=') << std::endl;
}
