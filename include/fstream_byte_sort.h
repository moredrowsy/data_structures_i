/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : fstream_byte_sort
 * DESCRIPTION : This header defines a file stream BYTE sorting or block of
 *      BYTE sorting. Because this sorting relies on a fixed block of bytes,
 *      the element of each data's size must match the FStreamByte's byte_size,
 *      either during instantiation or set by modifiers.
 *
 *      EX: Data are 1024 bytes + '\n' delimeter, then byte_size is 1025.
 *          Data are 1024 bytes without delimter, then byte_size is 1024.
 *
 *      NOTE: byte_size == size of char strings (w/o '\0')
 *            max_block ==  size of array of char strings
 *            buffer size == max_block * byte_size == size of temporary files
 ******************************************************************************/
#ifndef FSTREAM_BYTE_SORT_H
#define FSTREAM_BYTE_SORT_H

#include <algorithm>  // sort(), copy()
#include <cassert>    // assert()
#include <cstdio>     // remove()
#include <cstring>    // strcmp()
#include <fstream>    // ifstream, ofstream
#include <iostream>   // stream objects
#include <memory>     // shared_ptr
#include <string>     // string, c_str()
#include <vector>     // vector

namespace fstream_byte_sort {

enum { BYTE_SIZE = 1024, BLOCK_SIZE = 10000, endl = 10, space = 32, tab = 9 };

struct FSByteHandler {
    // CONSTRUCTORS
    FSByteHandler(std::string name, char **block = nullptr,
                  std::size_t size = 0, std::size_t cstr_size = BYTE_SIZE + 1);

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
            outs.write(f._data, f._cstr_size - 1);  // size-1 exclude '\0'

            f._ifstream.read(f._data, f._cstr_size - 1);
            f._data[f._cstr_size - 1] = '\0';  // add '\0' for cstring
        }
        return outs;
    }

    friend bool operator<(const FSByteHandler &left,
                          const FSByteHandler &right) {
        return std::strcmp(left._data, right._data) < 0;
    }

    std::size_t _cstr_size;
    char *_data;
    std::string _name;
    std::ifstream _ifstream;
};

class FStreamByteSort {
public:
    // CONSTRUCTOR & DESTRUCTOR
    FStreamByteSort(std::size_t byte_size = BYTE_SIZE,
                    std::size_t block_size = BLOCK_SIZE,
                    std::string tname = "__temp__");
    ~FStreamByteSort();

    // MUTATORS
    void clear();  // clean up all stream objects and temp files
    void set_byte_size(std::size_t size);   // block size of bytes
    void set_max_block(std::size_t size);   // change max block size
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
    std::size_t _max_block;  // maximum size for array of char strings
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

/*******************************************************************************
 * DESCRIPTION:
 *  Default constructor. Takes in data array, write to ofstream, open file with
 *  ifstream, and extract first element to _data.
 *
 * PRE-CONDITIONS:
 *  std::string name     : file name for output creation
 *  char **block         : array of char strings
 *  std::size_t size     : size of array
 *  std::size_t cstr_size: size of char strings (include '\0')
 *
 * POST-CONDITIONS:
 *  char* _data            : holds the first char string
 *  std::ifstream _ifstream: rest of data written to file associated w/ ifstream
 *
 * RETURN:
 *  none
 ******************************************************************************/
FSByteHandler::FSByteHandler(std::string name, char **block, std::size_t size,
                             std::size_t cstr_size)
    : _cstr_size(cstr_size),
      _data(new char[_cstr_size]),
      _name(name),
      _ifstream() {
    if(block && size) {  // write the rest to temp file
        char *buffer = new char[size * _cstr_size];
        std::size_t count = 0;

        std::ofstream outs(_name.c_str(), std::ios::binary | std::ios::trunc);
        for(std::size_t i = 0; i < size; ++i) {
            std::copy(block[i], block[i] + _cstr_size - 1, buffer + count);
            count += (_cstr_size - 1);  // size-1 exclude '\0'
        }
        outs.write(buffer, count);  // write entire buffer
        outs.close();               // close ofstream writing

        _ifstream.open(_name.c_str(), std::ios::binary);
        _ifstream.read(_data, _cstr_size - 1);
        _data[_cstr_size - 1] = '\0';  // add '\0' for cstring

        delete[] buffer;
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Destructor calls clear() to close ifstream and remove associated file.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  file is removed from operating system
 *
 * RETURN:
 *  none
 ******************************************************************************/
FSByteHandler::~FSByteHandler() { clear(); }

/*******************************************************************************
 * DESCRIPTION:
 *  Explicit bool conversion. Reports ifstream's state.
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
FSByteHandler::operator bool() { return _ifstream.good(); }

/*******************************************************************************
 * DESCRIPTION:
 *  Extraction operator from FSByteHandler to ofstream.
 *
 * PRE-CONDITIONS:
 *  std::ifstream _ifstream: good state
 *
 * POST-CONDITIONS:
 *  std::ifstream _ifstream: good state/bad state
 *  std::ostream      &outs: extracted data transferred
 *
 * RETURN:
 *  FSByteHandler &
 ******************************************************************************/
FSByteHandler &FSByteHandler::operator>>(std::ostream &outs) {
    if(_ifstream) {                         // when ifstream is good
        outs.write(_data, _cstr_size - 1);  // size-1 exclude '\0'

        _ifstream.read(_data, _cstr_size - 1);
        _data[_cstr_size - 1] = '\0';  // add '\0' for cstring
    }
    return *this;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Extraction operator from FSByteHandler to char* data.
 *
 * PRE-CONDITIONS:
 *  std::ifstream _ifstream: good state
 *
 * POST-CONDITIONS:
 *  std::ifstream _ifstream: good state/bad state
 *  char*             &data: memory copied to data
 *
 * RETURN:
 *  FSByteHandler &
 ******************************************************************************/
FSByteHandler &FSByteHandler::operator>>(char *&data) {
    if(_ifstream) {  // when ifstream is good
        std::copy(_data, _data + _cstr_size - 1, data);  // size-1 exclude '\0'

        _ifstream.read(_data, _cstr_size - 1);
        _data[_cstr_size - 1] = '\0';  // add '\0' for cstring
    }
    return *this;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Close ifstream and remove associated file.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  file is removed from operating system
 *
 * RETURN:
 *  none
 ******************************************************************************/
void FSByteHandler::clear() {
    _ifstream.close();
    std::remove(_name.c_str());
    delete[] _data;
}

// ----- FStreamByteSort IMPLEMENTATIONS -----

/*******************************************************************************
 * DESCRIPTION:
 *  Default constructor.
 *
 * PRE-CONDITIONS:
 *  std::size_t byte_size  : size of bytes block, > 0
 *  std::size_t block_size : size of array of char strings
 *  std::string tname      : valid non-empty temporary file name prefix
 *
 * POST-CONDITIONS:
 *  initializations
 *
 * RETURN:
 *  none
 ******************************************************************************/
FStreamByteSort::FStreamByteSort(std::size_t byte_size, std::size_t block_size,
                                 std::string tname)
    : _byte_size(byte_size), _max_block(block_size), _tname(tname) {
    assert(byte_size > 0);
    assert(_max_block > 0);
    assert(!_tname.empty());
}

/*******************************************************************************
 * DESCRIPTION:
 *  Destructor calls clear() to clear file handlers in the event that file
 *  handlers did not parse all files.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  _fs_handlers: cleared
 *
 * RETURN:
 *  none
 ******************************************************************************/
FStreamByteSort::~FStreamByteSort() { clear(); }

/*******************************************************************************
 * DESCRIPTION:
 *  Empties file handlers
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  _fs_handlers: cleared
 *
 * RETURN:
 *  none
 ******************************************************************************/
void FStreamByteSort::clear() { _fs_handlers.clear(); }

/*******************************************************************************
 * DESCRIPTION:
 *  Set size of a block of BYTES.
 *
 * PRE-CONDITIONS:
 *  std::size_t size: size > 0
 *
 * POST-CONDITIONS:
 *  std::size_t _byte_size: new size
 *
 * RETURN:
 *  none
 ******************************************************************************/
void FStreamByteSort::set_byte_size(std::size_t size) {
    assert(size > 0);
    assert(_fs_handlers.empty());
    _byte_size = size;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Set new size for block (array of char strings).
 *
 * PRE-CONDITIONS:
 *  std::size_t size: size > 0
 *
 * POST-CONDITIONS:
 *  std::size_t _max_block: new size
 *
 * RETURN:
 *  none
 ******************************************************************************/
void FStreamByteSort::set_max_block(std::size_t size) {
    assert(size > 0);
    assert(_fs_handlers.empty());
    _max_block = size;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Set new temporary file name prefix.
 *
 * PRE-CONDITIONS:
 *  std::string tname: prefix for temporary file name
 *
 * POST-CONDITIONS:
 *  std::string tname: set to new prefix
 *
 * RETURN:
 *  none
 ******************************************************************************/
void FStreamByteSort::set_temp_name(std::string tname) {
    assert(!tname.empty());
    _tname = tname;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Splits the ifstream data into sorted partitions and dump it to FSHandlers.
 *  Internally, it extracts each data into an array (buffer), sorts it
 *  and dump buffer to temporary files via FSHandler.
 *
 * PRE-CONDITIONS:
 *  std::istream &ins: valid ifstream
 *
 * POST-CONDITIONS:
 *  std::istream &ins: exhausted ifstream
 *  _fs_handlers     : handlers hold sorted partitions via associated outfiles
 *
 * RETURN:
 *  none
 ******************************************************************************/
std::istream &FStreamByteSort::_extractions(std::istream &ins) {
    char **block = nullptr, *buffer = new char[_max_block * _byte_size];
    std::size_t count, size;

    block = new char *[_max_block];  // allocate block array
    for(std::size_t i = 0; i < _max_block; ++i)
        block[i] = new char[_byte_size + 1];  // allocate each char string

    while(ins) {
        ins.read(buffer, _max_block * _byte_size);  // read to one long buffer
        size = ins.gcount() / _byte_size;           // get array size for block
        count = 0;

        for(std::size_t i = 0; i < size; ++i) {  // convert buf to block array
            std::copy(buffer + count, buffer + count + _byte_size, block[i]);
            block[i][_byte_size] = '\0';  // add '0' for char string
            count += _byte_size;
        }
        _sort_and_dump(block, size);
    }

    // deallocate
    for(std::size_t j = 0; j < _max_block; ++j) delete[] block[j];
    delete[] block;
    delete[] buffer;

    return ins;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Merges sorted partitions to outstream.
 *  Internally, each FSHandler holds the local min of sorted partition.
 *  Then find global min of all FSHandlers. FSHandler with the global min
 *  will be inserted to buffer. FSHandler's insertion overload will
 *  simultaneously grab the next local min of the sorted partition, essentially
 *  popping the used global min from its data.
 *
 * PRE-CONDITIONS:
 *  std::ostream &outs: valid outstream
 *
 * POST-CONDITIONS:
 *  std::ostream &outs: buffer written to ostream
 *  _fs_handlers     : exhausted FSHandlers erased from array
 *
 * RETURN:
 *  none
 ******************************************************************************/
std::ostream &FStreamByteSort::_insertions(std::ostream &outs) {
    char *buffer = new char[_max_block * _byte_size],
         *data = new char[_byte_size];
    std::size_t count = 0;

    while(_fs_handlers.size()) {  // loop until _fs_handlers are gone
        auto min = std::min_element(_fs_handlers.begin(), _fs_handlers.end(),
                                    [](auto &l, auto &r) { return *l < *r; });

        **min >> data;                                       // extract to data
        std::copy(data, data + _byte_size, buffer + count);  // copy to buf
        count += _byte_size;  // increase buffer count

        if(count == _max_block * _byte_size) {  // when buffer is full
            outs.write(buffer, count);          // write entire buffer
            count = 0;                          // reset buffer count
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

/*******************************************************************************
 * DESCRIPTION:
 *  Sorts array of char strings and dumps its content via FSByteHandler.
 *  This FSByteHandler is then added to array of FSHandlers to keep record of
 *  all files associated with dumped data.
 *
 * PRE-CONDITIONS:
 *  char **block    : data array
 *  std::size_t size: size of array
 *
 * POST-CONDITIONS:
 *  _fs_handlers: added new FSByteHandler to list of handlers
 *
 * RETURN:
 *  none
 ******************************************************************************/
void FStreamByteSort::_sort_and_dump(char **block, std::size_t size) {
    // generate unique file name
    std::string name = _tname + std::to_string(_fs_handlers.size());

    std::sort(block, block + size, [](const char *a, const char *b) {
        return std::strcmp(a, b) < 0;
    });

    // pass block to FSByteHandler to create temp file
    _fs_handlers.emplace_back(  // _byte_size + 1 includes '\0' for cstring
        new FSByteHandler(name, block, size, _byte_size + 1));
}

}  // namespace fstream_byte_sort

#endif  // FSTREAM_BYTE_SORT_H
