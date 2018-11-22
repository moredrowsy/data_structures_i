#ifndef SQL_TABLE_H
#define SQL_TABLE_H

#include <iomanip>       // setw()
#include <string>        // string
#include <vector>        // vector
#include "bpt_map.h"     // B+Tree's Map/MMap class
#include "sql_record.h"  // SQLRecord class

namespace sql {

class SQLTable {
public:
    // field pos to field name; ie: pos #1, "lName"; pos #2, "fName"
    typedef bpt_map::Map<int, std::string> FieldPosMap;
    // field name to field pos; ie: "fName", pos #2; "lName", pos #1
    typedef bpt_map::Map<std::string, int> FiledNamesMap;

    // ----- map chains -----
    typedef bpt_map::MMap<std::string, long> IndexMap;
    typedef bpt_map::Map<std::string, IndexMap> FieldMap;

    SQLTable() : _rec_count(0), _table_name() {}
    SQLTable(std::string table_name);
    SQLTable(std::string table_name, const std::vector<std::string>& values);

    std::size_t field_count() const;

    bool insert(const std::vector<std::string>& values);
    void print_all();
    void print_rec(long rec_pos, const std::vector<std::string>& field_names);

private:
    long _rec_count;
    FieldMap _map;
    FieldPosMap _pos_to_fields;   // map field pos to field name
    FiledNamesMap _field_to_pos;  // map field name to pos
    std::string _table_name;      // table name
    SQLRecord _record;            //  read/write to table file

    void init_table();
    void init_fields();
    void init_data();

    std::string truncate(std::string str, size_t width, bool ellipsis = true);
};

}  // namespace sql

#endif  // SQL_TABLE_H
