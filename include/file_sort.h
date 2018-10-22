/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : file_sort
 * DESCRIPTION : This header defines a templated file data sort. It takes in an
 *      ifstream object and parses it into temporary files. It then takes in
 *      an ofstream object and return sorted items to it.
 *
 *      NOTE: Fstream objects must be opened in binary mode, std::ios::binary!
 *            Under Windows, opening in text mode will not have 1:1 read
 *            chars:position. Thus text mode give seek position.
 ******************************************************************************/
#ifndef FILE_SORT_H
#define FILE_SORT_H

#include <algorithm>  // sort()
#include <cassert>    // assert()
#include <cstdio>     // remove()
#include <cstring>    // c_str()
#include <fstream>    // ifstream, ofstream
#include <iostream>   // stream objects
#include <string>     // string objects
#include <vector>     // vector objects

namespace file_sort {

struct FileInfo {
    // CONSTRUCTORS
    FileInfo(std::string name, std::streampos fpos = 0)
        : _name(name), _if(_name, std::ios::binary), _fpos(fpos) {
        assert(_if.is_open());
    }

    // FRIENDS
    friend std::ostream &operator<<(std::ostream &outs, const FileInfo &f) {
        return outs << f._name << ":" << f._fpos;
    }

    std::string _name;
    std::ifstream _if;  // must initialize with existing file or fail!
    std::streampos _fpos;
};

template <typename T>
class FileSort {
public:
    enum { MAX_BLOCK = 250000 };

    // CONSTRUCTOR & DESTRUCTOR
    FileSort(std::size_t s = MAX_BLOCK, std::string tname = "__temp");
    ~FileSort();

    // MUTATORS
    void cleanup();  // clean up all stream objects and temp files
    void set_buffer(std::size_t size);      // change max block size
    void set_temp_name(std::string tname);  // set temp output file prefix

    // FRIENDS
    friend std::istream &operator>>(std::istream &ins, FileSort<T> &fs) {
        return fs._extractions(ins);
    }

    friend std::ostream &operator<<(std::ostream &outs, FileSort<T> &fs) {
        return fs._insertions(outs);
    }

private:
    std::size_t _buffer_size;           // maximum size for memory allocation
    std::size_t _tsize;                 // number of temporary file outputs
    std::string _tname;                 // temporary file prefix
    std::vector<FileInfo> _file_infos;  // holds file name, pos and ifstream

    // MUTATORS
    std::istream &_extractions(std::istream &ins);  // read in-stream
    std::ostream &_insertions(std::ostream &outs);  // write out-stream
    // output a block of data to a unique temporary file
    void _output_block(int *block, std::size_t size);
};

// public helper function to determine file size in BYTES
std::ifstream::pos_type filesize(std::string fname);

// public helper function to count data in file stream
template <typename T>
int count_file(std::string fname);

// public helper function to verify a medium sized file is sorted
// for testing purposes
template <typename T>
bool validate_sorted_file(std::string fname);

template <typename T>
FileSort<T>::FileSort(std::size_t s, std::string tname)
    : _buffer_size(s), _tsize(0), _tname(tname) {
    assert(_buffer_size > 0);
    assert(!_tname.empty());
}

template <typename T>
FileSort<T>::~FileSort() {
    cleanup();
}

template <typename T>
void FileSort<T>::cleanup() {
    for(auto &a : _file_infos) std::remove(a._name.c_str());  // delete all temp
    _file_infos.clear();
    _tsize = 0;
}

template <typename T>
void FileSort<T>::set_buffer(std::size_t size) {
    assert(size > 0);
    _buffer_size = size;
}

template <typename T>
void FileSort<T>::set_temp_name(std::string tname) {
    assert(!tname.empty());
    _tname = tname;
}

template <typename T>
std::istream &FileSort<T>::_extractions(std::istream &ins) {
    int i = 0;
    T *block = nullptr;

    block = new T[_buffer_size];

    while(ins >> block[i]) {
        ++i;

        if(i == (int)_buffer_size) {  // file output if block is full
            _output_block(block, i);
            i = 0;
        }
    }
    if(i) _output_block(block, i);  // output last block if has data

    delete[] block;

    return ins;
}

template <typename T>
std::ostream &FileSort<T>::_insertions(std::ostream &outs) {
    std::size_t min_index = 0;
    std::streampos fpos = -1;
    T current, min;

    while(_file_infos.size()) {  // loop until _file_infos is size zero
        // read first data to current
        _file_infos[0]._if.seekg(_file_infos[0]._fpos);
        _file_infos[0]._if >> current;

        // initialize min, min_index and store fpos to FileInfo
        min = current;
        min_index = 0;
        fpos = _file_infos[0]._if.tellg();

        // parse every FileInfo items in vector
        for(std::size_t i = 1; i < _file_infos.size(); ++i) {
            _file_infos[i]._if.seekg(_file_infos[i]._fpos);
            _file_infos[i]._if >> current;

            // store min
            if(current < min) {
                min = current;
                min_index = i;
                fpos = _file_infos[i]._if.tellg();
            }
        }

        // add min to output file and update fpos to min_index's FileInfo
        outs << min << std::endl;
        _file_infos[min_index]._fpos = fpos;

        // remove block file if file read fails via fpos -1 or 0
        if(fpos < 1) {
            std::remove(_file_infos[min_index]._name.c_str());
            _file_infos.erase(_file_infos.begin() + min_index);
            --_tsize;
        }
    }

    return outs;
}

template <typename T>
void FileSort<T>::_output_block(int *block, std::size_t size) {
    // generate unique block file name
    std::string name = _tname + "." + std::to_string(_tsize++);

    std::sort(block, block + size);  // sort block

    std::ofstream fout(name, std::ios::binary | std::ios::trunc);
    for(std::size_t i = 0; i < size; ++i) {
        fout << block[i];

        if(i != size - 1) fout << std::endl;
    }
    fout.close();

    _file_infos.push_back(name);  // must push back name after file creation!!!
}

std::ifstream::pos_type filesize(std::string fname) {
    std::ifstream in(fname.c_str(), std::ios::ate | std::ios::binary);
    return in.tellg();
}

template <typename T>
int count_file(std::string fname) {
    std::ifstream ins(fname.c_str(), std::ios::binary);

    int count = 0;
    T temp;
    while(ins >> temp) ++count;

    return count;
}

template <typename T>
bool validate_sorted_file(std::string fname) {
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

}  // namespace file_sort

#endif  // FILE_SORT_H
