/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : file_sort
 * DESCRIPTION : This header defines a templated very large file data sort.
 *      It takes an istream object and splits it into temporary files.
 *      It then takes in an ostream object, read the asscoated temporary
 *      files and return sorted items to ostream.
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

template <typename T>
struct FileInfo {
    // CONSTRUCTORS
    FileInfo(std::string name);

    // MUTATORS
    bool operator>>(std::ostream &outs);

    friend std::ostream &operator<<(std::ostream &outs, const FileInfo &f) {
        return outs << f._data;
    }

    friend bool operator==(const FileInfo &left, const FileInfo &right) {
        return left._data == right._data;
    }

    friend bool operator!=(const FileInfo &left, const FileInfo &right) {
        return left._data != right._data;
    }

    friend bool operator<(const FileInfo &left, const FileInfo &right) {
        return left._data < right._data;
    }

    friend bool operator<=(const FileInfo &left, const FileInfo &right) {
        return left._data <= right._data;
    }

    friend bool operator>(const FileInfo &left, const FileInfo &right) {
        return left._data > right._data;
    }

    friend bool operator>=(const FileInfo &left, const FileInfo &right) {
        return left._data >= right._data;
    }

    std::string _name;
    std::ifstream _if;  // must initialize with existing file or fail!
    T _data;
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
    std::size_t _buffer_size;              // maximum size for memory allocation
    std::string _tname;                    // temporary file prefix
    std::vector<FileInfo<T>> _file_infos;  // has file name, data and ifstream

    // MUTATORS
    std::istream &_extractions(std::istream &ins);  // read in-stream
    std::ostream &_insertions(std::ostream &outs);  // write out-stream
    // output a block of data to a unique temporary file
    void _output_block(T *block, std::size_t size);
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
FileInfo<T>::FileInfo(std::string name)
    : _name(name), _if(_name, std::ios::binary), _data() {
    assert(_if.is_open());
    _if >> _data;
}

template <typename T>
bool FileInfo<T>::operator>>(std::ostream &outs) {
    if(_if) {
        outs << _data;
        _if >> _data;
    }

    return (bool)_if;
}

template <typename T>
FileSort<T>::FileSort(std::size_t s, std::string tname)
    : _buffer_size(s), _tname(tname) {
    assert(_buffer_size > 0);
    assert(!_tname.empty());
}

template <typename T>
FileSort<T>::~FileSort() {
    cleanup();
}

template <typename T>
void FileSort<T>::cleanup() {
    for(auto &a : _file_infos) std::remove(a._name.c_str());  // delete all
    _file_infos.clear();
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
    std::size_t i = 0;
    T *block = nullptr;

    block = new T[_buffer_size];

    while(ins >> block[i]) {
        ++i;

        if(i == _buffer_size) {       // when block is full
            _output_block(block, i);  // sort and output block
            i = 0;
        }
    }
    if(i) _output_block(block, i);  // sort and output last block if has data

    delete[] block;

    return ins;
}

template <typename T>
std::ostream &FileSort<T>::_insertions(std::ostream &outs) {
    std::size_t min_index;
    T current, min;

    while(_file_infos.size()) {  // loop until _file_infos are gone
        current = _file_infos[0]._data;
        min = current;
        min_index = 0;

        for(std::size_t i = 1; i < _file_infos.size(); ++i) {  // find min index
            current = _file_infos[i]._data;

            if(current < min) {
                min = current;
                min_index = i;
            }
        }

        if(_file_infos[min_index] >> outs)  // output data @ min index
            outs << std::endl;
        else {  // remove file and delete associated FileInfo if >> fails
            std::remove(_file_infos[min_index]._name.c_str());
            _file_infos.erase(_file_infos.begin() + min_index);
            outs << std::endl;
        }
    }

    return outs;
}

template <typename T>
void FileSort<T>::_output_block(T *block, std::size_t size) {
    // generate unique file name
    std::string name = _tname + std::to_string(_file_infos.size());

    std::sort(block, block + size);  // sort block before output

    std::ofstream fout(name, std::ios::binary | std::ios::trunc);
    for(std::size_t i = 0; i < size; ++i) fout << block[i] << std::endl;
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
