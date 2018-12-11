#include <iostream>          //stream objects
#include "../include/sql.h"  // SQL class

int main() {
    sql::SQL sql1("_!sample.txt");
    sql1.save_session();
    std::cout << "Batch file process completed for sql1." << std::endl
              << std::endl;

    std::cout << "Executing SQL::run() for sql2." << std::endl;
    sql::SQL sql2;
    sql2.load_session();
    sql2.run();

    return 0;
}
