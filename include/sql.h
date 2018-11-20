#ifndef SQL_H
#define SQL_H

#include <fstream>       // ifstream
#include "bpt_map.h"     // B+Tree's Map/MMap class
#include "sql_parser.h"  // SQLTokenizer class
#include "sql_table.h"   // SQLTable class

namespace sql {

class SQL {
public:
    typedef bpt_map::MMap<std::string, std::string> ParseTree;
    typedef bpt_map::Map<int, ParseTree> ParseMap;
    typedef bpt_map::Map<std::string, SQLTable> TableMap;

    SQL() {}
    SQL(char *fname);

private:
    SQLParser _parser;
    ParseMap _parse_map;
    TableMap _table_map;
};

}  // namespace sql

#endif  // SQL_H
