#ifndef SQLTOKENIZER_H
#define SQLTOKENIZER_H

#include <algorithm>     // transform()
#include "bpt_map.h"     // MMap class
#include "sql_states.h"  // SQL constants
#include "sql_token.h"   // Token class
#include "stokenizer.h"  // state_machine funcs, STokenizer

namespace sql {

class SQLTokenizer {
    enum { MAX_BLOCK = stokenizer::MAX_BUFFER };
    typedef bpt_map::MMap<std::string, std::string> Map;

public:
    // CONSTRUCTORS
    SQLTokenizer(char* buffer, std::size_t block_size = MAX_BLOCK);

    // ACCESSORS
    bool more() const;               // returns the current value of _more
    explicit operator bool() const;  // boolean conversion for extractor

    // MUTATORS
    void set_string(char* buffer);
    bool get_query(Map& map);

    // FRIENDS
    friend SQLTokenizer& operator>>(SQLTokenizer& f, SQLToken& t);

private:
    static bool _need_init;
    static int _table[MAX_ROWS][MAX_COLS];
    static std::string _keys[MAX_KEYS];

    std::size_t _block_size;      // block size to grab from ifstream
    bool _more;                   // false if last token of the last block
    SQLToken _prev_token;         // previous token that was extracted
    stokenizer::STokenizer _stk;  // STokenizer obj to tokenize current block

    void make_table(int _table[][MAX_COLS]);  // SQL's state table set up
    bool get_parse_key(int command, int prev_state, int state, int& key_code);
    SQLToken next_token();
};

}  // namespace sql

#endif  // SQLTOKENIZER_H
