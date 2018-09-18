#include <iostream>                 // io stream objects
#include "../include/ftokenizer.h"  // Token, FTokenizer class

void extract_file();

int main() {
    extract_file();

    return 0;
}

void extract_file() {
    using namespace ftokenizer;

    int token_count = 0;
    char fname[] = "solitude.txt";
    FTokenizer ftk(fname);
    stokenizer::Token t;

    while(ftk >> t) {
        if(t.type_string() == "ALPHA") {
            ++token_count;
            std::cout << std::setw(10) << std::left << token_count
                      << std::setw(3) << ":" << std::setw(25) << std::left
                      << t.token_str() << t.type_string() << std::endl;
        }
    }
}
