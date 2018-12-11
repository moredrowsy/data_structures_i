#include <iostream>          //stream objects
#include "../include/sql.h"  // SQL class

int main() {
    sql::SQL sql("_!sample.txt");
    sql.save_session();

    return 0;
}
