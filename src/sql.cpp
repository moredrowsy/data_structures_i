#include "../include/sql.h"

namespace sql {

// STATIC VARIABLES
bool SQL::_need_init = true;
QueryCodeMap SQL::_query_code_map;

/*******************************************************************************
 * DESCRIPTION:
 *  Default constructor.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  initializations
 *
 * RETURN:
 *  none
 ******************************************************************************/
SQL::SQL() : _session("default") {
    if(_need_init) init();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Constructor with a file name to process a batch of commands.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  initializations and data population
 *
 * RETURN:
 *  none
 ******************************************************************************/
SQL::SQL(char *fname) : _session("default") {
    if(_need_init) init();

    load_commands(fname);
    std::cout << std::endl;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Destructor saves session information.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
SQL::~SQL() { save_session(); }

/*******************************************************************************
 * DESCRIPTION:
 *  Returns map of query code to strings. Use for printing out parse query
 *  states.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
const QueryCodeMap &SQL::query_code_map() const { return _query_code_map; }

/*******************************************************************************
 * DESCRIPTION:
 *  Prints a list of table managed by current session.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
void SQL::print_table_list() const {
    std::cout << "Tables Managed in Session: " << _session << std::endl;
    for(const auto &table : _table_map) std::cout << table.key << std::endl;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Interactive run function.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
void SQL::run() {
    char c;
    int query_code;
    std::string input;

    do {
        std::cout << "[Q]uery  [T]able  [L]oad_Commands  [S]ave_Session  "
                     "[C]hange_Session     e[X]it: ";
        std::cin >> c;

        switch(c) {
            case 'X':
            case 'x':
                return;  // exit function
            case 'C':
            case 'c':
                std::cout << "Session: ";
                std::cin >> input;
                change_session(input);

                break;
            case 'L':
            case 'l':
                std::cout << "Filename: ";
                std::cin >> input;
                load_commands(input);

                break;
            case 'Q':
            case 'q':
                std::cout << "Query: ";
                std::cin.ignore();
                if(get_query()) {
                    query_code = exec_query();
                    std::cout << "SQL Query: " << _parse_tree["COMMAND"] << " ";
                    std::cout << _query_code_map[query_code] << std::endl;
                } else
                    std::cout << "Invalid query." << std::endl;

                _parse_tree.clear();
                _infix.clear();

                break;
            case 'S':
            case 's':
                save_session();

                break;
            case 'T':
            case 't':
                print_table_list();

                break;
            default:
                std::cout << "Invalid Choice" << std::endl;
        }
        std::cout << std::endl;
    } while(c != 'X' && c != 'x');
}

/*******************************************************************************
 * DESCRIPTION:
 *  Get a char string from user and parses it. Returns validity of query.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
bool SQL::get_query() {
    bool is_valid = false;
    char *buffer = new char[MAX_BUFFER];

    std::cin.getline(buffer, MAX_BUFFER);                 // get one input line
    _parser << buffer;                                    // pass to parser
    is_valid = _parser.parse_query(_parse_tree, _infix);  // parse input

    delete[] buffer;

    return is_valid;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Initialize query code map.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
void SQL::init() {
    std::string error = "ERROR: ";

    _query_code_map[0] = "SUCCESS";
    _query_code_map[UNKNOWN_COMMAND] = error + "Unknown command";
    _query_code_map[EXIST_TABLE] = error + "Table already exists";
    _query_code_map[NOT_EXIST_TABLE] = error + "Table does not exist";
    _query_code_map[WRONG_FIELD_SIZE] = error + "Wrong field size";
    _query_code_map[FIELDS_OVERLIMIT] = error + "Too many fields";
    _query_code_map[WRONG_FIELDS_NAME] = error + "Field name does not match";

    _need_init = false;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Opens file, get every line of command and parses it.
 *
 * PRE-CONDITIONS:
 *  const std::string &file_name: batch file name
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
void SQL::load_commands(const std::string &file_name) {
    char *buffer = new char[MAX_BUFFER];
    int num = 0, query_code = 0;
    std::ifstream file(file_name.c_str(), std::ios::binary);

    while(file.getline(buffer, MAX_BUFFER)) {  // get each line from file

        if(buffer[0] != '\0' && buffer[0] != '/' && buffer[0] != ' ' &&
           buffer[0] != '\n') {
            _parser << buffer;  // pass buffer to parser

            // if parsing query returns good, execute query
            if(_parser.parse_query(_parse_tree, _infix)) {
                std::cout << "SQL Query: DONE" << std::endl;
                std::cout << "[" << num++ << "] " << buffer << std::endl;

                query_code = exec_query();
                std::cout << "SQL Query: " << _parse_tree["COMMAND"] << " ";
                std::cout << _query_code_map[query_code] << std::endl;

                _parse_tree.clear();  // reset parse tree
                _infix.clear();       // reset infix
            } else {
                std::cout << "SQL Query: ERROR" << std::endl;
                std::cout << "[" << num++ << "] " << buffer << std::endl;
            }
            std::cout << std::endl;

        } else
            std::cout << buffer << std::endl;
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Changes user session.
 *
 * PRE-CONDITIONS:
 *  const std::string &name: new session to load
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
void SQL::change_session(const std::string &name) {
    _infix.clear();
    _parse_tree.clear();
    _table_map.clear();
    _session = name;

    load_session();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Load session information.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
void SQL::load_session() {
    std::string fname = _session + ".sql";
    std::ifstream file(fname.c_str(), std::ios::binary);
    std::string table_name;
    while(file >> table_name) _table_map[table_name] = SQLTable(table_name);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Save current session information.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
void SQL::save_session() {
    std::string fname = _session + ".sql";
    std::ofstream file(fname.c_str(), std::ios::binary | std::ios::trunc);
    for(const auto &table : _table_map) file << table.key << "\n";
}

/*******************************************************************************
 * DESCRIPTION:
 *  Execute SQL query. Only call when query parser passes!
 *
 * PRE-CONDITIONS:
 *  _parser's parse_query() returns true
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  int: Query code
 ******************************************************************************/
int SQL::exec_query() {
    std::string table_name = _parse_tree["TABLE"][0];
    std::string command = _parse_tree["COMMAND"][0];
    bool table_found = _table_map.contains(table_name);

    switch(_parser.types()[command]) {
        case CREATE:
            return create_table(table_name, table_found);
        case INSERT:
            return insert_table(table_name, table_found);
        case SELECT:
            return select_table(table_name, table_found);
        default:
            return UNKNOWN_COMMAND;
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Create SQL table.
 *
 * PRE-CONDITIONS:
 *  const std::string &table_name: table name
 *  bool table_found             : flag for table found
 *
 * POST-CONDITIONS:
 *  _table_map added with new SQLTable if creation success
 *
 * RETURN:
 *  int: Query code
 ******************************************************************************/
int SQL::create_table(const std::string &table_name, bool table_found) {
    if(table_found)
        return EXIST_TABLE;
    else {
        _table_map[table_name] = SQLTable(table_name, _parse_tree["FIELDS"]);
        return 0;
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Insert data into SQL table.
 *
 * PRE-CONDITIONS:
 *  const std::string &table_name: table name
 *  bool table_found             : flag for table found
 *
 * POST-CONDITIONS:
 *  _table_map's SQLTable modified if insertion success
 *
 * RETURN:
 *  int: Query code
 ******************************************************************************/
int SQL::insert_table(const std::string &table_name, bool table_found) {
    if(table_found) {
        if(insert_values_match_fields_size(table_name)) {
            _table_map[table_name].insert(_parse_tree["VALUES"]);
            return 0;
        } else
            return WRONG_FIELD_SIZE;
    } else
        return NOT_EXIST_TABLE;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Select data from SQL table.
 *
 * PRE-CONDITIONS:
 *  const std::string &table_name: table name
 *  bool table_found             : flag for table found
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  int: Query code
 ******************************************************************************/
int SQL::select_table(const std::string &table_name, bool table_found) {
    if(table_found) {
        int query_code = is_valid_fields(table_name);

        if(query_code == 0) {
            std::cout << "\nTABLE: " << table_name << std::endl;

            SQLTable new_table(table_name + "__temp__");

            _table_map[table_name].select(_parse_tree["FIELDS"], _infix,
                                          new_table);
            new_table.delete_table();

            std::cout << std::endl;

            return 0;
        } else
            return query_code;
    } else
        return NOT_EXIST_TABLE;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Checks if list of values matches fields size.
 *
 * PRE-CONDITIONS:
 *  const std::string &table_name: table name
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
bool SQL::insert_values_match_fields_size(const std::string &table_name) {
    if(_parse_tree["VALUES"].size() != _table_map[table_name].field_count())
        return false;
    else
        return true;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Checks if fields or relation fields are valid from SQL table.
 *
 * PRE-CONDITIONS:
 *  const std::string &table_name: table name
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  int: Query code
 ******************************************************************************/
int SQL::is_valid_fields(const std::string &table_name) {
    if(_parse_tree["FIELDS"].size() > _table_map[table_name].field_count())
        return FIELDS_OVERLIMIT;

    if(_parse_tree["FIELDS"][0] != "*" &&
       !_table_map[table_name].is_match_fields(_parse_tree["FIELDS"]))
        return WRONG_FIELDS_NAME;

    if(_parse_tree.contains("WHERE") &&
       !_table_map[table_name].is_match_fields(_parse_tree["R_FIELDS"]))
        return WRONG_FIELDS_NAME;

    return 0;
}

}  // namespace sql
