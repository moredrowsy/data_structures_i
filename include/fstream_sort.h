/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : fstream_sort
 * DESCRIPTION : This header defines a templated file stream sort, FStreamSort
 *      and a non-templated FStreamByteSort. The former sorts by
 *      insertion/extraction operators for any data while latter sorts by byte
 *      chunks.
 *
 *      FStreamSort's mechanism
 *      -----------------------
 *      I. Partitioning the file stream: ifstream >> fstream_sort;
 *      It takes in an istream via operator extraction, where data are
 *      read & partitioned and sorted & dump to FSHandler objects.
 *
 *      FSHandlers are used to stored the partitioned data and a list of
 *      handlers is used to keeps track of each partitions. FSHandler also
 *      store the first data of each partition, ie local min/max of partition.
 *
 *      II. Merging the partition files: ofstream << fstream_sort;
 *      FStreamSort find the global min by cycling through all of FSHandler
 *      objects's local min and then write the min value to the output file.
 *
 *      FStreamByteSort's mechanism
 *      ---------------------------
 *      Similar to FStreamSort's algorithm except uses read()/write() to grab
 *      blocks of BYTE for efficiency; thus, it relies on data by fixed byte
 *      blocks, which has to match FStreamByteSort's byte_size either during
 *      instantiation or set by modifiers.
 *
 *      EX: Data are 1024 bytes + '\n' delimeter, then byte_size is 1025.
 *          Data are 1024 bytes without delimter, then byte_size is 1024.
 *
 *      NOTE: byte_size == size of char array (w/o NUL terminate)
 *            max_block ==  size of array of char arrays
 *            buffer size == max_block * byte_size == size of temporary files
 ******************************************************************************/
#ifndef FSTREAM_SORT_H
#define FSTREAM_SORT_H

#include <algorithm>          // sort()
#include <cassert>            // assert()
#include <fstream>            // ifstream, ofstream
#include <memory>             // shared_ptr
#include <string>             // string, c_str()
#include <vector>             // vector
#include "fstream_handler.h"  // FShandler, FSByteHandler

namespace fstream_sort {

enum { BLOCK_SIZE = 10000, BYTE_SIZE = fstream_handler::BYTE_SIZE };
enum : char { endl = '\n', space = ' ', tab = '\t' };

template <typename T>
class FStreamSort {
public:
    // CONSTRUCTOR & DESTRUCTOR
    FStreamSort(std::size_t size = BLOCK_SIZE, char delim = endl,
                std::string tname = "__temp__");
    ~FStreamSort();

    // MUTATORS
    void clear();  // clean up all stream objects and temp files
    void set_max_buffer(std::size_t size);  // change max buffer size
    void set_delim(char delim);             // change delimiter between data
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
    std::size_t _max_buf;  // maximum array size for memory allocation
    std::string _tname;    // temporary file prefix
    std::vector<std::shared_ptr<fstream_handler::FSHandler<T>>> _fs_handlers;

    // MUTATORS
    std::istream &_extractions(std::istream &ins);     // read in-stream
    std::ostream &_insertions(std::ostream &outs);     // write out-stream
    void _sort_and_dump(T *buffer, std::size_t size);  // dump to FSHandler
};

class FStreamByteSort {
public:
    // CONSTRUCTOR & DESTRUCTOR
    FStreamByteSort(std::size_t byte_size = BYTE_SIZE,
                    std::size_t block_size = BLOCK_SIZE,
                    std::string tname = "__temp__");
    ~FStreamByteSort();

    // MUTATORS
    void clear();  // destroy up all handlers
    void set_byte_size(std::size_t size);
    void set_max_block(std::size_t size);
    void set_temp_name(std::string tname);

    // FRIENDS
    friend std::istream &operator>>(std::istream &ins, FStreamByteSort &fs) {
        return fs._extractions(ins);
    }

    friend std::ostream &operator<<(std::ostream &outs, FStreamByteSort &fs) {
        return fs._insertions(outs);
    }

private:
    std::size_t _byte_size;  // size of char array
    std::size_t _max_block;  // maximum size for array of char arrays
    std::string _tname;      // temporary file prefix
    std::vector<std::shared_ptr<fstream_handler::FSByteHandler>> _fs_handlers;

    // MUTATORS
    std::istream &_extractions(std::istream &ins);        // read in-stream
    std::ostream &_insertions(std::ostream &outs);        // write out-stream
    void _sort_and_dump(char **block, std::size_t size);  // dump to handler
};

// ----- FStreamSort IMPLEMENTATIONS -----

/*******************************************************************************
 * DESCRIPTION:
 *  Default constructor.
 *
 * PRE-CONDITIONS:
 *  std::size_t size : array size for buffer > 0
 *  short delim      : specifies valid whitespace delimeter character
 *  std::string tname: valid non-empty temporary file name prefix
 *
 * POST-CONDITIONS:
 *  initializations
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
FStreamSort<T>::FStreamSort(std::size_t size, char delim, std::string tname)
    : _delim(delim), _max_buf(size), _tname(tname) {
    assert(delim == endl || delim == space || delim == tab);
    assert(_max_buf > 0);
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
template <typename T>
FStreamSort<T>::~FStreamSort() {
    clear();
}

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
template <typename T>
void FStreamSort<T>::clear() {
    _fs_handlers.clear();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Set new array size for buffer.
 *
 * PRE-CONDITIONS:
 *  std::size_t size: size > 0
 *
 * POST-CONDITIONS:
 *  std::size_t _max_buf: new size
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void FStreamSort<T>::set_max_buffer(std::size_t size) {
    assert(size > 0);
    _max_buf = size;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Set new character delimiter.
 *
 * PRE-CONDITIONS:
 *  short delim: valid whitespace character in ASCII value
 *
 * POST-CONDITIONS:
 *  char _delim: set to new whitespace char
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void FStreamSort<T>::set_delim(char delim) {
    assert(delim == endl || delim == space || delim == tab);
    _delim = delim;
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
template <typename T>
void FStreamSort<T>::set_temp_name(std::string tname) {
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
template <typename T>
std::istream &FStreamSort<T>::_extractions(std::istream &ins) {
    std::size_t i = 0;
    T *buffer = new T[_max_buf];

    while(ins >> buffer[i++]) {         // inc i after read
        if(i == _max_buf) {             // when buffer is full
            _sort_and_dump(buffer, i);  // sort and dump buffer
            i = 0;
        }
    }

    // dec i to reverse previous inc; sort and dump last buffer if has data
    if(i && --i) _sort_and_dump(buffer, i);

    delete[] buffer;

    return ins;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Merges sorted partitions to outstream.
 *  Internally, each FSHandler holds the local min of sorted partition.
 *  Then find global min of all FSHandlers. FSHandler with the global min
 *  will be inserted to outstream, via it's insertion overload. FSHandler's
 *  insertion overload will simultaneously grab the next local min of the
 *  sorted partition, essentially popping the used global min from its data.
 *
 * PRE-CONDITIONS:
 *  std::ostream &outs: valid outstream
 *
 * POST-CONDITIONS:
 *  std::ostream &outs: data inserted
 *  _fs_handlers     : exhausted FSHandlers erased from array
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
std::ostream &FStreamSort<T>::_insertions(std::ostream &outs) {
    while(_fs_handlers.size()) {  // loop until _fs_handlers are gone
        auto min = std::min_element(_fs_handlers.begin(), _fs_handlers.end(),
                                    [](auto &l, auto &r) { return *l < *r; });

        outs << **min << _delim;  // deref iterator to deref pointer to min

        if(!**min)  // erase exhausted FSHandler
            _fs_handlers.erase(min);
    }

    return outs;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Sorts data array and dumps its content via FSHandler. This FSHandler is
 *  then added to array of FSHandlers to keep record of all files associated
 *  with dumped data.
 *
 * PRE-CONDITIONS:
 *  T *buffer       : data array
 *  std::size_t size: size of array
 *
 * POST-CONDITIONS:
 *  _fs_handlers: added new FSHandler list of handlers
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
void FStreamSort<T>::_sort_and_dump(T *buffer, std::size_t size) {
    using namespace fstream_handler;

    // generate unique file name
    std::string name = _tname + std::to_string(_fs_handlers.size());

    std::sort(buffer, buffer + size);  // sort before passing to FSHandler

    // pass buffer to FSHandler to create temp file
    _fs_handlers.emplace_back(new FSHandler<T>(name, buffer, size, _delim));
}

// ----- FStreamByteSort IMPLEMENTATIONS -----

/*******************************************************************************
 * DESCRIPTION:
 *  Default constructor.
 *
 * PRE-CONDITIONS:
 *  std::size_t byte_size  : size of bytes block, > 0
 *  std::size_t block_size : size of array of char array
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
    assert(_byte_size > 0);
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
 *  Set new size for block (array of char array).
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
    char **block = new char *[_max_block];
    char *buffer = new char[_max_block * _byte_size];
    std::size_t count, size;

    while(ins) {
        ins.read(buffer, _max_block * _byte_size);  // read to one long buffer
        size = ins.gcount() / _byte_size;           // get array size for block
        count = 0;

        for(std::size_t i = 0; i < size; ++i) {  // convert buf to block array
            block[i] = &buffer[count];           // set block[i] to buf offset
            count += _byte_size;                 // get next buf offset
        }
        _sort_and_dump(block, size);  // sort and dump block to handler
    }

    // deallocate
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
    char *buffer = new char[_max_block * _byte_size], *data = nullptr;
    std::size_t count = 0;  // buf offset

    while(_fs_handlers.size()) {  // loop until _fs_handlers are gone
        auto min = std::min_element(_fs_handlers.begin(), _fs_handlers.end(),
                                    [](auto &l, auto &r) { return *l < *r; });

        data = buffer + count;  // offset buffer to data
        **min >> data;          // extract to data
        count += _byte_size;    // increase buffer count

        if(count == _max_block * _byte_size) {  // when buffer is full
            outs.write(buffer, count);          // write entire buffer
            count = 0;                          // reset buffer count
        }

        if(!**min)  // erase exhausted FSHandler
            _fs_handlers.erase(min);
    }
    if(count) outs.write(buffer, count);  // write remaining buffer

    delete[] buffer;  // deallocate

    return outs;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Sorts array of char array and dumps its content via FSByteHandler.
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
    using namespace fstream_handler;

    // generate unique file name
    std::string name = _tname + std::to_string(_fs_handlers.size());

    std::sort(block, block + size, [this](const char *a, const char *b) {
        return std::memcmp(a, b, this->_byte_size) < 0;
    });

    // pass block to FSByteHandler to create temp file
    _fs_handlers.emplace_back(new FSByteHandler(name, block, size, _byte_size));
}

}  // namespace fstream_sort

#endif  // FSTREAM_SORT_H
