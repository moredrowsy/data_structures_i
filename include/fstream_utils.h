/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : fstream_utils
 * DESCRIPTION : This header defines a templated file stream utilities.
 *      Provides calculating file size, counting data elements in file stream,
 *      and verify data elements are sorted.
 ******************************************************************************/
#ifndef FSTREAM_UTILS_H
#define FSTREAM_UTILS_H

#include <fstream>  // ifstream, ofstream
#include <string>   // string

namespace fstream_utils {

// calculate file size in BYTES
std::ifstream::pos_type filesize(std::string fname);

// count data in file stream
template <typename T>
int count_file(std::string fname);

// verify a medium sized file is sorted
template <typename T>
bool verify_sorted_file(std::string fname);

/*******************************************************************************
 * DESCRIPTION:
 *  Reports file size in bytes.
 *
 * PRE-CONDITIONS:
 *  std::string fname: valid file name
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  std::ifstream::pos_type
 ******************************************************************************/
std::ifstream::pos_type filesize(std::string fname) {
    std::ifstream in(fname.c_str(), std::ios::ate | std::ios::binary);
    return in.tellg();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Reports the number of elements in file, delimited by valid whitespace
 *  characters.
 *
 * PRE-CONDITIONS:
 *  std::string fname: valid file name
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  int
 ******************************************************************************/
template <typename T>
int count_file(std::string fname) {
    std::ifstream ins(fname.c_str(), std::ios::binary);

    int count = 0;
    T temp;
    while(ins >> temp) ++count;

    return count;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Verifies the data in file are sorted.
 *
 * PRE-CONDITIONS:
 *  std::string fname: valid file name
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool verify_sorted_file(std::string fname) {
    bool is_sorted = true;
    std::ifstream ins(fname.c_str(), std::ios::binary);
    T prev, current;

    ins >> prev;
    while(ins >> current) {
        if(prev > current) {
            is_sorted = false;
            break;
        }

        prev = current;
    }

    return is_sorted;
}

}  // namespace fstream_utils

#endif  // FSTREAM_UTILS_H
