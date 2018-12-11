/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : sql
 * NAMESPACE   : sql
 * DESCRIPTION : This header defines the SQL class. The SQL class stores data
 *          in table files (.tbl) and load them into memory, which are sorted
 *          Maps. From there, various SQL commands can process such data.
 *
 *          SUPPORTED COMMANDS:
 *          - CREATE: create a table
 *          - INSERT: insert values into table
 *          - SELECT: select data from table with WHERE conditions to display
 *                    specific fields
 ******************************************************************************/
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

enum SQL_QUERY_CODE {
    UNKNOWN_COMMAND = 1,
    EXIST_TABLE = 2,
    NOT_EXIST_TABLE = 3,
    WRONG_FIELD_SIZE = 4,
    FIELDS_OVERLIMIT = 5,
    WRONG_FIELDS_NAME = 6
};

class SQL {
public:
    typedef bpt_map::Map<std::string, SQLTable> TableMap;

    SQL();
    SQL(char* fname);  // fname := file with batch commands
    ~SQL();

    const QueryCodeMap& query_code_map() const;  // map of string for query code
    void print_table_list() const;

    void run();  // interactive sql

    void load_commands(const std::string& file_name);  // load command from file
    void change_session(const std::string& name);      // switch session
    void load_session();                               // restore session
    void save_session();                               // save session

private:
    static bool _need_init;
    static QueryCodeMap _query_code_map;

    QueueTokens _infix;     // curent infix expression
    SQLParser _parser;      // SQL parser
    ParseTree _parse_tree;  // parser tree
    TableMap _table_map;    // map of tables
    std::string _session;

    void init();  // init static variables

    bool get_query();  // get query and output a valid parse tree
    int exec_query();  // execute query but also do additional checks

    int create_table(const std::string& table_name, bool table_found);
    int insert_table(const std::string& table_name, bool table_found);
    int select_table(const std::string& table_name, bool table_found);

    // pre-condition: table exists
    bool insert_values_match_fields_size(const std::string& table_name);
    int is_valid_fields(const std::string& table_name);
};

}  // namespace sql

#endif  // SQL_H
