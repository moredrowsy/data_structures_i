#include "../include/ftokenizer.h"  // ftokenizer declarations

namespace ftokenizer {

FTokenizer::FTokenizer(char* fname) {
    _f.open(fname);  // open file stream with fname

    // assert file name is correct
    assert(static_cast<bool>(_f) == true);

    _pos = 0;
    _blockPos = 0;
    _more = true;
}

FTokenizer::~FTokenizer() {
    if(_f.is_open()) {
        _f.close();
    }
}

bool FTokenizer::more() const { return _more; }

int FTokenizer::pos() const { return _pos; }

int FTokenizer::block_pos() const { return _blockPos; }

}  // namespace ftokenizer
