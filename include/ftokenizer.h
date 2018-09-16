#ifndef FTOKENIZER_H
#define FTOKENIZER_H

#include <cassert>       // assertions
#include <fstream>       // file stream objects
#include <iostream>      // stream objects
#include "stokenizer.h"  // state_machine funcs, STokenizer, Token classes

namespace ftokenizer {

class FTokenizer {
public:
    // CLASS CONSTANTS
    const int MAX_BLOCK = stokenizer::MAX_BUFFER;

    // CONSTRUCTORS
    FTokenizer(char* fname);

    // DESTRUCTOR
    ~FTokenizer();

    // ACCESSORS
    bool more() const;      // returns the current value of _more
    int pos() const;        // returns the value of _pos
    int block_pos() const;  // returns the value of _blockPos

    // MUTATORS
    stokenizer::Token next_token();

    // FRIENDS
    friend FTokenizer& operator>>(FTokenizer& f, stokenizer::Token& t);

private:
    bool get_new_block();         // gets the new block from the file
    std::ifstream _f;             // file being tokenized
    stokenizer::STokenizer _stk;  // STokenizer obj to tokenize current block
    int _pos;                     // current position in the file
    int _blockPos;                // current position in the current block
    bool _more;                   // false if last token of the last block
                                  //  has been processed and now we are at
                                  //  the end of the last block.
};

}  // namespace ftokenizer

#endif  // FTOKENIZER_H
