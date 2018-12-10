#include <iostream>          //stream objects
#include "../include/sql.h"  // SQL class

int main() {
    sql::SQL sql("commands.txt");
    sql.run();

    sql::SQL sql1;
    sql1.load_session("session");
    sql1.run();

    return 0;
}
