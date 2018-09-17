/*******************************************************************************
 * AUTHOR        : Thuan Tang
 * ID            : 00991588
 * CLASS         : CS008
 * ASSIGNMENT 03 : String Tokenizer
 * DESCRIPTION   : This program will use the class Token and STokenizer, which
 *      uses the state_machine to extract a valid token from its rule in the
 *      adjacency matrix. Class Token holds string and string type states.
 *      Class STokenizer will get a token and insert a string and type to the
 *      Token object. Extraction on the Token object will output the string
 *      type and string token.
 ******************************************************************************/
#include <iomanip>
#include <iostream>
#include "../include/stokenizer.h"

// continuously extract tokens from char array
void extract_tokens(char s[]);

int main() {
    const int TEST_SIZE = 9;
    char *test_str[TEST_SIZE] = {"",   " ",   "!",   "A",     "1",
                                 ".1", "0.1", "1.0", "1,000!"};

    for(int i = 0; i < TEST_SIZE; ++i) {
        std::cout << std::string(80, '=') << std::endl;
        std::cout << "Starting extraction..." << std::endl << std::endl;
        std::cout << "Buffer string is (excluding pipe symbol):" << std::endl
                  << "|" << test_str[i] << "|" << std::endl
                  << std::endl;
        extract_tokens(test_str[i]);
        std::cout << std::endl << "Extraction done." << std::endl;
        std::cout << std::string(80, '=') << std::endl;
        std::cout << std::endl << std::endl;
    }

    char s1[] =
        "It was October 17th. and pi was still 3.14 and so lost $4,000!";
    std::cout << std::string(80, '=') << std::endl;
    std::cout << "Starting extraction..." << std::endl << std::endl;
    std::cout << "Buffer string is (excluding pipe symbol):" << std::endl
              << "|" << s1 << "|" << std::endl
              << std::endl;
    extract_tokens(s1);
    std::cout << std::endl << "Extraction done." << std::endl;
    std::cout << std::string(80, '=') << std::endl;

    return 0;
}

/*******************************************************************************
 * DESCRIPTION:
 *  This function implements token extraction from a cstring. It will create
 *  two classes: STokenizer and Token. Class Token will hold the string token
 *  and token type. STokenizer will get a token from cstring and insert it into
 *  the Token object. Output of token is done via the extraction operator on
 *  the Token object. Token extraction will loop until end of cstring.
 *
 * PRE-CONDITIONS:
 *  char s[]: cstring array for input string
 *
 * POST-CONDITIONS:
 *  Output for token extractions
 *
 * RETURN:
 *  none
 ******************************************************************************/
void extract_tokens(char s[]) {
    using namespace stokenizer;

    STokenizer stk(s);
    Token t;

    std::cout << std::setw(10) << std::right << "TYPE"
              << " " << std::left << "TOKEN" << std::endl
              << std::string(10, '-') << " " << std::string(10, '-')
              << std::endl;

    while(stk >> t) {
        std::cout << std::setw(10) << std::right << t.type_string() << " "
                  << std::left << t << std::endl;
    }
}
