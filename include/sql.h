#ifndef SQL_H
#define SQL_H

#include <fstream>         // ifstream
#include <memory>          // shared_ptr
#include "bpt_map.h"       // B+Tree's Map/MMap class
#include "list.h"          // Queue class
#include "queue.h"         // Queue class
#include "set.h"           // Set class
#include "sql_parser.h"    // SQLTokenizer class
#include "sql_states.h"    // SQL states
#include "sql_table.h"     // SQLTable class
#include "sql_typedefs.h"  // typedefs for SQL
#include "stack.h"         // Stack class

namespace sql {

class SQL {
public:
    typedef bpt_map::Map<std::string, SQLTable> TableMap;

    SQL();
    SQL(char* fname);

    void run();

    // private:
    QueueTokens _infix;
    SQLParser _parser;
    ParseTree _parse_tree;
    ParseMap _parse_map;
    TableMap _table_map;

    void load_commands(const std::string& file_name);  // load command from file
    bool get_query();   // get query and output a valid parse tree
    void exec_query();  // execute query but also do additional checks

    void create_table(const std::string& table_name, bool table_found);
    void insert_table(const std::string& table_name, bool table_found);
    void select_table(const std::string& table_name, bool table_found);

    // pre: table exists
    bool insert_values_match_fields_size(const std::string& table_name);
    bool is_valid_fields(const std::string& table_name);
};

}  // namespace sql

#endif  // SQL_H
