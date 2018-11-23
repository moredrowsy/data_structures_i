#include "../include/sql.h"

namespace sql {

SQL::SQL(char *fname) {
    char *buffer = new char[MAX_BUFFER];
    int parse_counts = 0;
    ParseTree parse_tree;
    std::ifstream fin(fname);

    while(fin.getline(buffer, MAX_BUFFER)) {
        _parser.set_string(buffer);
        if(_parser.parse_query(parse_tree)) {
            _parse_map[parse_counts++] = parse_tree;
            std::cout << buffer << std::endl;
            std::cout << _parse_map[parse_counts - 1] << std::endl << std::endl;
            parse_tree.clear();
        }
    }
    std::cout << _parse_map.size() << std::endl;

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
                std::cin >> file_name;
                load_commands(file_name);

                break;
            case 'Q':
            case 'q':
                std::cin.ignore();
                if(get_query())
                    exec_query();
                else
                    std::cout << "Invalid query." << std::endl;
                _parse_tree.clear();

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

    std::cin.getline(buffer, MAX_BUFFER);
    _parser.set_string(buffer);
    is_valid = _parser.parse_query(_parse_tree);

    delete[] buffer;

    return is_valid;
}

void SQL::load_commands(const std::string &file_name) {
    char *buffer = new char[MAX_BUFFER];
    std::ifstream file(file_name.c_str(), std::ios::binary);

    while(file.getline(buffer, MAX_BUFFER)) {
        _parser.set_string(buffer);
        if(_parser.parse_query(_parse_tree)) exec_query();
        _parse_tree.clear();
    }
}

void SQL::exec_query() {
    std::string table_name = _parse_tree["TABLE"][0];
    bool table_found = _table_map.contains(table_name);

    // TO DO CHECK IF TABLE EXISTS

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
        if(values_match_fields(table_name))
            _table_map[table_name].insert(_parse_tree["VALUES"]);
    } else
        std::cout << "The object named '" << table_name
                  << "' is not in the database." << std::endl;
}

void SQL::select_table(const std::string &table_name, bool table_found) {
    if(table_found) {
        std::cout << "\nTABLE: " << table_name << std::endl;
        if(_parse_tree["FIELDS"][0] == "*")
            _table_map[table_name].print_all();
        else {
            if(is_valid_fields(table_name)) print_specific(table_name);
        }
    } else
        std::cout << "The object named '" << table_name
                  << "' is not in the database." << std::endl;
}

bool SQL::values_match_fields(const std::string &table_name) {
    if(_parse_tree["VALUES"].size() != _table_map[table_name].field_count()) {
        std::cout
            << "Number of supplied values does not match table definition."
            << std::endl;
        return false;
    } else
        return true;
}

bool SQL::fields_match_fields(const std::string &table_name) {
    std::size_t field_size = _parse_tree["FIELDS"].size();

    for(std::size_t i = 0; i < field_size; ++i)
        if(!_table_map[table_name].contains(_parse_tree["FIELDS"][i]))
            return false;

    return true;
}

bool SQL::is_valid_fields(const std::string &table_name) {
    if(_parse_tree["FIELDS"].size() > _table_map[table_name].field_count()) {
        std::cout
            << "Number of supplied fields is larger than table definition."
            << std::endl;
        return false;
    }

    if(!fields_match_fields(table_name)) {
        std::cout << "The value of field columns does not match table "
                     "definition."
                  << std::endl;

        return false;
    }

    return true;
}

void SQL::make_set(const std::string &field, const std::string op,
                   set::Set<long> &result) {
    if(op == "=") {
        //
    }
}

void SQL::print_specific(const std::string &table_name) {
    std::size_t rec_count = _table_map[table_name].size();
    std::size_t rec_pos;

    if(_parse_tree.contains("WHERE")) {
        //
    } /* else
        for(std::size_t i = 0; i < rec_count; ++i)
            _table_map[table_name].print_rec(i, _parse_tree["FIELDS"]); */
    else {
        _table_map[table_name].print_specific_header(_parse_tree["FIELDS"]);

        auto field = _table_map[table_name].map()[_parse_tree["FIELDS"][0]];

        for(auto it = field.begin(); it != field.end(); ++it) {
            rec_pos = *it->value;
            _table_map[table_name].print_rec(rec_pos, _parse_tree["FIELDS"]);
        }
    }
}

}  // namespace sql
