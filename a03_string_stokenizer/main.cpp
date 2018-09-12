/*******************************************************************************
 * AUTHOR        : Thuan Tang
 * ID            : 00991588
 * CLASS         : CS008
 * ASSIGNMENT 03 : String Tokenizer
 * DESCRIPTION   :
 ******************************************************************************/
#include <iostream>
#include "../include/stokenizer.h"

void extract_tokens(char s[]);

int main() {
    char s1[] = "0.315";
    extract_tokens(s1);

    return 0;
}

void extract_tokens(char s[]) {
    STokenizer::STokenizer stk(s);
    Token::Token t("something", 0);
    std::cout << t;
    std::cout << std::endl;

    stk >> t;
    std::cout << t;
    std::cout << std::endl;
}
