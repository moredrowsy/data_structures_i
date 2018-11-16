/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : ftokenizer
 * DESCRIPTION : This header declares class FTokenizer. The FTokenizer class is
 *      a composition of the ifstream and STokenizer, which in turn is a
 *      composition of the state machine and Token class. The mechanism of the
 *      FTokenizer class: Extraction operator calls next_token(), which checks
 *      if STokenizer has tokens to extract and extract. If not, next_token()
 *      calls get_new_block(), which if success inserts a block string to
 *      STokenizer to extract tokens. If fail, it will set more() to false.
 *      Continuously calling the extraction operator will repeat the mechanism
 *      above until get_new_block() returns fail to more() and thus signal EOF.
 *
 * NOTE: FTokenizer must be constructed with a valid cstring file name.
 ******************************************************************************/
#ifndef FTOKENIZER_H
#define FTOKENIZER_H

#include <cassert>       // assertions
#include <fstream>       // file stream objects
#include <iostream>      // stream objects
#include "stokenizer.h"  // state_machine funcs, STokenizer
#include "token.h"       // Token class

namespace ftokenizer {

class FTokenizer {
public:
    enum { MAX_BLOCK = stokenizer::MAX_BUFFER };

    // CONSTRUCTORS
    FTokenizer(char* fname, std::size_t block_size = MAX_BLOCK);

    // ACCESSORS
    bool more() const;               // returns the current value of _more
    int pos() const;                 // returns the value of _pos
    int block_pos() const;           // returns the value of _block_pos
    explicit operator bool() const;  // boolean conversion for extractor

    // MUTATORS
    token::Token next_token();

    // FRIENDS
    friend FTokenizer& operator>>(FTokenizer& f, token::Token& t);

private:
    bool get_new_block();         // gets the new block from the file
    std::ifstream _f;             // file being tokenized
    stokenizer::STokenizer _stk;  // STokenizer obj to tokenize current block
    std::size_t _pos;             // current position in the file
    std::size_t _block_pos;       // current position in the current block
    std::size_t _block_size;      // block size to grab from ifstream
    bool _more;                   // false if last token of the last block
                                  //  has been processed and now we are at
                                  //  the end of the last block.
};

}  // namespace ftokenizer

#endif  // FTOKENIZER_H
