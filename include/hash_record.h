/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : hash_record
 * DESCRIPTION : This header defines a templated Record structure with _key
 *      and _value pair. Comparison of Record type are by _key, not _value.
 *      Used by Hash type classes.
 ******************************************************************************/
#ifndef RECORD_H
#define RECORD_H

namespace hash_record {

template <typename T>
struct Record {
    int _key;
    T _value;

    // CONSTRUCTORS
    Record(int k = -1, const T& v = T()) : _key(k), _value(v) {}

    // FRIENDS
    friend bool operator==(const Record& left, const Record& right) {
        return left._key == right._key;
    }

    friend bool operator!=(const Record& left, const Record& right) {
        return left._key != right._key;
    }

    friend bool operator<(const Record& left, const Record& right) {
        return left._key < right._key;
    }

    friend bool operator<=(const Record& left, const Record& right) {
        return left._key <= right._key;
    }

    friend bool operator>(const Record& left, const Record& right) {
        return left._key > right._key;
    }

    friend bool operator>=(const Record& left, const Record& right) {
        return left._key >= right._key;
    }

    friend std::ostream& operator<<(std::ostream& outs,
                                    const Record& print_me) {
        return outs << print_me._key << " : " << print_me._value;
    }
};

}  // namespace hash_record

#endif  // RECORD_H
