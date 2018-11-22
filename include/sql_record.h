/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : pair
 * DESCRIPTION : This header provides a SQL Record class. It reads/writes
 *      record data at a given position.
 *
 *      BINARY STRUCTURE OF RECORD:
 *      pos | binary data (in bytes)
 *      0x0  | 1'\0'            <-- auto add row 0 by record for valid state
 *      0x1  | Field_name1'\0'
 *      0x2  | Field_name2'\0'
 *       .
 *       .
 *      1x0 | 1'\0'             <-- valid state
 *      1x1 | Field_value1'\0'
 *      1x2 | Field_value2'\0'
 *       .
 *       .
 *      2x0 | 0'\0'             <-- invalid state
 *      2x1 | Field_value1'\0'
 *      2x2 | Field_value2'\0'
 *       .
 *       .
 *      EOF
 ******************************************************************************/
#ifndef SQL_RECORD_H
#define SQL_RECORD_H

#include <cassert>   // assert()
#include <cstring>   // strncpy
#include <fstream>   // file streams
#include <iostream>  //stream
#include <string>    // string
#include <vector>    // vector

namespace sql {

enum RECORD_SIZE { REC_COL = 10, REC_ROW = 5, REC_SIZE = REC_ROW * REC_COL };

class SQLRecord {
public:
    SQLRecord(std::string fname);
    ~SQLRecord();

    void set_fname(std::string fname);  // set file name
    std::streamsize read(std::vector<std::string>& v, long rpos = 0);
    long write(const std::vector<std::string>& v, long rpos = -1);

private:
    char* _data;
    std::string _fname;
};

}  // namespace sql

#endif  // SQL_RECORD_H
