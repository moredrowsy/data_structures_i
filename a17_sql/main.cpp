#include <iostream>          //stream objects
#include "../include/sql.h"  // SQL class

int main() {
    char buffer[] =
        "seLect _lName, f_name, Age, Sex, '_Comments_1', \"D_Quotes_2\" "
        "from Student";
    char buffer2[] = "SeLect * from STUDENT";
    char buffer3[] =
        "creaTE Student _lName, f_name, Age, Sex, '_Comments_1', "
        "\"D_Quotes_2\" ";
    char buffer4[] = "insert into Student values 'Smith', John, 35, M";
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
    parse_tree.clear();

    parser.set_string(buffer3);
    std::cout << std::endl << std::endl;
    std::cout << "BUFFER:     " << buffer3 << std::endl;
    std::cout << "Valid query? " << parser.get_query(parse_tree) << std::endl;
    std::cout << "MAP IS: " << std::endl;
    std::cout << parse_tree << std::endl;
    parse_tree.clear();

    parser.set_string(buffer4);
    std::cout << std::endl << std::endl;
    std::cout << "BUFFER:     " << buffer4 << std::endl;
    std::cout << "Valid query? " << parser.get_query(parse_tree) << std::endl;
    std::cout << "MAP IS: " << std::endl;
    std::cout << parse_tree << std::endl;
    parse_tree.clear();

    return 0;
}
