#ifndef TEST_STOKENIZER_H
#define TEST_STOKENIZER_H

#include <cassert>
#include <iostream>
#include "../include/stokenizer.h"
#include "../include/token.h"

namespace test_stokenizer {

// test Token class
void test_token();

// test STokenizer class
void test_stokenizer();

// call all test functions
void test_all();

void test_token() {
    using namespace stokenizer;

    std::cout << "test_token: ";

    token::Token t1, t2("123456789", state_machine::STATE_DOUBLE),
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
    token::Token t;
    int i;

    // test default constructor
    assert(t.type_string() == "UNKNOWN");
    assert(t.token_str().empty() == true);
    assert(stk.done() == true);
    assert(stk.more() == false);

    // test empty string
    stk.set_string("");
    stk >> t;
    assert(t.type_string() == "UNKNOWN");
    assert(t.token_str().empty() == true);
    assert(stk.done() == true);
    assert(stk.more() == false);

    // test on more insertion calls on empty string
    stk >> t;
    assert(t.type_string() == "UNKNOWN");
    assert(t.token_str().empty() == true);
    assert(stk.done() == true);
    assert(stk.more() == false);
    stk >> t;
    assert(t.type_string() == "UNKNOWN");
    assert(t.token_str().empty() == true);
    assert(stk.done() == true);
    assert(stk.more() == false);

    // test string 01
    char s01[] = " ";
    std::string test_types_s01[] = {"SPACE"}, test_strings_s01[] = {" "};

    stk.set_string(s01);
    i = 0;
    while(stk >> t) {
        assert(t.type_string() == test_types_s01[i]);
        assert(t.token_str() == test_strings_s01[i]);
        ++i;
    }

    // test string 02
    char s02[] = "A";
    std::string test_types_s02[] = {"ALPHA"}, test_strings_s02[] = {"A"};

    stk.set_string(s02);
    i = 0;
    while(stk >> t) {
        assert(t.type_string() == test_types_s02[i]);
        assert(t.token_str() == test_strings_s02[i]);
        ++i;
    }

    // test string 03
    char s03[] = "1";
    std::string test_types_s03[] = {"DOUBLE"}, test_strings_s03[] = {"1"};

    stk.set_string(s03);
    i = 0;
    while(stk >> t) {
        assert(t.type_string() == test_types_s03[i]);
        assert(t.token_str() == test_strings_s03[i]);
        ++i;
    }

    // test string 04
    char s04[] = ".1";
    std::string test_types_s04[] = {"DOUBLE"}, test_strings_s04[] = {".1"};

    stk.set_string(s04);
    i = 0;
    while(stk >> t) {
        assert(t.type_string() == test_types_s04[i]);
        assert(t.token_str() == test_strings_s04[i]);
        ++i;
    }

    // test string 05
    char s05[] = "0.1";
    std::string test_types_s05[] = {"DOUBLE"}, test_strings_s05[] = {"0.1"};

    stk.set_string(s05);
    i = 0;
    while(stk >> t) {
        assert(t.type_string() == test_types_s05[i]);
        assert(t.token_str() == test_strings_s05[i]);
        ++i;
    }

    // test string 06
    char s06[] = "1.";
    std::string test_types_s06[] = {"DOUBLE", "PUNCT"},
                test_strings_s06[] = {"1", "."};

    stk.set_string(s06);
    i = 0;
    while(stk >> t) {
        assert(t.type_string() == test_types_s06[i]);
        assert(t.token_str() == test_strings_s06[i]);
        ++i;
    }

    // test string 07
    char s07[] = "1.0";
    std::string test_types_s07[] = {"DOUBLE"}, test_strings_s07[] = {"1.0"};

    stk.set_string(s07);
    i = 0;
    while(stk >> t) {
        assert(t.type_string() == test_types_s07[i]);
        assert(t.token_str() == test_strings_s07[i]);
        ++i;
    }

    // test string 08
    char s08[] = "  ";
    std::string test_types_s08[] = {"SPACE"}, test_strings_s08[] = {"  "};

    stk.set_string(s08);
    i = 0;
    while(stk >> t) {
        assert(t.type_string() == test_types_s08[i]);
        assert(t.token_str() == test_strings_s08[i]);
        ++i;
    }

    // test string 09
    char s09[] = "1,000!";
    std::string test_types_s09[] = {"DOUBLE", "PUNCT"},
                test_strings_s09[] = {"1,000", "!"};

    stk.set_string(s09);
    i = 0;
    while(stk >> t) {
        assert(t.type_string() == test_types_s09[i]);
        assert(t.token_str() == test_strings_s09[i]);
        ++i;
    }

    // test string 10
    char s10[] = "Account balance: $100,000.00, $100000.00, or just $100000?";
    std::string test_types_s10[] = {"ALPHA", "SPACE", "ALPHA",  "PUNCT",
                                    "SPACE", "PUNCT", "DOUBLE", "PUNCT",
                                    "SPACE", "PUNCT", "DOUBLE", "PUNCT",
                                    "SPACE", "ALPHA", "SPACE",  "ALPHA",
                                    "SPACE", "PUNCT", "DOUBLE", "PUNCT"},
                test_strings_s10[] = {"Account", " ",  "balance",    ":",
                                      " ",       "$",  "100,000.00", ",",
                                      " ",       "$",  "100000.00",  ",",
                                      " ",       "or", " ",          "just",
                                      " ",       "$",  "100000",     "?"};

    stk.set_string(s10);
    i = 0;
    while(stk >> t) {
        assert(t.type_string() == test_types_s10[i]);
        assert(t.token_str() == test_strings_s10[i]);
        ++i;
    }

    // test string 11
    char s11[] = "First line\nSecond line!";
    std::string test_types_s11[] = {"ALPHA", "SPACE", "ALPHA", "SPACE",
                                    "ALPHA", "SPACE", "ALPHA", "PUNCT"},
                test_strings_s11[] = {"First",  " ", "line", "\n",
                                      "Second", " ", "line", "!"};

    stk.set_string(s11);
    i = 0;
    while(stk >> t) {
        assert(t.type_string() == test_types_s11[i]);
        assert(t.token_str() == test_strings_s11[i]);
        ++i;
    }

    // test unknown or non-ascii characters so that it doesn't crash
    char s12[] = "£©±";
    std::string test_types_s12[] = {"UNKNOWN", "UNKNOWN", "UNKNOWN",
                                    "UNKNOWN", "UNKNOWN", "UNKNOWN"};

    stk.set_string(s12);
    i = 0;
    while(stk >> t) {
        assert(t.type_string() == test_types_s12[i]);
        ++i;
    }

    // test incorrectly formatted numbers
    char s13[] = "1,000,0 10,000,000,0!";
    std::string test_types_s13[] = {"DOUBLE", "PUNCT", "DOUBLE", "SPACE",
                                    "DOUBLE", "PUNCT", "DOUBLE", "PUNCT"},
                test_strings_s13[] = {"1,000",      ",", "0", " ",
                                      "10,000,000", ",", "0", "!"};

    stk.set_string(s13);
    i = 0;
    while(stk >> t) {
        assert(t.type_string() == test_types_s13[i]);
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
