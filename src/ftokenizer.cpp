#include "../include/ftokenizer.h"  // ftokenizer declarations

namespace ftokenizer {

FTokenizer::FTokenizer(char* fname) {
    _f.open(fname);  // open file stream with fname

    // assert file name is correct
    assert(_f);

    _pos = 0;
    _blockPos = 0;
    _more = true;
}

bool FTokenizer::more() const { return _more; }

int FTokenizer::pos() const { return _pos; }

int FTokenizer::block_pos() const { return _blockPos; }

FTokenizer::operator bool() const { return more(); }

stokenizer::Token FTokenizer::next_token() {
    stokenizer::Token t;

    if(_stk.more()) {
        _stk >> t;
    } else {
        if(get_new_block()) {
            _stk >> t;
        } else {
            _more = false;
        }
    }

    // update blockPos
    _blockPos += t.token_str().size();

    return t;
}

FTokenizer& operator>>(FTokenizer& f, stokenizer::Token& t) {
    t = f.next_token();

    return f;
}

bool FTokenizer::get_new_block() {
    // get a block of text from file and add NUL terminate
    char block[MAX_BLOCK];
    _f.read(block, MAX_BLOCK - 1);
    block[_f.gcount()] = '\0';

    if(_f.gcount() > 0) {
        // add block to STokenizer
        _stk.set_string(block);

        // pos variables not used in class but for info purposes
        _pos += _f.gcount();
        _blockPos = 0;
    }

    return _f.gcount() > 0 ? true : false;
}

}  // namespace ftokenizer
