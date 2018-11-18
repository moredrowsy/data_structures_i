#include "../include/sql_tokenizer.h"

namespace sql {

SQLTokenizer::SQLTokenizer(char *buffer, std::size_t block_size)
    : _block_size(block_size), _more(false), _stk(buffer, _block_size) {
    assert(_block_size <= MAX_BLOCK);
}

bool SQLTokenizer::more() const { return (bool)_stk; }

SQLTokenizer::operator bool() const { return (bool)_stk; }

token::Token SQLTokenizer::parse() {
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
    else
        t.set_type(STRING);

    return t;
}

SQLTokenizer &operator>>(SQLTokenizer &f, token::Token &t) {
    t = f.parse();

    return f;
}

}  // namespace sql
