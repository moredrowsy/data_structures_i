#include <iostream>          //stream objects
#include "../include/sql.h"  // SQL class

int main() {
    char buffer[] =
        "seLect L_Name1, _FName2, Age, Sex, '_Comments_123', \"Quotes_123\" "
        "from Student";
    char buffer2[] = "SeLect * from STUDENT";
    sql::SQL sql;
    token::Token t;
    sql::SQLParser parser(buffer);
    bpt_map::MMap<std::string, std::string> parse_tree;

    std::cout << "BUFFER:     " << buffer << std::endl;
    std::cout << "Valid query? " << parser.get_query(parse_tree) << std::endl;
    std::cout << "MAP IS: " << std::endl;
    std::cout << parse_tree << std::endl;
    parse_tree.clear();

    parser.set_string(buffer2);
    std::cout << std::endl << std::endl;
    std::cout << "BUFFER:     " << buffer2 << std::endl;
    std::cout << "Valid query? " << parser.get_query(parse_tree) << std::endl;
    std::cout << "MAP IS: " << std::endl;
    std::cout << parse_tree << std::endl;

    return 0;
}
