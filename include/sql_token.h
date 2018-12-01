#ifndef SQL_TOKEN_H
#define SQL_TOKEN_H

#include <memory>  // shared_ptr
#include <string>  // string
#include "set.h"   // Set class

namespace sql {

enum SQLTokenTypes {
    TOKEN_SET_STR = -2,
    TOKEN_DOUBLE = -1,
    TOKEN_L_OP,
    TOKEN_OP_OR,
    TOKEN_OP_AND,
    TOKEN_R_OP,
    TOKEN_R_EQ,
    TOKEN_R_L,
    TOKEN_R_LEQ,
    TOKEN_R_G,
    TOKEN_R_GEQ,
    TOKEN_ADD_SUB,
    TOKEN_ADD,
    TOKEN_SUB,
    TOKEN_MUL_DIV,
    TOKEN_MUL,
    TOKEN_DIV,
};

class SQLToken {
public:
    SQLToken(std::string str, int type = TOKEN_SET_STR)
        : _type(type),
          _subtype(type),
          _string(str),
          _data(std::make_shared<set::Set<long>>()) {}

    SQLToken(std::string str, int type, int subtype)
        : _type(type),
          _subtype(subtype),
          _string(str),
          _data(std::make_shared<set::Set<long>>()) {}

    int type() const { return _type; }
    int subtype() const { return _subtype; }
    const std::string& string() const { return _string; }
    std::shared_ptr<set::Set<long>>& data() { return _data; }

    // FRIENDS
    friend std::ostream& operator<<(std::ostream& outs, const SQLToken& t) {
        outs << "type: " << t.type() << ", subtype: " << t.subtype()
             << ", string: " << t.string();
        return outs;
    }

    friend bool operator==(const SQLToken& lhs, const SQLToken& rhs) {
        return lhs.type() == rhs.type() && lhs.subtype() == rhs.subtype();
    }

    friend bool operator!=(const SQLToken& lhs, const SQLToken& rhs) {
        return lhs.type() != rhs.type() && lhs.subtype() != rhs.subtype();
    }

    friend bool operator<(const SQLToken& lhs, const SQLToken& rhs) {
        return lhs.type() < rhs.type() && lhs.subtype() < rhs.subtype();
    }

    friend bool operator<=(const SQLToken& lhs, const SQLToken& rhs) {
        return lhs.type() <= rhs.type() && lhs.subtype() <= rhs.subtype();
    }

    friend bool operator>(const SQLToken& lhs, const SQLToken& rhs) {
        return lhs.type() > rhs.type() && lhs.subtype() > rhs.subtype();
    }

    friend bool operator>=(const SQLToken& lhs, const SQLToken& rhs) {
        return lhs.type() >= rhs.type() && lhs.subtype() >= rhs.subtype();
    }

private:
    int _type;
    int _subtype;
    std::string _string;
    std::shared_ptr<set::Set<long>> _data;
};

}  // namespace sql

#endif  // SQL_TOKEN_H
