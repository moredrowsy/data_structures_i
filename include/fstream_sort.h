/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : stream_sort
 * DESCRIPTION : This header defines a templated fstream sorting.
 *      FStreamSort first takes in an istream object via operator extraction,
 *      where data are read and then dumps their contents to FStreamData
 *objects. FStreamData takes a buffer of data and write to temporary files. It's
 *attribute, _data, holds the first data element of the buffer. Internally, it
 *creates an ifstream object linked to the temp file and continuously reads its
 *content into _data. FStreamSort's finally writes the output data via operator
 *insertion. Internally, it cycles through all of FStreamData objects, find the
 *      mininum value and write to the output file.
 ******************************************************************************/
#ifndef FSTREAM_SORT_H
#define FSTREAM_SORT_H

#include <algorithm>  // sort()
#include <cassert>    // assert()
#include <cstdio>     // remove()
#include <cstring>    // c_str()
#include <fstream>    // ifstream, ofstream
#include <iostream>   // stream objects
#include <string>     // string objects
#include <vector>     // vector objects

namespace fstream_sort {

enum { MAX_BLOCK = 250000, endl = 10, space = 32, tab = 9 };

template <typename T>
struct FStreamData {
    // CONSTRUCTORS
    FStreamData(std::string name, T *block = nullptr, std::size_t size = 0,
                char delim = char(endl));

    // MUTATORS
    explicit operator bool();
    FStreamData<T> &operator>>(std::ostream &outs);
    FStreamData<T> &operator>>(T &data);
    void clear();
    bool set_data(T *block = nullptr, std::size_t size = 0,
                  char delim = char(endl));

    // FRIENDS
    friend std::ostream &operator<<(std::ostream &outs, FStreamData &f) {
        if(f._ifstream) {
            outs << f._data;
            f._ifstream >> f._data;
        }
        return outs;
    }

    friend bool operator==(const FStreamData &left, const FStreamData &right) {
        return left._data == right._data;
    }

    friend bool operator!=(const FStreamData &left, const FStreamData &right) {
        return left._data != right._data;
    }

    friend bool operator<(const FStreamData &left, const FStreamData &right) {
        return left._data < right._data;
    }

    friend bool operator<=(const FStreamData &left, const FStreamData &right) {
        return left._data <= right._data;
    }

    friend bool operator>(const FStreamData &left, const FStreamData &right) {
        return left._data > right._data;
    }

    friend bool operator>=(const FStreamData &left, const FStreamData &right) {
        return left._data >= right._data;
    }

    char _delim;
    std::string _name;
    std::ifstream _ifstream;  // must initialize with existing file or fail!
    T _data;
};

template <typename T>
class FStreamSort {
public:
    // CONSTRUCTOR & DESTRUCTOR
    FStreamSort(std::size_t s = MAX_BLOCK, short delim = endl,
                std::string tname = "__temp__");
    ~FStreamSort();

    // MUTATORS
    void cleanup();  // clean up all stream objects and temp files
    void set_buffer(std::size_t size);      // change max block size
    void set_delim(short delim);            // change delimiter between data
    void set_temp_name(std::string tname);  // set temp output file prefix

    // FRIENDS
    friend std::istream &operator>>(std::istream &ins, FStreamSort<T> &fs) {
        return fs._extractions(ins);
    }

    friend std::ostream &operator<<(std::ostream &outs, FStreamSort<T> &fs) {
        return fs._insertions(outs);
    }

private:
    char _delim;
    std::size_t _buffer_size;  // maximum size for memory allocation
    std::string _tname;        // temporary file prefix
    std::vector<FStreamData<T>>
        _file_infos;  // has file name, data and ifstream

    // MUTATORS
    std::istream &_extractions(std::istream &ins);  // read in-stream
    std::ostream &_insertions(std::ostream &outs);  // write out-stream
    // output a block of data to a unique temporary file
    inline void _dump_block(T *block, std::size_t size);
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
FStreamData<T>::FStreamData(std::string name, T *block, std::size_t size,
                            char delim)
    : _delim(delim), _name(name), _ifstream(), _data() {
    if(block && size) {
        _data = block[0];
        std::ofstream outs(_name, std::ios::binary | std::ios::trunc);
        for(std::size_t i = 1; i < size; ++i) outs << block[i] << _delim;
        outs.close();
    }

    _ifstream.open(_name.c_str(), std::ios::binary);
}

template <typename T>
FStreamData<T>::operator bool() {
    return _ifstream.good();
}

template <typename T>
FStreamData<T> &FStreamData<T>::operator>>(std::ostream &outs) {
    if(_ifstream) {
        outs << _data;
        _ifstream >> _data;
    }
    return *this;
}

template <typename T>
FStreamData<T> &FStreamData<T>::operator>>(T &data) {
    if(_ifstream) {
        data = _data;
        _ifstream >> _data;
    }
    return *this;
}

template <typename T>
void FStreamData<T>::clear() {
    _ifstream.close();
    std::remove(_name.c_str());
}

template <typename T>
bool FStreamData<T>::set_data(T *block, std::size_t size, char delim) {
    _ifstream.close();
    std::remove(_name.c_str());

    if(block && size) {
        _data = block[0];
        std::ofstream outs(_name, std::ios::binary | std::ios::trunc);
        for(std::size_t i = 1; i < size; ++i) outs << block[i] << _delim;
        outs.close();
    }

    _ifstream.open(_name.c_str(), std::ios::binary);

    return _ifstream.good();
}

template <typename T>
FStreamSort<T>::FStreamSort(std::size_t s, short delim, std::string tname)
    : _delim(delim), _buffer_size(s), _tname(tname) {
    assert(delim == endl || delim == space || delim == tab);
    assert(_buffer_size > 0);
    assert(!_tname.empty());
}

template <typename T>
FStreamSort<T>::~FStreamSort() {
    cleanup();
}

template <typename T>
void FStreamSort<T>::cleanup() {
    for(auto &a : _file_infos) {
        a._ifstream.close();
        std::remove(a._name.c_str());
    }
    _file_infos.clear();
}

template <typename T>
void FStreamSort<T>::set_buffer(std::size_t size) {
    assert(size > 0);
    _buffer_size = size;
}

template <typename T>
void FStreamSort<T>::set_delim(short delim) {
    assert(delim == endl || delim == space || delim == tab);
    _delim = char(delim);
}

template <typename T>
void FStreamSort<T>::set_temp_name(std::string tname) {
    assert(!tname.empty());
    _tname = tname;
}

template <typename T>
std::istream &FStreamSort<T>::_extractions(std::istream &ins) {
    std::size_t i = 0;
    T *block = nullptr;

    block = new T[_buffer_size];

    while(ins >> block[i]) {
        ++i;

        if(i == _buffer_size) {     // when block is full
            _dump_block(block, i);  // sort and write block
            i = 0;
        }
    }
    if(i) _dump_block(block, i);  // sort and write last block if has data

    delete[] block;

    return ins;
}

template <typename T>
std::ostream &FStreamSort<T>::_insertions(std::ostream &outs) {
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

        outs << _file_infos[min_index] << _delim;  // write data @ min index

        if(!_file_infos[min_index]) {  // remove file and delete FStreamData
            _file_infos[min_index]._ifstream.close();
            std::remove(_file_infos[min_index]._name.c_str());
            _file_infos.erase(_file_infos.begin() + min_index);
        }
    }

    return outs;
}

template <typename T>
void FStreamSort<T>::_dump_block(T *block, std::size_t size) {
    // generate unique file name
    std::string name = _tname + std::to_string(_file_infos.size());

    std::sort(block, block + size);  // sort block before passing to FStreamData

    // pass block of data to FStreamData to create temp file
    _file_infos.emplace_back(name, block, size, _delim);
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

}  // namespace fstream_sort

#endif  // FSTREAM_SORT_H
