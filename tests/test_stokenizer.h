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

    char s1[] = "3.14 Is.";
    int test_states[4] = {
        state_machine::STATE_DOUBLE, state_machine::STATE_SPACE,
        state_machine::STATE_ALPHA, state_machine::STATE_UNKNOWN};
    std::string test_types[4] = {"DOUBLE", "SPACE", "ALPHA", "UNKNOWN"},
                test_strings[4] = {"3.14", " ", "Is", "."};
    STokenizer stk(s1);
    Token t;
    for(int i = 0; i < 4; ++i) {
        stk >> t;
        assert(t.type() == test_states[i]);
        assert(t.type_string() == test_types[i]);
        assert(t.token_str() == test_strings[i]);
    }

    std::cout << "passed." << std::endl;
}

void test_all() {
    test_token();
    test_stokenizer();
}

}  // namespace test_stokenizer

#endif  // TEST_STOKENIZER_H
