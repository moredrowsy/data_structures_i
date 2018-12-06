#include "../include/sql.h"

namespace sql {

std::string SQL::_op_strings[STR_OPS_SIZE];
bool SQL::need_init = true;

SQL::SQL() {
    if(need_init) init();
}

SQL::SQL(char *fname) {
    char *buffer = new char[MAX_BUFFER];
    int parse_counts = 0;

    std::ifstream fin(fname);

    if(need_init) init();

    // WIP NEED TO FIX
    while(fin.getline(buffer, MAX_BUFFER)) {
        _parser.set_string(buffer);  // set string for each line to parser
        if(_parser.parse_query(_parse_tree, _infix)) {  // if the query is valid
            _parse_map[parse_counts++] = _parse_tree;
            // std::cout << buffer << std::endl;
            // std::cout << _parse_map[parse_counts - 1] << std::endl <<
            // std::endl;
            _parse_tree.clear();
            _infix.clear();
        }
    }

    delete[] buffer;
}

void SQL::run() {
    char c;
    std::string file_name;

    do {
        std::cout << "[Q]uery  [L]oad_Commands    e[X]it: ";
        std::cin >> c;

        switch(c) {
            case 'X':
            case 'x':
                return;  // exit function
            case 'L':
            case 'l':
                std::cout << "Filename: ";
                std::cin >> file_name;
                load_commands(file_name);

                break;
            case 'Q':
            case 'q':
                std::cout << "Query: ";
                std::cin.ignore();
                if(get_query())
                    exec_query();
                else
                    std::cout << "Invalid query." << std::endl;
                _parse_tree.clear();
                _infix.clear();

                break;

            default:
                std::cout << "Invalid Choice" << std::endl;
        }
        std::cout << std::endl;
    } while(c != 'X' && c != 'x');
}

bool SQL::get_query() {
    bool is_valid = false;
    char *buffer = new char[MAX_BUFFER];

    std::cin.getline(buffer, MAX_BUFFER);                 // get one input line
    _parser.set_string(buffer);                           // pass to parser
    is_valid = _parser.parse_query(_parse_tree, _infix);  // parse input

    delete[] buffer;

    return is_valid;
}

void SQL::init() {
    sql::init_ops(_op_strings);
    need_init = false;
}

void SQL::load_commands(const std::string &file_name) {
    char *buffer = new char[MAX_BUFFER];
    std::ifstream file(file_name.c_str(), std::ios::binary);

    while(file.getline(buffer, MAX_BUFFER)) {  // get each line from file
        _parser.set_string(buffer);            // pass to parser

        // if parser passes good query, execute query
        if(_parser.parse_query(_parse_tree, _infix)) exec_query();
        _parse_tree.clear();  // reset parse tree
        _infix.clear();       // reset infix
    }
}

void SQL::exec_query() {
    std::string table_name = _parse_tree["TABLE"][0];
    std::string command = _parse_tree["COMMAND"][0];
    bool table_found = _table_map.contains(table_name);

    if(_parse_tree["COMMAND"][0] == "CREATE")
        create_table(table_name, table_found);
    else if(_parse_tree["COMMAND"][0] == "INSERT")
        insert_table(table_name, table_found);
    else if(_parse_tree["COMMAND"][0] == "SELECT")
        select_table(table_name, table_found);
}

void SQL::create_table(const std::string &table_name, bool table_found) {
    if(table_found) {
        std::cout << "There is already an object named '" << table_name
                  << "' in the database." << std::endl;
    } else
        _table_map[table_name] = SQLTable(table_name, _parse_tree["FIELDS"]);
}

void SQL::insert_table(const std::string &table_name, bool table_found) {
    if(table_found) {
        if(insert_values_match_fields_size(table_name))
            _table_map[table_name].insert(_parse_tree["VALUES"]);
    } else
        std::cout << "The object named '" << table_name
                  << "' is not in the database." << std::endl;
}

void SQL::select_table(const std::string &table_name, bool table_found) {
    if(table_found) {
        std::cout << "\nTABLE: " << table_name << std::endl;

        if(is_valid_fields(table_name)) {
            SQLTable new_table("__temp__1");

            _table_map[table_name].select(_parse_tree["FIELDS"], _infix,
                                          new_table);
            new_table.delete_table();
        }
    } else
        std::cout << "The object named '" << table_name
                  << "' is not in the database." << std::endl;
}

bool SQL::insert_values_match_fields_size(const std::string &table_name) {
    if(_parse_tree["VALUES"].size() != _table_map[table_name].field_count()) {
        std::cout
            << "Number of supplied values does not match table definition."
            << std::endl;
        return false;
    } else
        return true;
}

bool SQL::is_valid_fields(const std::string &table_name) {
    if(_parse_tree["FIELDS"].size() > _table_map[table_name].field_count()) {
        std::cout
            << "Number of supplied fields is larger than table definition."
            << std::endl;
        return false;
    }

    if(_parse_tree["FIELDS"][0] != "*" &&
       !_table_map[table_name].is_match_fields(_parse_tree["FIELDS"])) {
        std::cout << "The FIELD columns do not match table definition."
                  << std::endl;

        return false;
    }

    if(_parse_tree.contains("WHERE") &&
       !_table_map[table_name].is_match_fields(_parse_tree["R_FIELDS"])) {
        std::cout << "The field columns do not match table definition."
                  << std::endl;

        return false;
    }

    return true;
}

}  // namespace sql
