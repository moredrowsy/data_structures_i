#ifndef SQL_H
#define SQL_H

#include "bpt_map.h"        // B+Tree's Map/MMap class
#include "sql_states.h"     // SQL constants
#include "sql_token.h"      // SQLToken class
#include "sql_tokenizer.h"  // SQLTokenizer class

namespace sql {

class SQL {
public:
    SQL() {}

    bool parse(char *buffer);

private:
    // unsigned _table[MAX_COLUMNS][MAX_ROWS];
};

bool SQL::parse(char *buffer) {
    if(buffer) {
        //
    }
    return true;
}

}  // namespace sql

#endif  // SQL_H
