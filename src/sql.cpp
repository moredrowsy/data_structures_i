#include "../include/sql.h"

namespace sql {

SQL::SQL(char *fname) {
    char *buffer = new char[MAX_BUFFER];
    int parse_counts = 0;
    ParseTree parse_tree;
    std::ifstream fin(fname);

    while(fin.getline(buffer, MAX_BUFFER)) {
        _parser.set_string(buffer);
        if(_parser.get_query(parse_tree)) {
            _parse_map[parse_counts++] = parse_tree;
            std::cout << buffer << std::endl;
            std::cout << _parse_map[parse_counts - 1] << std::endl << std::endl;
            parse_tree.clear();
        }
    }
    std::cout << _parse_map.size() << std::endl;

    delete[] buffer;
}

}  // namespace sql
