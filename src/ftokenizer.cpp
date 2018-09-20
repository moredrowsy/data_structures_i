#include "../include/ftokenizer.h"  // ftokenizer declarations

namespace ftokenizer {

/*******************************************************************************
 * DESCRIPTION:
 *  Contructor that initializes ifstream to argument and performs assertion on
 *  ifstream.
 *
 * PRE-CONDITIONS:
 *  char* fname: cstring for valid file name
 *
 * POST-CONDITIONS:
 *  initialized member variables
 *
 * RETURN:
 *  none
 ******************************************************************************/
FTokenizer::FTokenizer(char* fname) {
    _f.open(fname);  // open file stream with fname
    assert(_f);      // assert file name is correct
    _pos = 0;
    _block_pos = 0;
    _more = false;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns more boolean, which represents available tokens in file stream.
 *
 * PRE-CONDITIONS:
 *  bool _more: state for available tokens in ifstream
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  boolean
 ******************************************************************************/
bool FTokenizer::more() const { return _more; }

/*******************************************************************************
 * DESCRIPTION:
 *  Returns current position in file stream.
 *
 * PRE-CONDITIONS:
 *  int _pos: position
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  int
 ******************************************************************************/
int FTokenizer::pos() const { return _pos; }

/*******************************************************************************
 * DESCRIPTION:
 *  Returns current position in block.
 *
 * PRE-CONDITIONS:
 *  int _block_pos: position
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  int
 ******************************************************************************/
int FTokenizer::block_pos() const { return _block_pos; }

/*******************************************************************************
 * DESCRIPTION:
 *  Explicit operator bool() overloading, such as in the case of using
 *  extraction operators. Ex: while(ftokenizer >> token) statements.
 *
 * PRE-CONDITIONS:
 *  more(): state for available tokens in ifstream
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  boolean
 ******************************************************************************/
FTokenizer::operator bool() const { return more(); }

/*******************************************************************************
 * DESCRIPTION:
 *  Performs token extraction via _stk >> t. If _stk has no tokens to extract
 *  from current buffer string, then call get_new_block to get a new block of
 *  string from file and set it to _stk to extract more tokens.
 *
 * PRE-CONDITIONS:
 *  STokenizer _stk : provides tokens extraction from cstring
 *  get_new_block(): get new block and set new block cstring to _stk
 *
 * POST-CONDITIONS:
 *  _more: true when get_new_block() succeeds; else false b/c of EOF.
 *
 * RETURN:
 *  Token
 ******************************************************************************/
stokenizer::Token FTokenizer::next_token() {
    // if _stk has no tokens to extract, get new block to set block to _stk
    if(_stk.done()) {
        _more = get_new_block();
    }

    stokenizer::Token t;
    _stk >> t;
    _block_pos += t.token_str().size();  // update block pos

    return t;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Calls next_token to extract token from file.
 *
 * PRE-CONDITIONS:
 *  FTokenizer& f: FTokenizer obj for extraction
 *  Token& t     : holds Token strings and type
 *
 * POST-CONDITIONS:
 *  New token is extracted if next_token() success
 *
 * RETURN:
 *  FTokenizer for extraction  chaining
 ******************************************************************************/
FTokenizer& operator>>(FTokenizer& f, stokenizer::Token& t) {
    t = f.next_token();

    return f;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Reads a block of text from file. Read success via gcount(), which returns
 *  number of character extracted from input stream.
 *
 * PRE-CONDITIONS:
 *  ifstream _f: input file stream for reading
 *
 * POST-CONDITIONS:
 *  Cstring block extraction if valid
 *
 * RETURN:
 *  boolean for reading block from ifstream
 ******************************************************************************/
bool FTokenizer::get_new_block() {
    // get block from file
    char* block = new char[MAX_BLOCK];  // allocate char array block on heap
    _f.read(block, MAX_BLOCK - 1);      // read block
    block[_f.gcount()] = '\0';          // add NUL terminate

    // add block to stk when gcount has valid char extracted in block
    if(_f.gcount()) {
        _stk.set_string(block);  // set cstring to stk
        _pos += _f.gcount();     // update position in file
        _block_pos = 0;          // reset block position
    }

    delete[] block;  // delete char array, stk already has copy of cstring

    return _f.gcount();
}

}  // namespace ftokenizer
