/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : stream_sort
 * DESCRIPTION : This header defines a templated fstream sorting.
 *      FStreamSort first takes in an istream object via operator extraction,
 *      where data are read and then dumps their contents to FSHandler objects.
 *      FSHandler takes a buffer of data and write to temporary files. It's
 *      attribute, _data, holds the first data element of the buffer.
 *      Internally, it creates an ifstream object linked to the temp file and
 *      continuously reads its content into _data.
 *      FStreamSort's finally writes the output data via operator insertion.
 *      Internally, it cycles through  all of FSHandler objects, find the
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
#include <memory>     // shared_ptr
#include <string>     // string objects
#include <vector>     // vector objects

namespace fstream_sort {

enum { MAX_BLOCK = 250000, endl = 10, space = 32, tab = 9 };

template <typename T>
struct FSHandler {
    // CONSTRUCTORS
    FSHandler(std::string name, T *buffer = nullptr, std::size_t size = 0,
              char delim = char(endl));

    // MOVE SEMANTICS for std::ifstream
    FSHandler(FSHandler &&) = default;
    FSHandler &operator=(FSHandler &&) = default;
    ~FSHandler();

    // MUTATORS
    explicit operator bool();
    FSHandler<T> &operator>>(std::ostream &outs);
    FSHandler<T> &operator>>(T &data);
    void clear();
    bool set_data(T *buffer = nullptr, std::size_t size = 0,
                  char delim = char(endl));

    // FRIENDS
    friend std::ostream &operator<<(std::ostream &outs, FSHandler &f) {
        if(f._ifstream) {
            outs << f._data;
            f._ifstream >> f._data;
        }
        return outs;
    }

    friend bool operator<(const FSHandler &left, const FSHandler &right) {
        return left._data < right._data;
    }

    char _delim;
    std::string _name;
    std::ifstream _ifstream;
    T _data;
};

template <typename T>
class FStreamSort {
public:
    // CONSTRUCTOR & DESTRUCTOR
    FStreamSort(std::size_t size = MAX_BLOCK, short delim = endl,
                std::string tname = "__temp__");
    ~FStreamSort();

    // MUTATORS
    void cleanup();  // clean up all stream objects and temp files
    void set_max_buffer(std::size_t size);  // change max buffer size
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
    std::size_t _max_buf;  // maximum size for memory allocation
    std::string _tname;    // temporary file prefix
    std::vector<std::shared_ptr<FSHandler<T>>> _fs_handlers;  // make temp files

    // MUTATORS
    std::istream &_extractions(std::istream &ins);  // read in-stream
    std::ostream &_insertions(std::ostream &outs);  // write out-stream
    // sort and dump data to FSHandler objects, which writes temp files
    void _sort_and_dump(T *buffer, std::size_t size);
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

// ----- FSHandler -----

template <typename T>
FSHandler<T>::FSHandler(std::string name, T *buffer, std::size_t size,
                        char delim)
    : _delim(delim), _name(name), _ifstream(), _data() {
    if(buffer && size) {
        _data = buffer[0];  // store first data element

        // write the rest to temp file
        std::ofstream outs(_name.c_str(), std::ios::binary | std::ios::trunc);
        for(std::size_t i = 1; i < size; ++i) outs << buffer[i] << _delim;
        outs.close();  // close ofstream writing

        _ifstream.open(_name.c_str(), std::ios::binary);  // open ifs reading
    }
}

template <typename T>
FSHandler<T>::~FSHandler() {
    clear();
}

template <typename T>
FSHandler<T>::operator bool() {
    return _ifstream.good();
}

template <typename T>
FSHandler<T> &FSHandler<T>::operator>>(std::ostream &outs) {
    if(_ifstream) {          // when ifstream is good
        outs << _data;       // insert current data
        _ifstream >> _data;  // extract next data
    }
    return *this;
}

template <typename T>
FSHandler<T> &FSHandler<T>::operator>>(T &data) {
    if(_ifstream) {          // when ifstream is good
        data = _data;        // insert current data
        _ifstream >> _data;  // extract next data
    }
    return *this;
}

template <typename T>
void FSHandler<T>::clear() {
    _ifstream.close();
    std::remove(_name.c_str());
}

template <typename T>
bool FSHandler<T>::set_data(T *buffer, std::size_t size, char delim) {
    _delim = delim;
    _ifstream.close();
    std::remove(_name.c_str());

    if(buffer && size) {
        _data = buffer[0];  // store first data element

        // write the rest to temp file
        std::ofstream outs(_name.c_str(), std::ios::binary | std::ios::trunc);
        for(std::size_t i = 1; i < size; ++i) outs << buffer[i] << _delim;
        outs.close();
    }

    _ifstream.open(_name.c_str(), std::ios::binary);  // open associated file

    return _ifstream.good();
}

// ----- FStreamSort -----

template <typename T>
FStreamSort<T>::FStreamSort(std::size_t size, short delim, std::string tname)
    : _delim(delim), _max_buf(size), _tname(tname) {
    assert(delim == endl || delim == space || delim == tab);
    assert(_max_buf > 0);
    assert(!_tname.empty());
}

template <typename T>
FStreamSort<T>::~FStreamSort() {
    cleanup();
}

template <typename T>
void FStreamSort<T>::cleanup() {
    _fs_handlers.clear();
}

template <typename T>
void FStreamSort<T>::set_max_buffer(std::size_t size) {
    assert(size > 0);
    _max_buf = size;
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
    T *buffer = nullptr;

    buffer = new T[_max_buf];

    while(ins >> buffer[i++]) {         // inc i after read
        if(i == _max_buf) {             // when buffer is full
            _sort_and_dump(buffer, i);  // sort and dump buffer
            i = 0;
        }
    }

    // dec i to reverse previous inc; sort and dump last buffer if has data
    if(--i) _sort_and_dump(buffer, i);

    delete[] buffer;

    return ins;
}

template <typename T>
std::ostream &FStreamSort<T>::_insertions(std::ostream &outs) {
    std::size_t min_index;

    while(_fs_handlers.size()) {  // loop until _fs_handlers are gone
        min_index = 0;

        for(std::size_t i = 1; i < _fs_handlers.size(); ++i)  // find min
            if(_fs_handlers[i]->_data < _fs_handlers[min_index]->_data)
                min_index = i;

        outs << *_fs_handlers[min_index] << _delim;  // write data @ min index

        if(!*_fs_handlers[min_index])  // erase exhausted FSHandler
            _fs_handlers.erase(_fs_handlers.begin() + min_index);
    }

    return outs;
}

template <typename T>
void FStreamSort<T>::_sort_and_dump(T *buffer, std::size_t size) {
    // generate unique file name
    std::string name = _tname + std::to_string(_fs_handlers.size());

    std::sort(buffer, buffer + size);  // sort before passing to FSHandler

    // pass buffer to FSHandler to create temp file
    _fs_handlers.emplace_back(new FSHandler<T>(name, buffer, size, _delim));
}

// ----- Misc public helpers -----

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
