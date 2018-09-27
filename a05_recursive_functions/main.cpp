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
void r1_levels(unsigned start_level, unsigned end_level);

// prints out prefix + successive levels of numbers (0 to 9)
void r2_box(std::string prefix, unsigned level);

// prints out permutation of the first word
void r3_permutation(std::string first, std::string second);

// ask user pick prefix within prefix
unsigned r4_i_box(std::string prefix, unsigned level);

// prints ouf the sum of reciprocal of levels
double r5_sumover(unsigned level);

// guess user's number
int r16_guess(unsigned low, unsigned high);

int main() {
    // recursion function 1
    std::cout << std::string(80, '=') << std::endl;
    unsigned start_level = 1, end_level = 4;
    std::cout << "Recursion 1: Prints out levels before/after recursion:"
              << std::endl
              << "start_level = " << start_level
              << ", end_level = " << end_level << std::endl
              << std::string(80, '-') << std::endl;
    r1_levels(start_level, end_level);
    std::cout << std::string(80, '=') << std::endl;
    std::cout << std::endl << std::endl;

    // recursion function 2
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
    std::cout << std::endl << std::endl;

    // recursion function 3
    std::string first = "CAT", second = "MAN";
    std::cout << std::string(80, '=') << std::endl;
    std::cout << "Recursion 3: Prints out permutations of the first word:"
              << std::endl
              << "first word = " << first << ", second word = " << second
              << std::endl
              << std::string(80, '-') << std::endl;
    r3_permutation(first, second);
    std::cout << std::string(80, '=') << std::endl;
    std::cout << std::endl << std::endl;

    // recursion function 4
    std::cout << std::string(80, '=') << std::endl;
    std::cout << "Recursion 4: Interactively ask user to pick a number to "
                 "open successive boxes."
              << std::endl
              << std::string(80, '-') << std::endl;
    double total_boxes = r4_i_box("", 3);
    std::cout << std::endl << "total_boxes = " << total_boxes << std::endl;
    std::cout << std::string(80, '=') << std::endl;
    std::cout << std::endl << std::endl;

    // recursion function 5
    std::cout << std::string(80, '=') << std::endl;
    unsigned reciprocal_level = 3;
    std::cout << "Recursion 5: Returns the sum of reciprocals of levels"
              << std::endl
              << "reciprocal level = " << reciprocal_level << std::endl
              << std::string(80, '-') << std::endl;
    double sumover = r5_sumover(reciprocal_level);
    std::cout << "sumover = " << sumover << std::endl;
    std::cout << std::string(80, '=') << std::endl;
    std::cout << std::endl << std::endl;

    // recursion function 16
    std::cout << std::string(80, '=') << std::endl;
    unsigned low = 1, high = 10;
    std::cout << "Recursion 16: Guess the user's number between " << low
              << " to " << high << "." << std::endl
              << std::string(80, '-') << std::endl;
    double guess = r16_guess(low, high);
    std::cout << "guess = " << guess << std::endl;
    std::cout << std::string(80, '=') << std::endl;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Prints out current level, recurve, and prints out level again.
 *
 * PRE-CONDITIONS:
 *  unsigned start_level: starting recursion level (low)
 *  unsigned end_level  : ending recursion level (high)
 *
 * POST-CONDITIONS:
 *  unsigned start_level: increases towards end_level
 *
 * RETURN:
 *  none
 ******************************************************************************/
void r1_levels(unsigned start_level, unsigned end_level) {
    if(start_level > end_level) return;  // base: start is beyond end level

    // print out first sentence
    std::cout << std::string(start_level, ' ')
              << "This was written by call number " << start_level << "."
              << std::endl;

    // recurve to higher level
    r1_levels(start_level + 1, end_level);

    // print out second sentence
    std::cout << std::string(start_level, ' ')
              << "This ALSO written by call number " << start_level << "."
              << std::endl;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Prints out prefix + recursive sections.
 *  Mechanism: create new string from prefix, add a digit char (shift ASCII),
 *  add 'dot' if not last level = 1. Then, recurve using new appended string
 *  as new parameter for prefix, and reduce level by 1.
 *  Base: level is 0 and print out prefix string.
 *
 * PRE-CONDITIONS:
 *  string prefix : string to append
 *  unsigned level: recursion level
 *
 * POST-CONDITIONS:
 *  unsigned level: decreases toward zero
 *
 * RETURN:
 *  none
 ******************************************************************************/
void r2_box(std::string prefix, unsigned level) {
    if(!level) {                           // base: level 0
        std::cout << prefix << std::endl;  // print out final string
        return;
    }

    for(int i = 1; i < 10; ++i) {  // iterate all digits (0 to 9)
        std::string s = prefix;    // new string from prev string
        s += char('0' + i);        // append digit with ASCII shift
        if(level != 1) s += '.';   // append 'dot' if not last valid level

        r2_box(s, level - 1);  // recurve to lower level
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Prints permutation of first string followed by normal second string.
 *
 * PRE-CONDITIONS:
 *  string first : string for permutation changes
 *  string second: static string
 *
 * POST-CONDITIONS:
 *  string first : permutation changes
 *
 * RETURN:
 *  none
 ******************************************************************************/
void r3_permutation(std::string first, std::string second) {
    if(!first.size()) {                    // base: empty first string
        std::cout << second << std::endl;  // print out second word
        return;
    }

    // loop through all pos of first string
    for(int i = first.size() - 1; i >= 0; --i) {
        // recurve with first string's char erased at i pos and with second
        // inserted with erased char at front
        r3_permutation(std::string(first).erase(i, 1),
                       std::string(second).insert(0, 1, first[i]));
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Interactively ask user to enter a number and function recurve until number
 *  is zero.
 *
 * PRE-CONDITIONS:
 *  string first  : string to append
 *  unsigned level: recursion level, default 0
 *
 * POST-CONDITIONS:
 *  unsigned level: user input level to recurve
 *
 * RETURN:
 *  none
 ******************************************************************************/
unsigned r4_i_box(std::string prefix, unsigned level) {
    unsigned count = 0;

    std::cout << std::endl << "How many unnumbered boxes can you see? ";
    std::cin >> level;

    // base: level is 0 and return count 0
    // else: start recursion and return x count
    if(level) {
        // display numbered boxes
        std::cout << std::endl;
        for(unsigned i = 1; i <= level; ++i) {
            std::string s = prefix;  // new string from prev string
            s += char('0' + i);      // append digit with ASCII shift
            std::cout << "[" << s << "] ";
        }
        std::cout << std::endl;

        // loop through section combinations
        for(unsigned i = 1; i <= level; ++i) {
            std::string s = prefix;  // new string from prev string
            s += char('0' + i);      // append digit with ASCII shift

            std::cout << std::endl << "Opening box [" << s << "] " << std::endl;

            s += '.';  // append 'dot'

            // recurve until user input 0 level and update count
            ++count += r4_i_box(s, level);
        }
    }

    return count;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Sum of all reciprocal levels.
 *
 * PRE-CONDITIONS:
 *  unsigned level: recursion level
 *
 * POST-CONDITIONS:
 *  unsigned level: decreases toward zero
 *
 * RETURN:
 *  none
 ******************************************************************************/
double r5_sumover(unsigned level) {
    if(!level) return 0;  // base: level is zero

    return (1.0 / level) + r5_sumover(level - 1);  // return recurve sum
}

/*******************************************************************************
 * DESCRIPTION:
 *  Guess user's number by decreasing problem by half each recursion.
 *
 * PRE-CONDITIONS:
 *  unsigned low : start point
 *  unsigned high: end point
 *
 * POST-CONDITIONS:
 *  unsigned low : midpoint replacement
 *  unsigned high: midpoint replacement
 *
 * RETURN:
 *  none
 ******************************************************************************/
int r16_guess(unsigned low, unsigned high) {
    bool is_yes = false;

    if(low == high) return low;  // base: if both numbers are same, return

    // as long as low is less than high, then ask for midpoint questions
    if(low < high) {
        // ask for mid
        unsigned midpoint = (low + high) / 2.0;
        std::cout << "Is the number " << midpoint << "? ";
        std::cin >> is_yes;

        if(is_yes) return midpoint;

        // ask for less than midpoint
        std::cout << "Is the number less than " << midpoint << "? ";
        std::cin >> is_yes;

        if(is_yes)
            return r16_guess(low, midpoint - 1);  // recurve first half
        else
            return r16_guess(midpoint + 1, high);  // recurve second half
    }

    // print out inconsistent answers error
    std::cout << "Error: inconsistent answers." << std::endl;
    return -1;
}
