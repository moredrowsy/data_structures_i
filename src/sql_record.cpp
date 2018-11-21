#include "../include/sql_record.h"

namespace sql {

SQLRecord::SQLRecord(std::string fname) : _fname(fname) {
    _data = new char[REC_ROW * REC_COL]();
}

SQLRecord::~SQLRecord() { delete[] _data; }

bool SQLRecord::set_fname(std::string fname) {
    return (bool)std::ifstream(fname.c_str(), std::ios::binary);
}

std::streamsize SQLRecord::read(char* buffer, long rpos) {
    std::fstream file(_fname.c_str(), std::ios::in | std::ios::binary);
    file.seekg(rpos * REC_ROW * REC_COL);
    file.read(buffer, REC_ROW * REC_COL);

    return file.gcount();
}

long SQLRecord::write(std::vector<std::string>& v, long rpos) {
    assert(v.size() <= REC_ROW);
    for(std::size_t i = 0; i < v.size(); ++i)
        std::strncpy(_data + i * REC_COL, v[i].c_str(), REC_COL);

    std::fstream file(_fname.c_str(),
                      std::ios::in | std::ios::out | std::ios::binary);
    if(rpos < 0)
        file.seekp(0, file.end);  // if rpos is negative, append to file
    else                          // else seek to record position
        file.seekp(rpos * REC_ROW * REC_COL);

    file.write(_data, REC_ROW * REC_COL);

    return file.tellp() / (long)(REC_ROW * REC_COL);
}

}  // namespace sql
