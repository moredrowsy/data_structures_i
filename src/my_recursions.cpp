#include "../include/my_recursions.h"

namespace my_recursions {

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

    // print out first sentence before recurve
    std::cout << std::string(start_level, ' ')
              << "This was written by call number " << start_level << "."
              << std::endl;

    // recurve to higher level
    r1_levels(start_level + 1, end_level);

    // print out second sentence after recurve
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

}  // namespace my_recursions
