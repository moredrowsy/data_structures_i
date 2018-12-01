#ifndef SQL_TABLE_H
#define SQL_TABLE_H

#include <algorithm>       // transform()
#include <cstdio>          // remove()
#include <iomanip>         // setw()
#include <string>          // string
#include <vector>          // vector
#include "bpt_map.h"       // B+Tree's Map/MMap class
#include "set.h"           // Set class
#include "sql_record.h"    // SQLRecord class
#include "sql_token.h"     // SQLToken class
#include "sql_typedefs.h"  // typedefs for SQL
#include "token.h"         // Token class

namespace sql {

class SQLTable {
public:
    enum { PRINT_COL_WIDTH = 20 };

    SQLTable() : _rec_count(0), _table_name() {}
    SQLTable(std::string table_name);
    SQLTable(std::string table_name, const std::vector<std::string>& fields);

    std::size_t field_count() const;
    std::size_t size() const;
    const FieldMap& map() const;

    void delete_table();

    bool contains(const std::string& field_name) const;
    bool insert(const std::vector<std::string>& values);
    bool is_match_fields(const std::vector<std::string>& list);

    bool select(const std::vector<std::string>& fields_list, QueueTokens& infix,
                SQLTable& new_table);

    void make_equal_set(const std::string& field, const std::string& value,
                        set_ptr& result);
    void make_less_set(const std::string& field, const std::string& value,
                       set_ptr& result);
    void make_less_eq_set(const std::string& field, const std::string& value,
                          set_ptr& result);
    void make_greater_set(const std::string& field, const std::string& value,
                          set_ptr& result);
    void make_greater_eq_set(const std::string& field, const std::string& value,
                             set_ptr& result);

    void print(const std::vector<std::string>& field_names =
                   std::vector<std::string>({"*"}),
               int width = PRINT_COL_WIDTH);
    void print_rec(long rec_pos, const std::vector<std::string>& field_names,
                   int width = PRINT_COL_WIDTH);
    void print_header(const std::vector<std::string>& field_names,
                      int width = PRINT_COL_WIDTH);

private:
    long _rec_count;
    FieldMap _map;
    FieldPosMap _pos_to_fields;   // map field pos to field name
    FiledNamesMap _field_to_pos;  // map field name to pos
    std::string _table_name;      // table name
    SQLRecord _record;            // read/write to table file

    void init_table();
    void init_fields();
    void init_data();

    std::string truncate(std::string str, size_t width, bool ellipsis = true);
    void infix_to_postfix(QueueTokens& infix, QueueTokens& postfix);
    void eval_postfix(QueueTokens& postfix, set_ptr& result_set);

    void update_fields(const std::vector<std::string>& fields);
};

}  // namespace sql

#endif  // SQL_TABLE_H
