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
        case state_machine::STATE_UNKNOWN:
            type_string = "UNKNOWN";
            break;
        case state_machine::STATE_DOUBLE:
            type_string = "DOUBLE";
            break;
        case state_machine::STATE_SPACE:
            type_string = "SPACE";
            break;
        case state_machine::STATE_ALPHA:
            type_string = "ALPHA";
            break;
        case state_machine::STATE_PUNCT:
            type_string = "PUNCT";
            break;
        default:
            type_string = "ERROR";
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
bool operator==(const Token lhs, const Token rhs) {
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
bool operator!=(const Token lhs, const Token rhs) {
    return lhs._type != rhs._type;
}

}  // namespace token
