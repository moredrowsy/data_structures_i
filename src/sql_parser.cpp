#include "../include/sql_parser.h"

namespace sql {

// STATIC VARIABLES
bool SQLParser::_need_init = true;
int SQLParser::_table[MAX_ROWS][MAX_COLS];
std::string SQLParser::_keys[MAX_KEYS];
std::string SQLParser::_types[MAX_COLS];

SQLParser::SQLParser(char *buffer, std::size_t block_size)
    : _block_size(block_size), _more(false), _tokenizer(buffer, _block_size) {
    assert(_block_size <= MAX_BLOCK);
    if(_need_init) init();
}

bool SQLParser::more() const { return (bool)_tokenizer; }

SQLParser::operator bool() const { return (bool)_tokenizer; }

void SQLParser::set_string(char *buffer) { _tokenizer.set_string(buffer); }

bool SQLParser::get_query(bpt_map::MMap<std::string, std::string> &map) {
    bool is_good = false;  // query's success
    int state = CMD_START;
    int key_code;

    token::Token t = next_token();  // get SQL Token from STokenizer

    while(_tokenizer && t.type() > -1 && _table[state][t.type()] != -1) {
        state = _table[state][t.type()];
        if(is_success(_table, state)) is_good = true;  // log query success

        if(get_parse_key(state, key_code))       // if valid code
            map[_keys[key_code]] += t.string();  // add to map

        t = next_token();  // get next SQL Token
    }

    if(!is_good) map.clear();  // clear map if query fails

    return is_good;
}

void SQLParser::init() {
    init_keys(_keys);
    init_types(_types);
    init_table(_table);
    mark_table_command(_table);
    mark_table_select(_table);
    print_table(_table);

    _need_init = false;
}

bool SQLParser::get_parse_key(int state, int &key_code) {
    bool is_valid = true;

    if(state == CMD_SELECT)
        key_code = COMMAND;
    else if(state == SELECT_STRING || state == SELECT_ASTERISK)
        key_code = FIELDS;
    else if(state == SELECT_TABLE)
        key_code = TABLE;
    else
        is_valid = false;

    return is_valid;
}

token::Token SQLParser::next_token() {
    // extract no SPACE tokens
    token::Token t(" ", state_machine::STATE_SPACE);
    while(t.type() == state_machine::STATE_SPACE) _tokenizer >> t;

    // transform to comparison string with all caps
    std::string cmp_str = t.string();
    std::transform(cmp_str.begin(), cmp_str.end(), cmp_str.begin(), ::toupper);

    if(cmp_str == _types[SELECT]) {
        t.set_type(SELECT);
        t.set_string(std::move(cmp_str));
    } else if(cmp_str == _types[FROM]) {
        t.set_type(FROM);
        t.set_string(std::move(cmp_str));
    } else if(t.type() == state_machine::STATE_COMMA)
        t.set_type(COMMA);
    else if(t.type() == state_machine::STATE_STAR)
        t.set_type(ASTERISK);
    else if(t.type() == state_machine::STATE_IDENT)
        t.set_type(IDENT);
    else if(t.type() == state_machine::STATE_VALUE)
        t.set_type(VALUE);
    else
        t.set_type(VALUE);

    return t;
}

SQLParser &operator>>(SQLParser &f, token::Token &t) {
    t = f.next_token();
    return f;
}

}  // namespace sql
