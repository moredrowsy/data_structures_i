#include "../include/sql_parser.h"

namespace sql {

// STATIC VARIABLES
bool SQLParser::_need_init = true;
int SQLParser::_table[MAX_ROWS][MAX_COLS];
ParseKey SQLParser::_keys;
TokenType SQLParser::_types;
TokenType SQLParser::_subtypes;

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
 *  Parses the query and returns a valid ParseTree and QueueTokens (for infix
 *  expression) with SQL tokens; else if query is not valid, returns an empty
 *  ParseTree and QueueTokens.
 *
 * PRE-CONDITIONS:
 *  ParseTree &map: multip map by ref
 *
 * POST-CONDITIONS:
 *  ParseTree &map: multip map populated with valid Tokens or else empty
 *
 * RETURN:
 *  bool
 ******************************************************************************/
bool SQLParser::parse_query(ParseTree &tree, QueueTokens &infix) {
    bool is_good = false;  // query's success
    int state = CMD_START;
    int key_code;

    token::Token t = next_token();  // get SQL Token from STokenizer

    while(_tokenizer && t.type() > -1 && _table[state][t.type()] != -1) {
        state = _table[state][t.type()];
        is_good = is_success(_table, state);  // log query success

        if(get_parse_key(state, key_code)) {      // if valid code
            tree[_keys[key_code]] += t.string();  // add to map

            if(state == SELECT_R_FIELDS)
                tree[_keys[KEY_R_FIELDS]] += t.string();

            if(key_code == KEY_WHERE) {
                infix.push(get_sql_token(t));
            }
        }

        t = next_token();  // get next SQL Token
    }

    if(!is_good) tree.clear();  // clear map if query fails

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
    init_subtypes(_subtypes);
    init_table(_table);
    mark_table_command(_table);
    mark_table_create(_table);
    mark_table_insert(_table);
    mark_table_select(_table);

    _need_init = false;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Initializes key type to parse map string keys.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
void SQLParser::init_keys(ParseKey &keys) {
    keys[KEY_COMMAND] = "COMMAND";
    keys[KEY_FIELDS] = "FIELDS";
    keys[KEY_R_FIELDS] = "R_FIELDS";
    keys[KEY_WHERE] = "WHERE";
    keys[KEY_TABLE] = "TABLE";
    keys[KEY_VALUES] = "VALUES";
}

/*******************************************************************************
 * DESCRIPTION:
 *  Initializes strings to respective columns for the SQL adjacency matrix.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
void SQLParser::init_types(TokenType &types) {
    types["CREATE"] = CREATE;
    types["MAKE"] = CREATE;
    types["INSERT"] = INSERT;
    types["SELECT"] = SELECT;
    types["TABLE"] = TABLE;
    types["INTO"] = INTO;
    types["FROM"] = FROM;
    types["WHERE"] = WHERE;
    types["FIELDS"] = FIELDS;
    types["VALUES"] = VALUES;
    types["*"] = ASTERISK;
    types[","] = COMMA;
    types["AND"] = L_OPS;
    types["OR"] = L_OPS;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Initializes operator strings to sub types.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
void SQLParser::init_subtypes(TokenType &subtypes) {
    subtypes["="] = STR_ASSIGN;
    subtypes["<"] = STR_LESS;
    subtypes["<="] = STR_LESS_THAN;
    subtypes[">"] = STR_GREATER;
    subtypes[">="] = STR_GREATER_THAN;
    subtypes["AND"] = STR_OP_AND;
    subtypes["OR"] = STR_OP_OR;
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
    if(t.type() == state_machine::STATE_IDENT) {
        // check if IDENT is a keyword
        if(t.sub_type() == state_machine::STATE_IDENT_NORM)
            get_keyword(t, IDENT);
        else
            t.set_type(IDENT);
    } else if(t.type() == state_machine::STATE_PUNCT)
        get_keyword(t, VALUE);
    else if(t.type() == state_machine::STATE_R_OP)
        get_r_op_subtype(t, VALUE);
    else
        t.set_type(VALUE);  // VALUE w/o S
}

/*******************************************************************************
 * DESCRIPTION:
 *  Find if the string is a keyword. If found, set Token type to keyword ID and
 *  if L_OP type, also set subtype ID to distinguish what type of L_OP. If
 *  keyword is not found, set to generic IDENT id.
 *
 * PRE-CONDITIONS:
 *  token::Token &t: extracted token from SQL Tokenizer
 *  int default_id : default id to set if keyword not found
 *
 * POST-CONDITIONS:
 *  token::Token &t: type and sub_type changed to SQL type IDs
 *
 * RETURN:
 *  void
 ******************************************************************************/
void SQLParser::get_keyword(token::Token &t, int default_id) {
    std::string cmp_str = t.string();
    std::transform(cmp_str.begin(), cmp_str.end(), cmp_str.begin(), ::toupper);

    if(_types.contains(cmp_str)) {
        t.set_string(cmp_str);
        t.set_type(_types[cmp_str]);
    } else
        t.set_type(default_id);
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
void SQLParser::get_r_op_subtype(token::Token &t, int default_id) {
    if(_subtypes.contains(t.string())) {
        t.set_type(R_OPS);
        t.set_sub_type(_subtypes[t.string()]);
    } else {
        t.set_type(default_id);
        t.set_sub_type(default_id);
    }

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

    switch(state) {
        case CMD_CREATE:
        case CMD_INSERT:
        case CMD_SELECT:
            key_code = KEY_COMMAND;
            break;
        case CREATE_TABLE:
        case INSERT_TABLE:
        case SELECT_TABLE:
            key_code = KEY_TABLE;
            break;
        case CREATE_FIELDS:
        case SELECT_FIELDS:
        case SELECT_ASTERISK:
            key_code = KEY_FIELDS;
            break;
        case INSERT_VALUE:
            key_code = KEY_VALUES;
            break;
        case SELECT_VALUE:
        case SELECT_R_FIELDS:
        case SELECT_R_OPS:
        case SELECT_L_OPS:
            key_code = KEY_WHERE;
            break;
        default:
            is_valid = false;
    }

    return is_valid;
}

token_ptr SQLParser::get_sql_token(token::Token &t) {
    if(t.type() == VALUE && t.sub_type() == state_machine::STATE_DOUBLE)
        return std::make_shared<SQLToken>(t.string(), TOKEN_DOUBLE);
    else if(t.string() == "=")
        return std::make_shared<SQLToken>(t.string(), TOKEN_R_OP, TOKEN_R_EQ);
    else if(t.string() == "<")
        return std::make_shared<SQLToken>(t.string(), TOKEN_R_OP, TOKEN_R_L);
    else if(t.string() == "<=")
        return std::make_shared<SQLToken>(t.string(), TOKEN_R_OP, TOKEN_R_LEQ);
    else if(t.string() == ">")
        return std::make_shared<SQLToken>(t.string(), TOKEN_R_OP, TOKEN_R_G);
    else if(t.string() == ">=")
        return std::make_shared<SQLToken>(t.string(), TOKEN_R_OP, TOKEN_R_GEQ);
    else if(t.string() == "OR")
        return std::make_shared<SQLToken>(t.string(), TOKEN_OP_OR);
    else if(t.string() == "AND")
        return std::make_shared<SQLToken>(t.string(), TOKEN_OP_AND);
    else
        return std::make_shared<SQLToken>(t.string(), TOKEN_SET_STR);
}

// TO DO
SQLParser &operator>>(SQLParser &f, token::Token &t) {
    t = f.next_token();
    return f;
}

}  // namespace sql
