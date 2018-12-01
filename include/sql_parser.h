#ifndef SQLPARSER_H
#define SQLPARSER_H

#include <algorithm>        // transform()
#include <memory>           // shared_ptr
#include <string>           // string
#include "bpt_map.h"        // MMap class
#include "sql_states.h"     // SQL states
#include "sql_token.h"      // SQL type tokens for infix
#include "sql_tokenizer.h"  // SQL Tokenizer
#include "sql_typedefs.h"   // typedefs for SQL
#include "state_machine.h"  // state machine enum codes
#include "token.h"          // Token class

namespace sql {

class SQLParser {
public:
    // CONSTRUCTORS
    SQLParser(char* buffer = nullptr, std::size_t block_size = MAX_BUFFER);

    // ACCESSORS
    explicit operator bool() const;  // boolean conversion for extractor

    // MUTATORS
    void set_string(char* buffer);  // set new buffer for STokenizer
    // parse query buffer to ParseTree and infix
    bool parse_query(ParseTree& tree, QueueTokens& infix);

    // FRIENDS
    friend SQLParser& operator>>(SQLParser& f, token::Token& t);

private:
    static bool _need_init;                   // need Class initializations?
    static std::string _keys[MAX_KEYS];       // list of keys
    static std::string _types[MAX_COLS];      // list of SQL token string types
    static std::string _r_ops[STR_OPS_SIZE];  // list of relation op
    static int _table[MAX_ROWS][MAX_COLS];    // adjacency table

    std::size_t _max_buf;      // block size to grab from ifstream
    bool _more;                // false if last token of the last block
    token::Token _prev_token;  // previous token that was extracted
    SQLTokenizer _tokenizer;   // STokenizer obj to tokenize current block

    void init();                                   // init static vars
    token::Token next_token();                     // get SQL token
    void parse_token(token::Token& t);             // ID token to SQL types
    void get_keyword(token::Token& t);             // get keyword id
    void get_r_op_subtype(token::Token& t);        // get r_op id
    bool get_parse_key(int state, int& key_code);  // get parse key for map
    token_ptr get_sql_token(token::Token& t);
};

}  // namespace sql

#endif  // SQLPARSER_H
