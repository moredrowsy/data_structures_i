#include "../include/sql_tokenizer.h"

namespace sql {
// STATIC VARIABLES
bool SQLTokenizer::_need_init = true;
int SQLTokenizer::_table[MAX_ROWS][MAX_COLS];
std::string SQLTokenizer::_keys[MAX_KEYS];

SQLTokenizer::SQLTokenizer(char *buffer, std::size_t block_size)
    : _block_size(block_size), _more(false), _stk(buffer, _block_size) {
    assert(_block_size <= MAX_BLOCK);
    if(_need_init) make_table(_table);
}

bool SQLTokenizer::more() const { return (bool)_stk; }

SQLTokenizer::operator bool() const { return (bool)_stk; }

void SQLTokenizer::set_string(char *buffer) { _stk.set_string(buffer); }

bool SQLTokenizer::get_query(bpt_map::MMap<std::string, std::string> &map) {
    bool is_good = false;  // query's success
    int state = STATE_COMMAND, prev_state;
    int command, key_code;

    SQLToken t = next_token();
    command = t.type();

    while(_stk && t.type() > -1 && _table[state][t.type()] != -1) {
        prev_state = state;
        state = _table[state][t.type()];
        if(is_success(_table, state)) is_good = true;

        if(get_parse_key(command, prev_state, state, key_code))
            map[_keys[key_code]] += t.string();

        t = next_token();
    }

    if(!is_good) map.clear();

    return is_good;
}

void SQLTokenizer::make_table(int _table[][MAX_COLS]) {
    init_table(_table);
    init_keys(_keys);
    mark_table_command(_table, STATE_COMMAND);
    mark_table_select(_table, STATE_SELECT);
    print_table(_table);

    _need_init = false;
}

bool SQLTokenizer::get_parse_key(int command, int prev_state, int state,
                                 int &key_code) {
    bool is_valid = true;
    if(prev_state == STATE_COMMAND)
        key_code = COMMAND;
    else if(command == SELECT) {
        // FIELDS
        if((prev_state == SELECT_START) &&
           (state == SELECT_STRING || state == SELECT_ASTERISK))
            key_code = FIELDS;
        else if(prev_state == SELECT_COMMA && state == SELECT_STRING)
            key_code = FIELDS;
        // TABLE
        else if(prev_state == SELECT_FROM && state == SELECT_END)
            key_code = TABLE;
        else
            is_valid = false;
    } else
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

    if(cmp_str == "SELECT") {
        t.set_type(SELECT);
        t.set_string(std::move(cmp_str));
    } else if(cmp_str == "FROM") {
        t.set_type(FROM);
        t.set_string(std::move(cmp_str));
    } else if(cmp_str == ",")
        t.set_type(COMMA);
    else if(cmp_str == "*")
        t.set_type(ASTERISK);
    else if(cmp_str == "'" || cmp_str == "\"")
        t.set_type(QUOTE);
    else
        t.set_type(STRING);

    return t;
}

SQLTokenizer &operator>>(SQLTokenizer &f, SQLToken &t) {
    t = f.next_token();
    return f;
}

}  // namespace sql
