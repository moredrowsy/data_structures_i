#include "../include/token.h"

namespace token {

/*******************************************************************************
 * DESCRIPTION:
 *  Returns an integer corresponding to the string type, such as the
 *  state_machine's state constants.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  int of string type (or state constants in state_machine)
 ******************************************************************************/
int Token::type() const { return _type; }

/*******************************************************************************
 * DESCRIPTION:
 *  Returns a readable string corresponding to the string type, such as the
 *  state_machine's state constants, for human interpretation.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  string type for human readable interpretation
 ******************************************************************************/
std::string Token::type_string() const {
    std::string type_string;

    switch(_type) {
        case state_machine::STATE_ERROR:
            type_string = "ERROR";
            break;
        case state_machine::STATE_DOUBLE:
            type_string = "DOUBLE";
            break;
        case state_machine::STATE_ALPHA:
            type_string = "ALPHA";
            break;
        case state_machine::STATE_SPACE:
            type_string = "SPACE";
            break;
        case state_machine::STATE_COMMA:
            type_string = "COMMA";
            break;
        case state_machine::STATE_STAR:
            type_string = "ASTERISK";
            break;
        case state_machine::STATE_PUNCT:
            type_string = "PUNCT";
            break;
        case state_machine::STATE_IDENT:
            type_string = "IDENT";
            break;
        case state_machine::STATE_IDENTIFIER:
            type_string = "IDENT";
            break;
        case state_machine::STATE_VALUE:
            type_string = "VALUE";
            break;
        case state_machine::STATE_IN_QUOTE_S:
            type_string = "ENCLOSED_QUOTE_S";
            break;
        case state_machine::STATE_IN_QUOTE_D:
            type_string = "ENCLOSED_QUOTE_D";
            break;
        case state_machine::STATE_OP:
            type_string = "OPERATOR";
            break;
        case state_machine::STATE_OP_SINGLE:
            type_string = "OPERATOR SINGLE";
            break;
        case state_machine::STATE_LT:
            type_string = "OPERATOR LESS";
            break;
        case state_machine::STATE_GT:
            type_string = "OPERATOR GREATER";
            break;
        case state_machine::STATE_EQUALITY:
            type_string = "OPERATOR EQUALITY";
            break;
        default:
            type_string = "UNKNOWN";
    }

    return type_string;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Returns the string value, such as the input buffer string from STokenizer.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  string
 ******************************************************************************/
std::string Token::string() const { return _token; }

/*******************************************************************************
 * DESCRIPTION:
 *  Changes string type.
 *
 * PRE-CONDITIONS:
 *  int type: string type
 *
 * POST-CONDITIONS:
 *  int _type: set to new type
 *
 * RETURN:
 *  none
 ******************************************************************************/
void Token::set_type(int type) { _type = type; }

/*******************************************************************************
 * DESCRIPTION:
 *  Changes token's string.
 *
 * PRE-CONDITIONS:
 *  std::string str: new string
 *
 * POST-CONDITIONS:
 *  std::string _token: set to new string
 *
 * RETURN:
 *  none
 ******************************************************************************/
void Token::set_string(std::string str) { _token = str; }

/*******************************************************************************
 * DESCRIPTION:
 *  Inserts a formatted Token's string value to the outstream.
 *
 * PRE-CONDITIONS:
 *  ostream& outs : out stream
 *  const Token& t: Token object with string value
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  ostream by reference
 ******************************************************************************/
std::ostream& operator<<(std::ostream& outs, const Token& t) {
    return outs << "|" + t.string() + "|";
}

/*******************************************************************************
 * DESCRIPTION:
 *  Equality comparison
 *
 * PRE-CONDITIONS:
 *  const Token lhs: left token
 *  const Token rhs: right token
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
bool operator==(const Token& lhs, const Token& rhs) {
    return lhs._type == rhs._type;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Inequality comparison
 *
 * PRE-CONDITIONS:
 *  const Token lhs: left token
 *  const Token rhs: right token
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
bool operator!=(const Token& lhs, const Token& rhs) {
    return lhs._type != rhs._type;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Add two Tokens together with left hand side concatenating the right hand
 *  side's string.
 *
 * PRE-CONDITIONS:
 *  const Token lhs: left token
 *  const Token rhs: right token
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
Token& operator+=(Token& lhs, const Token& rhs) {
    lhs._token += rhs._token;
    return lhs;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Add two Tokens together with left hand side concatenating the right hand
 *  side's string.
 *
 * PRE-CONDITIONS:
 *  const Token lhs: left token
 *  const Token rhs: right token
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
Token& operator+(Token& lhs, const Token& rhs) {
    lhs._token += rhs._token;
    return lhs;
}

}  // namespace token
