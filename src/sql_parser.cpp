#include "../include/sql_parser.h"

namespace sql {

// STATIC VARIABLES
bool SQLParser::_need_init = true;
int SQLParser::_table[MAX_ROWS][MAX_COLS];
std::string SQLParser::_keys[MAX_KEYS];
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
bool SQLParser::get_query(Map &map) {
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
    init_table(_table);
    mark_table_command(_table);
    mark_table_create(_table);
    mark_table_insert(_table);
    mark_table_select(_table);

    _need_init = false;
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
        key_code = R_FIELDS;
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

    if(cmp_str == _types[CREATE]) {
        t.set_type(CREATE);
        t.set_string(std::move(cmp_str));
    } else if(cmp_str == _types[INSERT]) {
        t.set_type(INSERT);
        t.set_string(std::move(cmp_str));
    } else if(cmp_str == _types[SELECT]) {
        t.set_type(SELECT);
        t.set_string(std::move(cmp_str));
    } else if(cmp_str == _types[TABLE]) {
        t.set_type(TABLE);
        t.set_string(std::move(cmp_str));
    } else if(cmp_str == _types[INTO]) {
        t.set_type(INTO);
        t.set_string(std::move(cmp_str));
    } else if(cmp_str == _types[FROM]) {
        t.set_type(FROM);
        t.set_string(std::move(cmp_str));
    } else if(cmp_str == _types[WHERE]) {
        t.set_type(WHERE);
        t.set_string(std::move(cmp_str));
    } else if(cmp_str == _types[FIELDS]) {
        t.set_type(FIELDS);
        t.set_string(std::move(cmp_str));
    } else if(cmp_str == _types[VALUES]) {  // VALUES with a S
        t.set_type(VALUES);
        t.set_string(std::move(cmp_str));
    } else if(cmp_str == _types[AND]) {
        t.set_type(L_OPS);
        t.set_sub_type(AND);
        t.set_string(std::move(cmp_str));
    } else if(cmp_str == _types[OR]) {
        t.set_type(L_OPS);
        t.set_sub_type(OR);
        t.set_string(std::move(cmp_str));
    } else if(t.type() == state_machine::STATE_COMMA)
        t.set_type(COMMA);
    else if(t.type() == state_machine::STATE_STAR)
        t.set_type(ASTERISK);
    else if(t.type() == state_machine::STATE_IDENT) {
        t.set_type(IDENT);
        t.set_string(std::move(cmp_str));
    } else if(t.type() == state_machine::STATE_IDENT_QUOTE)
        t.set_type(IDENT);
    else if(t.type() == state_machine::STATE_R_OP)
        t.set_type(R_OPS);
    else if(t.type() == state_machine::STATE_L_OP)
        t.set_type(L_OPS);
    else
        t.set_type(VALUE);  // VALUE w/o S
}

// TO DO
SQLParser &operator>>(SQLParser &f, token::Token &t) {
    t = f.next_token();
    return f;
}

}  // namespace sql
