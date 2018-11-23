#ifndef SQL_H
#define SQL_H

#include <fstream>       // ifstream
#include "bpt_map.h"     // B+Tree's Map/MMap class
#include "set.h"         // Set class
#include "sql_parser.h"  // SQLTokenizer class
#include "sql_table.h"   // SQLTable class
#include "stack.h"       // Stack class

namespace sql {

enum SET_CODE { SET_OR = -2, SET_AND = -1 };

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

    void init_error_codes();

    void load_commands(const std::string& file_name);  // load command from file
    bool get_query();   // get query and output a valid parse tree
    void exec_query();  // execute query but also do additional checks

    void create_table(const std::string& table_name, bool table_found);
    void insert_table(const std::string& table_name, bool table_found);
    void select_table(const std::string& table_name, bool table_found);

    // pre: table exists
    bool values_match_fields(const std::string& table_name);
    bool fields_match_fields(const std::string& table_name);
    bool is_valid_fields(const std::string& table_name);

    void make_set(const std::string& field, const std::string op,
                  set::Set<long>& result);

    void print_specific(const std::string& table_name);
};

}  // namespace sql

#endif  // SQL_H
