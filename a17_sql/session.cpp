#include <iostream>          //stream objects
#include "../include/sql.h"  // SQL class

int main() {
    sql::SQL sql;
    sql.load_session();
    sql.run();

    return 0;
}
