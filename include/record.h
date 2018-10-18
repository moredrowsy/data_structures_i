#ifndef RECORD_H
#define RECORD_H

namespace record {

template <typename T>
struct Record {
    int _key;
    T _value;

    // CONSTRUCTORS
    Record(int k = -1, const T& v = T()) : _key(k), _value(v) {}

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

}  // namespace record

#endif  // RECORD_H
