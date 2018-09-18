#ifndef TEST_FTOKENIZER_H
#define TEST_FTOKENIZER_H

#include <fstream>
#include "../include/ftokenizer.h"

namespace test_ftokenizer {

// test FTokenizer class
void test_ftokenizer();

// call all test functions
void test_all();

void test_ftokenizer() {
    using namespace ftokenizer;

    std::cout << "test_ftokenizer: ";

    // get test_ftokenizer_file.txt length
    std::ifstream fin("test_ftokenizer_file.txt");
    fin.seekg(0, fin.end);
    int length = fin.tellg();
    fin.close();

    // create FTokenizer with test_ftokenizer_file.txt
    int token_count = 0;
    FTokenizer ftk("test_ftokenizer_file.txt");
    stokenizer::Token t;

    while(ftk >> t) {
        ++token_count;
    }

    assert(token_count > 140000);  // token_count should be over 140,000
    assert(ftk.pos() == length);  // ftk's ending file pos should == file length

    std::cout << "passed." << std::endl;
}

void test_all() { test_ftokenizer(); }

}  // namespace test_ftokenizer

#endif  // TEST_FTOKENIZER_H
