/*******************************************************************************
 * AUTHOR        : Thuan Tang
 * ID            : 00991588
 * CLASS         : CS008
 * ASSIGNMENT 04 : File Tokenizer
 * DESCRIPTION   : This program illustrates the FTokenizer class, which is
 *      built upon the STokenizer class. It will take in a file name via its
 *      constructor to open a valid input file stream. From there, it will
 *      extract valid tokens from the file. See FTokenizer header for detailed
 *      mechanism.
 ******************************************************************************/
#include <iostream>                 // io stream objects
#include <string>                   // string objects
#include "../include/ftokenizer.h"  // Token, FTokenizer class
#include "../include/token.h"       // Token class

// continuously extract tokens from file
void extract_file(char fname[]);

int main() {
    char file_name[] = "solitude.txt";
    extract_file(file_name);

    return 0;
}

/*******************************************************************************
 * DESCRIPTION:
 *  This function implements token extraction from a file stream. It will
 *  create two classes: FTokenizer and Token. Class Token will hold the string
 *  token and token type. FTokenizer will get a token from file and insert it
 *  into the Token object. Output of token is done via the extraction operator
 *  on the Token object. Token extraction will loop when FTokenizer returns
 *  false on extraction, which signals the end of file.
 *
 * PRE-CONDITIONS:
 *  char fname[]: cstring file name
 *
 * POST-CONDITIONS:
 *  Output for token extractions
 *
 * RETURN:
 *  none
 ******************************************************************************/
void extract_file(char fname[]) {
    using namespace ftokenizer;

    int token_count = 0;
    FTokenizer ftk(fname);
    token::Token t;

    while(ftk >> t) {
        if(t.type_string() == "ALPHA") {
            ++token_count;
            std::cout << std::setw(10) << std::left << token_count
                      << std::setw(3) << ":" << std::setw(25) << std::left
                      << t.token_str() << t.type_string() << std::endl;
        }
    }
    std::cout << "Tokens Found: " << token_count << std::endl;
    std::cout << "==========" << std::endl;
}
