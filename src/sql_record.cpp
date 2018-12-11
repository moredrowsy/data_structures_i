#include "../include/sql_record.h"

namespace sql {

/*******************************************************************************
 * DESCRIPTION:
 *  Construct record with a char array of REC_SIZE.
 *
 * PRE-CONDITIONS:
 *  const std::string& fname: file name
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
SQLRecord::SQLRecord(const std::string& fname) : _data(nullptr), _fname(fname) {
    _data = new char[REC_SIZE]();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Copy constructor.
 *
 * PRE-CONDITIONS:
 *  const SQLRecord& src: source object
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
SQLRecord::SQLRecord(const SQLRecord& src)
    : _data(nullptr), _fname(src._fname) {
    _data = new char[REC_SIZE]();
    std::memcpy(_data, src._data, REC_SIZE);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Assignment operator.
 *
 * PRE-CONDITIONS:
 *  const SQLRecord& rhs: right hand side object
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  SQLRecord&
 ******************************************************************************/
SQLRecord& SQLRecord::operator=(const SQLRecord& rhs) {
    if(this != &rhs) {
        _fname = rhs._fname;
        std::memcpy(_data, rhs._data, REC_SIZE);
    }
    return *this;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Deallocates char array.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
SQLRecord::~SQLRecord() { delete[] _data; }

/*******************************************************************************
 * DESCRIPTION:
 *  Set new file name.
 *
 * PRE-CONDITIONS:
 *  const std::string& fname: file name
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
void SQLRecord::set_fname(const std::string& fname) { _fname = fname; }

/*******************************************************************************
 * DESCRIPTION:
 *  Read a record data of size REC_SIZE and return by ref to a vector.
 *
 * PRE-CONDITIONS:
 *  std::vector<std::string>& v: empty vector
 *  long rpos                  : record position to seek
 *
 * POST-CONDITIONS:
 *  std::vector<std::string>& v: populated vector for size REC_ROW
 *
 * RETURN:
 *  none
 ******************************************************************************/
std::streamsize SQLRecord::read(std::vector<std::string>& v, long rpos) {
    std::fstream file(_fname.c_str(), std::ios::in | std::ios::binary);
    file.seekg(rpos * REC_SIZE);
    file.read(_data, REC_SIZE);
    std::size_t size = file.gcount() / REC_COL;

    for(std::size_t i = 0; i < size; ++i) v.emplace_back(_data + i * REC_COL);

    return file.gcount();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Write a record data of size REC_SIZE from vector data.
 *
 * PRE-CONDITIONS:
 *  std::vector<std::string>& v: non-empty vector
 *  long rpos                  : record position to seek
 *
 * POST-CONDITIONS:
 *  Data written to file of REC_SIZE at rpos
 *
 * RETURN:
 *  none
 ******************************************************************************/
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
