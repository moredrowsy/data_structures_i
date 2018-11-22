#include "../include/sql_record.h"

namespace sql {

SQLRecord::SQLRecord(std::string fname) : _fname(fname) {
    _data = new char[REC_ROW * REC_COL]();
}

SQLRecord::~SQLRecord() { delete[] _data; }

void SQLRecord::set_fname(std::string fname) { _fname = fname; }

std::streamsize SQLRecord::read(std::vector<std::string>& v, long rpos) {
    std::fstream file(_fname.c_str(), std::ios::in | std::ios::binary);
    file.seekg(rpos * REC_SIZE);
    file.read(_data, REC_SIZE);
    std::size_t size = file.gcount() / REC_COL;

    for(std::size_t i = 0; i < size; ++i) v.emplace_back(_data + i * REC_COL);

    return file.gcount();
}

long SQLRecord::write(const std::vector<std::string>& v, long rpos) {
    assert(v.size() <= REC_ROW);
    std::string value;
    std::size_t size = v.size();
    long original_pos;

    for(std::size_t i = 0; i < size; ++i) {
        value = v[i].substr(0, REC_COL - 1);
        std::strncpy(_data + i * REC_COL, value.c_str(), REC_COL);
    }

    std::fstream file(_fname.c_str(),
                      std::ios::in | std::ios::out | std::ios::binary);

    if(rpos < 0)
        file.seekp(0, file.end);  // if rpos is negative, append to file
    else                          // else seek to record position
        file.seekp(rpos * REC_SIZE);

    original_pos = file.tellp();
    file.write(_data, REC_SIZE);

    return original_pos / REC_SIZE;
}

}  // namespace sql
