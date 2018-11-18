#ifndef SQLTOKENIZER_H
#define SQLTOKENIZER_H

#include <algorithm>     // transform()
#include "sql_consts.h"  // SQL constants
#include "sql_token.h"   // Token class
#include "stokenizer.h"  // state_machine funcs, STokenizer

namespace sql {

class SQLTokenizer {
    enum { MAX_BLOCK = stokenizer::MAX_BUFFER };

public:
    // CONSTRUCTORS
    SQLTokenizer(char* buffer, std::size_t block_size = MAX_BLOCK);

    // ACCESSORS
    bool more() const;               // returns the current value of _more
    explicit operator bool() const;  // boolean conversion for extractor

    // FRIENDS
    friend SQLTokenizer& operator>>(SQLTokenizer& f, token::Token& t);

private:
    std::size_t _block_size;      // block size to grab from ifstream
    bool _more;                   // false if last token of the last block
    token::Token _prev_token;     // previous token that was extracted
    stokenizer::STokenizer _stk;  // STokenizer obj to tokenize current block

    token::Token parse();
};

}  // namespace sql

#endif  // SQLTOKENIZER_H
