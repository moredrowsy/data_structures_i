/*******************************************************************************
 * AUTHOR        : Thuan Tang
 * ID            : 00991588
 * CLASS         : CS008
 * ASSIGNMENT 05 : Recursive Functions
 * DESCRIPTION   : This program tests various recursive functions to illustrate
 *      their mechanism.
 ******************************************************************************/
#include <iostream>

// prints out level before/after recursion
void recurve1_levels(unsigned start_level, unsigned end_level);

// prints out prefix + successive levels of numbers (0 to 9)
void recurve2_box(std::string prefix, unsigned level);

int main() {
    // recursion function 1
    std::cout << "Recursion 1: Prints out levels before/after recursion."
              << std::endl
              << std::string(80, '-') << std::endl;
    recurve1_levels(1, 4);
    std::cout << std::endl << std::endl;

    // recursion function 2
    std::cout << "Recursion 2: Prints out Prefix + successive levels of "
                 "numbers."
              << std::endl
              << std::string(80, '-') << std::endl;
    recurve2_box("BOX:", 3);
}

void recurve1_levels(unsigned start_level, unsigned end_level) {
    if(start_level > end_level) {  // base: start level is beyond end level
        return;
    }

    // print out first sentence
    std::cout << std::string(start_level, ' ')
              << "This was written by call number " << start_level << "."
              << std::endl;

    // recurve to higher level
    recurve1_levels(start_level + 1, end_level);

    // print out second sentence
    std::cout << std::string(start_level, ' ')
              << "This ALSO written by call number " << start_level << "."
              << std::endl;
}

void recurve2_box(std::string prefix, unsigned level) {
    if(level == 0) {                       // base: invalid level 0
        std::cout << prefix << std::endl;  // print out final string
        return;
    }

    for(int i = 1; i < 10; ++i) {  // iterate all digits (0 to 9)
        std::string s = prefix;    // create new string from previous string
        s += char('0' + i);        // append digit with ASCII shift
        if(level != 1) s += '.';   // append 'dot' if not last valid level

        recurve2_box(s, level - 1);  // recurve to lower level
    }
}
