#include "../include/sql_parser.h"

namespace sql {

// STATIC VARIABLES
bool SQLParser::_need_init = true;
int SQLParser::_table[MAX_ROWS][MAX_COLS];
std::string SQLParser::_keys[MAX_KEYS];
std::string SQLParser::_r_ops[R_OPS_SIZE];
std::string SQLParser::_types[MAX_COLS];

/*******************************************************************************
 * DESCRIPTION:
 *  Default constructor. Calls init for class static variables if _need_init is
 *  true.
 *
 * PRE-CONDITIONS:
 *  char *buffer       : buffer input
 *  std::size_t max_buf: max size of buffer
 *
 * POST-CONDITIONS:
 *  initialized member variables
 *
 * RETURN:
 *  none
 ******************************************************************************/
SQLParser::SQLParser(char *buffer, std::size_t max_buf)
    : _max_buf(max_buf), _more(false), _tokenizer(buffer, _max_buf) {
    assert(_max_buf <= MAX_BUFFER);
    if(_need_init) init();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Explicit bool overload that checks if tokenizer's buffer still have tokens
 *  to extract.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
SQLParser::operator bool() const { return (bool)_tokenizer; }

/*******************************************************************************
 * DESCRIPTION:
 *  Set new buffer to tokenizer.
 *
 * PRE-CONDITIONS:
 *  char *buffer       : buffer input
 *
 * POST-CONDITIONS:
 *  SQLTokenizer _tokenizer: internal buffer changed
 *
 * RETURN:
 *  none
 ******************************************************************************/
void SQLParser::set_string(char *buffer) { _tokenizer.set_string(buffer); }

/*******************************************************************************
 * DESCRIPTION:
 *  Parses the query and returns a valid map with SQL tokens; else if query is
 *  not valid, returns an empty map.
 *
 * PRE-CONDITIONS:
 *  Map &map: multip map by ref
 *
 * POST-CONDITIONS:
 *  Map &map: multip map populated with valid Tokens or else empty
 *
 * RETURN:
 *  bool
 ******************************************************************************/
bool SQLParser::parse_query(Map &map) {
    bool is_good = false;  // query's success
    int state = CMD_START;
    int key_code;

    token::Token t = next_token();  // get SQL Token from STokenizer

    while(_tokenizer && t.type() > -1 && _table[state][t.type()] != -1) {
        state = _table[state][t.type()];
        is_good = is_success(_table, state);  // log query success

        if(get_parse_key(state, key_code))       // if valid code
            map[_keys[key_code]] += t.string();  // add to map

        t = next_token();  // get next SQL Token
    }

    if(!is_good) map.clear();  // clear map if query fails

    return is_good;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Initializes class static variables.
 *
 * PRE-CONDITIONS:
 *  int _need_init := true
 *
 * POST-CONDITIONS:
 *  int _need_init := false
 *
 * RETURN:
 *  none
 ******************************************************************************/
void SQLParser::init() {
    init_keys(_keys);
    init_types(_types);
    init_r_ops(_r_ops);
    init_table(_table);
    mark_table_command(_table);
    mark_table_create(_table);
    mark_table_insert(_table);
    mark_table_select(_table);

    _need_init = false;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Get the next valid token and converts it to an SQL type token, which have
 *  the type ID associated with the SQL adjacency table values.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  token::Token: token with SQL type id
 ******************************************************************************/
token::Token SQLParser::next_token() {
    // extract until non-whitespace token
    token::Token t("", state_machine::STATE_SPACE);
    while(t.type() == state_machine::STATE_SPACE) _tokenizer >> t;

    parse_token(t);

    return t;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Parses token to SQL type token. SQL type tokens have Token's type that
 *  corresponds to SQL adjacency table and sub type for specifics.
 *
 * PRE-CONDITIONS:
 *  token::Token &t: extracted token from SQL Tokenizer
 *
 * POST-CONDITIONS:
 *  token::Token &t: type and sub_type changed to SQL type IDs
 *
 * RETURN:
 *  void
 ******************************************************************************/
void SQLParser::parse_token(token::Token &t) {
    // transform to comparison string with all caps
    std::string cmp_str = t.string();
    std::transform(cmp_str.begin(), cmp_str.end(), cmp_str.begin(), ::toupper);

    if(t.type() == state_machine::STATE_IDENT) {
        // check if IDENT is a keyword
        if(t.sub_type() == state_machine::STATE_IDENT_NORM) {
            t.set_type(get_keyword(cmp_str));
            t.set_string(std::move(cmp_str));
        } else
            t.set_type(IDENT);
    } else if(t.type() == state_machine::STATE_COMMA)
        t.set_type(COMMA);
    else if(t.type() == state_machine::STATE_STAR)
        t.set_type(ASTERISK);
    else if(t.type() == state_machine::STATE_R_OP)
        get_r_op_subtype(t);
    else
        t.set_type(VALUE);  // VALUE w/o S
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns a keyword type id (_types's col) if it finds a keyword in _types
 *  array; else returns a default IDENT id.
 *
 * PRE-CONDITIONS:
 *  token::Token &t: extracted token from SQL Tokenizer
 *
 * POST-CONDITIONS:
 *  token::Token &t: type and sub_type changed to SQL type IDs
 *
 * RETURN:
 *  void
 ******************************************************************************/
int SQLParser::get_keyword(const std::string &cmp) {
    for(int col = 0; col < MAX_COLS; ++col)
        if(cmp == _types[col]) {
            if(col == AND || col == OR)
                return L_OPS;  // return if key is logical operators
            else
                return col;
        }
    return IDENT;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Set a relation op subtype id if it matches a value from the _r_operators
 *  array, except for equality ("==") operator. If not found or is equality op,
 *  then set to id VALUE.
 *
 * PRE-CONDITIONS:
 *  token::Token &t: extracted token from SQL Tokenizer
 *
 * POST-CONDITIONS:
 *  token::Token &t: type and sub_type changed to SQL type IDs
 *
 * RETURN:
 *  void
 ******************************************************************************/
void SQLParser::get_r_op_subtype(token::Token &t) {
    for(int type = 0; type < R_OPS_SIZE; ++type)
        if(t.string() == _r_ops[type]) {
            t.set_type(R_OPS);
            t.set_sub_type(type);
            return;
        }
    t.set_type(VALUE);
    t.set_sub_type(VALUE);

    return;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Assigns a SQL parse key, used for adding into map's key. If no valid key
 *  can be found, returns false.
 *
 * PRE-CONDITIONS:
 *  int state    : adjacency table state
 *  int &key_code: key code by ref
 *
 * POST-CONDITIONS:
 *  int &key_code: new key code
 *
 * RETURN:
 *  bool
 ******************************************************************************/
bool SQLParser::get_parse_key(int state, int &key_code) {
    bool is_valid = true;

    // initial command codes
    if(state == CMD_CREATE || state == CMD_INSERT || state == CMD_SELECT)
        key_code = COMMAND;
    // CREATE command codes
    else if(state == CREATE_TABLE)
        key_code = TABLE_KEY;
    else if(state == CREATE_FIELDS)
        key_code = FIELDS_KEY;
    // INSERT command codes
    else if(state == INSERT_TABLE)
        key_code = TABLE_KEY;
    else if(state == INSERT_VALUE)
        key_code = VALUE_KEY;
    // SELECT command codes
    else if(state == SELECT_FIELDS || state == SELECT_ASTERISK)
        key_code = FIELDS_KEY;
    else if(state == SELECT_TABLE)
        key_code = TABLE_KEY;
    // SELECT command for relationship codes
    else if(state == SELECT_R_FIELDS)
        key_code = WHERE_KEY;
    else if(state == SELECT_R_OPS)
        key_code = R_OPS_KEY;
    else if(state == SELECT_VALUES)
        key_code = VALUE_KEY;
    else if(state == SELECT_L_OPS)
        key_code = L_OPS_KEY;
    else
        is_valid = false;

    return is_valid;
}

// TO DO
SQLParser &operator>>(SQLParser &f, token::Token &t) {
    t = f.next_token();
    return f;
}

}  // namespace sql
