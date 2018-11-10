/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : fstream_byte_sort
 * DESCRIPTION : This header defines a file stream BYTE sorting.
 ******************************************************************************/
#ifndef FSTREAM_BYTE_SORT_H
#define FSTREAM_BYTE_SORT_H

#include <algorithm>  // sort()
#include <cassert>    // assert()
#include <cstdio>     // remove()
#include <cstring>    // strcmp(), memcpy()
#include <fstream>    // ifstream, ofstream
#include <iostream>   // stream objects
#include <memory>     // shared_ptr
#include <string>     // string, c_str()
#include <vector>     // vector

namespace fstream_byte_sort {

enum { BYTE_SIZE = 1024, MAX_BUFFER = 10000, endl = 10, space = 32, tab = 9 };

struct FSByteHandler {
    // CONSTRUCTORS
    FSByteHandler(std::string name, char **block = nullptr,
                  std::size_t size = 0, std::size_t byte_size = BYTE_SIZE);

    // MOVE SEMANTICS for std::ifstream
    FSByteHandler(FSByteHandler &&) = default;
    FSByteHandler &operator=(FSByteHandler &&) = default;
    ~FSByteHandler();

    // OPERATORS
    explicit operator bool();
    FSByteHandler &operator>>(std::ostream &outs);
    FSByteHandler &operator>>(char *&data);

    // MUTATORS
    void clear();

    // FRIENDS
    friend std::ostream &operator<<(std::ostream &outs, FSByteHandler &f) {
        if(f._ifstream) {
            outs.write(f._data, f._byte_size - 1);  // (size-1) == exclude '\0'

            f._ifstream.read(f._data, f._byte_size - 1);
            f._data[f._byte_size - 1] = '\0';  // add '\0' for cstring
        }
        return outs;
    }

    friend bool operator<(const FSByteHandler &left,
                          const FSByteHandler &right) {
        return std::strcmp(left._data, right._data) < 0;
    }

    std::size_t _byte_size;
    char *_data;
    std::string _name;
    std::ifstream _ifstream;
};

class FStreamByteSort {
public:
    // CONSTRUCTOR & DESTRUCTOR
    FStreamByteSort(std::size_t byte_size = BYTE_SIZE,
                    std::size_t buf_size = MAX_BUFFER,
                    std::string tname = "__temp__");
    ~FStreamByteSort();

    // MUTATORS
    void clear();  // clean up all stream objects and temp files
    void set_max_buffer(std::size_t size);  // change max block size
    void set_temp_name(std::string tname);  // set temp output file prefix

    // FRIENDS
    friend std::istream &operator>>(std::istream &ins, FStreamByteSort &fs) {
        return fs._extractions(ins);
    }

    friend std::ostream &operator<<(std::ostream &outs, FStreamByteSort &fs) {
        return fs._insertions(outs);
    }

private:
    std::size_t _byte_size;  // size of each data segment
    std::size_t _max_buf;    // maximum size for memory allocation
    std::string _tname;      // temporary file prefix
    std::vector<std::shared_ptr<FSByteHandler>>
        _fs_handlers;  // make temp files

    // MUTATORS
    std::istream &_extractions(std::istream &ins);  // read in-stream
    std::ostream &_insertions(std::ostream &outs);  // write out-stream
    void _sort_and_dump(char **block,
                        std::size_t size);  // dump to FSByteHandler
};

// ----- FSByteHandler IMPLEMENTATIONS -----

FSByteHandler::FSByteHandler(std::string name, char **block, std::size_t size,
                             std::size_t byte_size)
    : _byte_size(byte_size),
      _data(new char[_byte_size]),
      _name(name),
      _ifstream() {
    if(block && size) {  // write the rest to temp file
        char *buffer = new char[size * _byte_size];
        std::size_t count = 0;

        std::ofstream outs(_name.c_str(), std::ios::binary | std::ios::trunc);
        for(std::size_t i = 0; i < size; ++i) {
            std::memcpy(buffer + count, block[i], _byte_size - 1);
            count += (_byte_size - 1);
        }
        outs.write(buffer, count);
        outs.close();  // close ofstream writing

        _ifstream.open(_name.c_str(), std::ios::binary);
        _ifstream.read(_data, _byte_size - 1);
        _data[_byte_size - 1] = '\0';  // add '\0' for cstring

        delete[] buffer;
    }
}

FSByteHandler::~FSByteHandler() { clear(); }

FSByteHandler::operator bool() { return _ifstream.good(); }

FSByteHandler &FSByteHandler::operator>>(std::ostream &outs) {
    if(_ifstream) {
        outs.write(_data, _byte_size - 1);  // (size-1) == exclude '\0'

        _ifstream.read(_data, _byte_size - 1);
        _data[_byte_size - 1] = '\0';  // add '\0' for cstring
    }
    return *this;
}

FSByteHandler &FSByteHandler::operator>>(char *&data) {
    if(_ifstream) {  // when ifstream is good
        std::memcpy(data, _data, _byte_size - 1);

        _ifstream.read(_data, _byte_size - 1);
        _data[_byte_size - 1] = '\0';  // add '\0' for cstring
    }
    return *this;
}

void FSByteHandler::clear() {
    _ifstream.close();
    std::remove(_name.c_str());
    delete[] _data;
}

// ----- FStreamByteSort IMPLEMENTATIONS -----

FStreamByteSort::FStreamByteSort(std::size_t byte_size, std::size_t buf_size,
                                 std::string tname)
    : _byte_size(byte_size), _max_buf(buf_size), _tname(tname) {
    assert(_max_buf > 0);
    assert(!_tname.empty());
}

FStreamByteSort::~FStreamByteSort() { clear(); }

void FStreamByteSort::clear() { _fs_handlers.clear(); }

void FStreamByteSort::set_max_buffer(std::size_t size) {
    assert(size > 0);
    _max_buf = size;
}

void FStreamByteSort::set_temp_name(std::string tname) {
    assert(!tname.empty());
    _tname = tname;
}

std::istream &FStreamByteSort::_extractions(std::istream &ins) {
    char **block = nullptr, *buffer = new char[_max_buf * _byte_size];
    std::size_t count, size;

    block = new char *[_max_buf];  // allocate block array
    for(std::size_t i = 0; i < _max_buf; ++i)
        block[i] = new char[_byte_size + 1];

    while(ins) {
        ins.read(buffer, _max_buf * _byte_size);
        size = ins.gcount() / _byte_size;
        count = 0;

        for(std::size_t i = 0; i < size; ++i) {
            std::memcpy(block[i], buffer + count, _byte_size);
            block[i][_byte_size] = '\0';
            count += _byte_size;
        }
        _sort_and_dump(block, size);
    }

    // deallocate
    for(std::size_t j = 0; j < _max_buf; ++j) delete[] block[j];
    delete[] block;
    delete[] buffer;

    return ins;
}

std::ostream &FStreamByteSort::_insertions(std::ostream &outs) {
    char *buffer = new char[_max_buf * _byte_size],
         *data = new char[_byte_size];
    std::size_t count = 0;

    while(_fs_handlers.size()) {  // loop until _fs_handlers are gone
        auto min = std::min_element(_fs_handlers.begin(), _fs_handlers.end(),
                                    [](auto &l, auto &r) { return *l < *r; });

        **min >> data;
        std::memcpy(buffer + count, data, _byte_size);
        count += _byte_size;

        if(count == _max_buf * _byte_size) {  // write when buffer is full
            outs.write(buffer, count);
            count = 0;
        }

        if(!**min)  // erase exhausted FSHandler
            _fs_handlers.erase(min);
    }

    if(count) outs.write(buffer, count);  // write remaining buffer

    // deallocate
    delete[] data;
    delete[] buffer;

    return outs;
}

void FStreamByteSort::_sort_and_dump(char **block, std::size_t size) {
    // generate unique file name
    std::string name = _tname + std::to_string(_fs_handlers.size());

    std::sort(block, block + size, [](const char *a, const char *b) {
        return std::strcmp(a, b) < 0;
    });

    // pass block to FSByteHandler to create temp file
    _fs_handlers.emplace_back(
        new FSByteHandler(name, block, size, _byte_size + 1));
}

}  // namespace fstream_byte_sort

#endif  // FSTREAM_BYTE_SORT_H
