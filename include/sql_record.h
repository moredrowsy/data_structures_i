#ifndef SQL_RECORD_H
#define SQL_RECORD_H

#include <cassert>   // assert()
#include <cstring>   // strncpy
#include <fstream>   // file streams
#include <iostream>  //stream
#include <string>    // string
#include <vector>    // vector

namespace sql {

enum RECORD_SIZE { REC_COL = 10, REC_ROW = 5 };

class SQLRecord {
public:
    SQLRecord(std::string fname);
    ~SQLRecord();

    bool set_fname(std::string fname);                        // re valid file?
    std::streamsize read(char* buffer, long rpos = 0);        // re read count
    long write(std::vector<std::string>& v, long rpos = -1);  // re rec index

private:
    char* _data;
    std::string _fname;
};

}  // namespace sql

#endif  // SQL_RECORD_H
