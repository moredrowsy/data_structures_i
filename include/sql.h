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
    SQL(char* fname);

    void run();

private:
    SQLParser _parser;
    ParseTree _parse_tree;
    ParseMap _parse_map;
    TableMap _table_map;

    bool get_query();
    void query();
    void create_table(const std::string& table_name, bool table_found);
    void insert_table(const std::string& table_name, bool table_found);
    void select_table(const std::string& table_name, bool table_found);

    // pre: table exists
    bool values_match_fields(const std::string& table_name);
    void print_specific(const std::string& table_name);
};

}  // namespace sql

#endif  // SQL_H
