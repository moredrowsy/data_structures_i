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
#include <cstring>    // strcmp()
#include <fstream>    // ifstream, ofstream
#include <iostream>   // stream objects
#include <memory>     // shared_ptr
#include <string>     // string, c_str()
#include <vector>     // vector

namespace fstream_byte_sort {

enum { DATA_SIZE = 1024, MAX_BUFFER = 10000, endl = 10, space = 32, tab = 9 };

struct FSByteHandler {
    // CONSTRUCTORS
    FSByteHandler(std::string name, char **buffer = nullptr,
                  std::size_t size = 0, std::size_t _data_size = DATA_SIZE,
                  char delim = char(endl));

    // MOVE SEMANTICS for std::ifstream
    FSByteHandler(FSByteHandler &&) = default;
    FSByteHandler &operator=(FSByteHandler &&) = default;
    ~FSByteHandler();

    // OPERATORS
    explicit operator bool();
    FSByteHandler &operator>>(std::ostream &outs);
    FSByteHandler &operator>>(char *data);

    // MUTATORS
    void clear();
    bool set_data(char **buffer = nullptr, std::size_t size = 0,
                  std::size_t _data_size = DATA_SIZE, char delim = char(endl));

    // FRIENDS
    friend std::ostream &operator<<(std::ostream &outs, FSByteHandler &f) {
        if(f._ifstream) {
            f._data[f._data_size] = f._delim;  // replace '\0' with delim
            outs.write(f._data, f._data_size + 1);

            f._ifstream.read(f._data, f._data_size + 1);
            f._data[f._data_size] = '\0';  // replace delim with '\0'
        }
        return outs;
    }

    friend bool operator<(const FSByteHandler &left,
                          const FSByteHandler &right) {
        return std::strcmp(left._data, right._data) < 0;
    }

    char _delim;
    char *_data;
    std::size_t _data_size;
    std::string _name;
    std::ifstream _ifstream;
};

class FStreamByteSort {
public:
    // CONSTRUCTOR & DESTRUCTOR
    FStreamByteSort(std::size_t size = DATA_SIZE,
                    std::size_t buf_size = MAX_BUFFER, short delim = endl,
                    std::string tname = "__temp__");
    ~FStreamByteSort();

    // MUTATORS
    void clear();  // clean up all stream objects and temp files
    void set_max_buffer(std::size_t size);  // change max buffer size
    void set_delim(short delim);            // change delimiter between data
    void set_temp_name(std::string tname);  // set temp output file prefix

    // FRIENDS
    friend std::istream &operator>>(std::istream &ins, FStreamByteSort &fs) {
        return fs._extractions(ins);
    }

    friend std::ostream &operator<<(std::ostream &outs, FStreamByteSort &fs) {
        return fs._insertions(outs);
    }

private:
    char _delim;
    std::size_t _data_size;  // size of each data segment
    std::size_t _max_buf;    // maximum size for memory allocation
    std::string _tname;      // temporary file prefix
    std::vector<std::shared_ptr<FSByteHandler>>
        _fs_handlers;  // make temp files

    // MUTATORS
    std::istream &_extractions(std::istream &ins);  // read in-stream
    std::ostream &_insertions(std::ostream &outs);  // write out-stream
    void _sort_and_dump(char **buffer,
                        std::size_t size);  // dump to FSByteHandler
};

// ----- FSByteHandler IMPLEMENTATIONS -----

FSByteHandler::FSByteHandler(std::string name, char **buffer, std::size_t size,
                             std::size_t data_size, char delim)
    : _delim(delim),
      _data(new char[data_size + 1]),
      _data_size(data_size),
      _name(name),
      _ifstream() {
    if(buffer && size) {  // write the rest to temp file
        std::ofstream outs(name.c_str(), std::ios::binary | std::ios::trunc);
        for(std::size_t i = 0; i < size; ++i) {
            buffer[i][_data_size] = _delim;
            outs.write(buffer[i], _data_size + 1);
        }
        outs.close();  // close ofstream writing

        _ifstream.open(_name.c_str(), std::ios::binary);
        _ifstream.read(_data, _data_size + 1);
        _data[_data_size] = '\0';
    }
}

FSByteHandler::~FSByteHandler() { clear(); }

FSByteHandler::operator bool() { return _ifstream.good(); }

FSByteHandler &FSByteHandler::operator>>(std::ostream &outs) {
    if(_ifstream) {
        _data[_data_size] = _delim;  // replace '\0' with delim
        outs.write(_data, _data_size + 1);

        _ifstream.read(_data, _data_size + 1);
        _data[_data_size] = '\0';  // replace delim with '\0'
    }
    return *this;
}

FSByteHandler &FSByteHandler::operator>>(char *data) {
    if(_ifstream) {    // when ifstream is good
        data = _data;  // insert current data

        _ifstream.read(_data, _data_size + 1);
        _data[_data_size] = '\0';  // replace delim with '\0'
    }
    return *this;
}

void FSByteHandler::clear() {
    _ifstream.close();
    std::remove(_name.c_str());
    delete[] _data;
}

bool FSByteHandler::set_data(char **buffer, std::size_t size,
                             std::size_t _data_size, char delim) {
    _delim = delim;
    _ifstream.close();
    std::remove(_name.c_str());

    if(buffer && size) {  // write the rest to temp file
        std::ofstream outs(_name.c_str(), std::ios::binary | std::ios::trunc);
        for(std::size_t i = 0; i < size; ++i) {
            buffer[i][_data_size] = _delim;
            outs.write(buffer[i], _data_size + 1);
        }
        outs.close();  // close ofstream writing

        _ifstream.open(_name.c_str(), std::ios::binary);
        _ifstream.read(_data, _data_size + 1);
        _data[_data_size] = '\0';
    }

    return _ifstream.good();
}

// ----- FStreamByteSort IMPLEMENTATIONS -----

FStreamByteSort::FStreamByteSort(std::size_t size, std::size_t buf_size,
                                 short delim, std::string tname)
    : _delim(delim), _data_size(size), _max_buf(buf_size), _tname(tname) {
    assert(delim == endl || delim == space || delim == tab);
    assert(_max_buf > 0);
    assert(!_tname.empty());
}

FStreamByteSort::~FStreamByteSort() { clear(); }

void FStreamByteSort::clear() { _fs_handlers.clear(); }

void FStreamByteSort::set_max_buffer(std::size_t size) {
    assert(size > 0);
    _max_buf = size;
}

void FStreamByteSort::set_delim(short delim) {
    assert(delim == endl || delim == space || delim == tab);
    _delim = char(delim);
}

void FStreamByteSort::set_temp_name(std::string tname) {
    assert(!tname.empty());
    _tname = tname;
}

std::istream &FStreamByteSort::_extractions(std::istream &ins) {
    char **buffer = nullptr;
    std::size_t i = 0;

    buffer = new char *[_max_buf];  // allocate buffer
    for(std::size_t j = 0; j < _max_buf; ++j)
        buffer[j] = new char[_data_size + 1];

    while(ins) {
        ins.read(buffer[i], _data_size + 1);
        buffer[i++][_data_size] = '\0';

        if(i == _max_buf) {
            _sort_and_dump(buffer, i);
            i = 0;
        }
    }

    // dec i to reverse previous inc; sort and dump last buffer if has data
    if(--i) _sort_and_dump(buffer, i);

    // deallocate
    for(std::size_t j = 0; j < _max_buf; ++j) delete[] buffer[j];
    delete[] buffer;

    return ins;
}

std::ostream &FStreamByteSort::_insertions(std::ostream &outs) {
    while(_fs_handlers.size()) {  // loop until _fs_handlers are gone
        auto min = std::min_element(_fs_handlers.begin(), _fs_handlers.end(),
                                    [](auto &l, auto &r) { return *l < *r; });

        outs << **min;  // deref iterator to deref pointer to min

        if(!**min)  // erase exhausted FSHandler
            _fs_handlers.erase(min);
    }

    return outs;
}

void FStreamByteSort::_sort_and_dump(char **buffer, std::size_t size) {
    // generate unique file name
    std::string name = _tname + std::to_string(_fs_handlers.size());

    std::sort(buffer, buffer + size, [](const char *a, const char *b) {
        return std::strcmp(a, b) < 0;
    });

    // pass buffer to FSByteHandler to create temp file
    _fs_handlers.emplace_back(
        new FSByteHandler(name, buffer, size, _data_size, _delim));
}

}  // namespace fstream_byte_sort

#endif  // FSTREAM_BYTE_SORT_H
