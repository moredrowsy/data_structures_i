/*******************************************************************************
 * AUTHOR        : Thuan Tang
 * ID            : 00991588
 * CLASS         : CS008
 * EXERCISE 04   : Recursive Brute Force
 * DESCRIPTION   : Simple exercise to demonstrate brute force combination
 ******************************************************************************/
#include <iostream>  // output stream
#include <string>    // string

void brute_force(std::string &list, std::string str, int level, int &count) {
    if(level > 0) {
        for(std::size_t i = 0; i < list.size(); ++i) {
            brute_force(list, str + list[i], level - 1, count);
        }
    } else {
        std::cout << str << '\n';
        ++count;
    }
}

int main() {
    int count = 0;     // hold the number of combinations
    std::string list;  // list for characters to rearrange

    // add numerals to list
    for(char c = '0'; c <= '9'; ++c) list += c;

    // add upper case alphabet
    for(char c = 'A'; c <= 'Z'; ++c) list += c;

    // add lower case alphabet
    for(char c = 'a'; c <= 'z'; ++c) list += c;

    brute_force(list, std::string(), 4, count);
    std::cout << "Permutations: " << count << std::endl;

    return 0;
}
