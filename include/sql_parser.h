#ifndef SQLPARSER_H
#define SQLPARSER_H

#include <algorithm>        // transform()
#include <string>           // string
#include "bpt_map.h"        // MMap class
#include "sql_states.h"     // SQL states
#include "sql_tokenizer.h"  // SQL Tokenizer
#include "token.h"          // Token class

namespace sql {

class SQLParser {
public:
    typedef bpt_map::MMap<std::string, std::string> Map;

    // CONSTRUCTORS
    SQLParser(char* buffer = nullptr, std::size_t block_size = MAX_BUFFER);

    // ACCESSORS
    explicit operator bool() const;  // boolean conversion for extractor

    // MUTATORS
    void set_string(char* buffer);  // set new buffer for STokenizer
    bool get_query(Map& map);       // parse query buffer to Map

    // FRIENDS
    friend SQLParser& operator>>(SQLParser& f, token::Token& t);

private:
    static bool _need_init;                 // need Class initializations?
    static std::string _keys[MAX_KEYS];     // list of keys
    static std::string _types[MAX_COLS];    // list of SQL token string types
    static int _table[MAX_ROWS][MAX_COLS];  // adjacency table

    std::size_t _max_buf;      // block size to grab from ifstream
    bool _more;                // false if last token of the last block
    token::Token _prev_token;  // previous token that was extracted
    SQLTokenizer _tokenizer;   // STokenizer obj to tokenize current block

    void init();                                   // init static vars
    bool get_parse_key(int state, int& key_code);  // get parse key
    void parse_token(token::Token& t);
    token::Token next_token();  // get SQL token
};

}  // namespace sql

#endif  // SQLPARSER_H
