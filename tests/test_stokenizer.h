#ifndef TEST_STOKENIZER_H
#define TEST_STOKENIZER_H

#include <cassert>
#include <iostream>
#include "../include/stokenizer.h"

namespace test_stokenizer {

// test Token class
void test_token();

// test STokenizer class
void test_stokenizer();

// call all test functions
void test_token();

void test_token() {
    using namespace stokenizer;

    std::cout << "test_token: ";

    Token t1, t2("123456789", state_machine::STATE_DOUBLE),
        t3(" ", state_machine::STATE_SPACE),
        t4("ABCabc", state_machine::STATE_ALPHA), t5("", 1000);

    assert(t1.type() == state_machine::STATE_UNKNOWN);
    assert(t1.type_string() == "UNKNOWN");
    assert(t1.token_str() == "");

    assert(t2.type() == state_machine::STATE_DOUBLE);
    assert(t2.type_string() == "DOUBLE");
    assert(t2.token_str() == "123456789");

    assert(t3.type() == state_machine::STATE_SPACE);
    assert(t3.type_string() == "SPACE");
    assert(t3.token_str() == " ");

    assert(t4.type() == state_machine::STATE_ALPHA);
    assert(t4.type_string() == "ALPHA");
    assert(t4.token_str() == "ABCabc");

    assert(t5.type() == 1000);
    assert(t5.type_string() == "ERROR");
    assert(t5.token_str() == "");

    std::cout << "passed." << std::endl;
}

void test_stokenizer() {
    using namespace stokenizer;

    std::cout << "test_stokenizer: ";
    STokenizer stk;
    Token t;
    int i;

    // test empty string
    stk.set_string("");
    stk >> t;
    assert(t.type_string() == "UNKNOWN");
    assert(t.token_str().empty() == true);

    // test string 0
    char s0[] = "1A? ";
    std::string test_types_s0[] = {"DOUBLE", "ALPHA", "PUNCT", "SPACE"},
                test_strings_s0[] = {"1", "A", "?", " "};

    stk.set_string(s0);
    i = 0;
    while(stk >> t) {
        assert(t.type_string() == test_types_s0[i]);
        assert(t.token_str() == test_strings_s0[i]);
        ++i;
    }

    // test string 1
    char s1[] = "Account balance: $100,000.00, $100000.00, or just $100000?";
    std::string test_types_s1[] = {"ALPHA", "SPACE", "ALPHA",  "PUNCT",
                                   "SPACE", "PUNCT", "DOUBLE", "PUNCT",
                                   "SPACE", "PUNCT", "DOUBLE", "PUNCT",
                                   "SPACE", "ALPHA", "SPACE",  "ALPHA",
                                   "SPACE", "PUNCT", "DOUBLE", "PUNCT"},
                test_strings_s1[] = {"Account", " ",  "balance",    ":",
                                     " ",       "$",  "100,000.00", ",",
                                     " ",       "$",  "100000.00",  ",",
                                     " ",       "or", " ",          "just",
                                     " ",       "$",  "100000",     "?"};

    stk.set_string(s1);
    i = 0;
    while(stk >> t) {
        assert(t.type_string() == test_types_s1[i]);
        assert(t.token_str() == test_strings_s1[i]);
        ++i;
    }

    std::cout << "passed." << std::endl;
}

void test_all() {
    test_token();
    test_stokenizer();
}

}  // namespace test_stokenizer

#endif  // TEST_STOKENIZER_H
