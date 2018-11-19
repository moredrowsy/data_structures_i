#include "../include/sql_tokenizer.h"

namespace sql {

// STATIC VARIABLES
bool SQLTokenizer::_need_init = true;
int SQLTokenizer::_table[MAX_ROWS][MAX_COLS];
std::string SQLTokenizer::_keys[MAX_KEYS];
std::string SQLTokenizer::_types[MAX_COLS];

SQLTokenizer::SQLTokenizer(char *buffer, std::size_t block_size)
    : _block_size(block_size), _more(false), _stk(buffer, _block_size) {
    assert(_block_size <= MAX_BLOCK);
    if(_need_init) init();
}

bool SQLTokenizer::more() const { return (bool)_stk; }

SQLTokenizer::operator bool() const { return (bool)_stk; }

void SQLTokenizer::set_string(char *buffer) { _stk.set_string(buffer); }

bool SQLTokenizer::get_query(bpt_map::MMap<std::string, std::string> &map) {
    bool is_good = false;  // query's success
    int state = CMD_START;
    int key_code;

    SQLToken t = next_token();  // get SQL Token from STokenizer

    while(_stk && t.type() > -1 && _table[state][t.type()] != -1) {
        state = _table[state][t.type()];
        if(is_success(_table, state)) is_good = true;  // log query success

        if(get_parse_key(state, key_code))       // if valid code
            map[_keys[key_code]] += t.string();  // add to map

        t = next_token();  // get next SQL Token
    }

    if(!is_good) map.clear();  // clear map if query fails

    return is_good;
}

void SQLTokenizer::init() {
    init_keys(_keys);
    init_types(_types);
    init_table(_table);
    mark_table_command(_table);
    mark_table_select(_table);
    print_table(_table);

    _need_init = false;
}

bool SQLTokenizer::get_parse_key(int state, int &key_code) {
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

SQLToken SQLTokenizer::next_token() {
    // extract no SPACE tokens
    SQLToken t(" ", state_machine::STATE_SPACE);
    while(t.type() == state_machine::STATE_SPACE) _stk >> t;

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
    else if(t.type() == state_machine::STATE_QUOTE_S)
        t.set_type(QUOTE_S);
    else if(t.type() == state_machine::STATE_QUOTE_D)
        t.set_type(QUOTE_D);
    else
        t.set_type(STRING);

    return t;
}

SQLTokenizer &operator>>(SQLTokenizer &f, SQLToken &t) {
    t = f.next_token();
    return f;
}

}  // namespace sql
