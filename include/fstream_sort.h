/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : fstream_sort
 * DESCRIPTION : This header defines a templated file stream sorting.
 *      FStreamSort first takes in an istream object via operator extraction,
 *      where data are read and then dump their contents to FSHandler objects.
 *
 *      FSHandler takes a buffer of data and write to temporary files. It's
 *      attribute, _data, holds the first data element of the buffer.
 *      Internally, it creates an ifstream object linked to the temp file and
 *      continuously reads its content to _data.
 *
 *      FStreamSort's finally writes the output data via operator insertion.
 *      Internally, it cycles through  all of FSHandler objects, find the
 *      mininum value and then write the min value to the output file.
 ******************************************************************************/
#ifndef FSTREAM_SORT_H
#define FSTREAM_SORT_H

#include <algorithm>  // sort()
#include <cassert>    // assert()
#include <cstdio>     // remove()
#include <fstream>    // ifstream, ofstream
#include <iostream>   // stream objects
#include <memory>     // shared_ptr
#include <string>     // string, c_str()
#include <vector>     // vector

namespace fstream_sort {

enum { MAX_BUFFER = 250000, endl = 10, space = 32, tab = 9 };

template <typename T>
struct FSHandler {
    // CONSTRUCTOR
    FSHandler(std::string name, T *buffer = nullptr, std::size_t size = 0,
              char delim = char(endl));

    // MOVE SEMANTICS for std::ifstream
    FSHandler(FSHandler &&) = default;
    FSHandler &operator=(FSHandler &&) = default;
    ~FSHandler();

    // OPERATORS
    explicit operator bool();
    FSHandler<T> &operator>>(std::ostream &outs);
    FSHandler<T> &operator>>(T &data);

    // MUTATORS
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
    FStreamSort(std::size_t size = MAX_BUFFER, short delim = endl,
                std::string tname = "__temp__");
    ~FStreamSort();

    // MUTATORS
    void clear();  // clean up all stream objects and temp files
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
    std::size_t _max_buf;  // maximum array size for memory allocation
    std::string _tname;    // temporary file prefix
    std::vector<std::shared_ptr<FSHandler<T>>> _fs_handlers;  // make temp files

    // MUTATORS
    std::istream &_extractions(std::istream &ins);     // read in-stream
    std::ostream &_insertions(std::ostream &outs);     // write out-stream
    void _sort_and_dump(T *buffer, std::size_t size);  // dump to FSHandler
};

// ----- FSHandler IMPLEMENTATIONS -----

/*******************************************************************************
 * DESCRIPTION:
 *  Default constructor. Takes in data array, write to ofstream, open file with
 *  ifstream, and extract first element to _data.
 *
 * PRE-CONDITIONS:
 *  std::string name: file name to handle, must be unique
 *  T *buffer       : array of data
 *  std::size_t size: array size
 *  char delim      : delimter for data
 *
 * POST-CONDITIONS:
 *  T _data                : holds first from buffer
 *  std::ifstream _ifstream: rest of data written to file associated w/ ifstream
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
FSHandler<T>::FSHandler(std::string name, T *buffer, std::size_t size,
                        char delim)
    : _delim(delim), _name(name), _ifstream(), _data() {
    if(buffer && size) {
        // write data array to file
        std::ofstream outs(_name.c_str(), std::ios::binary | std::ios::trunc);
        for(std::size_t i = 0; i < size; ++i) outs << buffer[i] << _delim;
        outs.close();  // close ofstream writing

        _ifstream.open(_name.c_str(), std::ios::binary);  // open for reading
        _ifstream >> _data;  // extract first data element
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
template <typename T>
FSHandler<T>::~FSHandler() {
    clear();
}

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
template <typename T>
FSHandler<T>::operator bool() {
    return _ifstream.good();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Extraction operator from FSHandler to ofstream.
 *
 * PRE-CONDITIONS:
 *  std::ifstream _ifstream: good state
 *
 * POST-CONDITIONS:
 *  std::ifstream _ifstream: good state/bad state
 *  std::ostream      &outs: extracted data transferred
 *
 * RETURN:
 *  FSHandler<T> &
 ******************************************************************************/
template <typename T>
FSHandler<T> &FSHandler<T>::operator>>(std::ostream &outs) {
    if(_ifstream) {          // when ifstream is good
        outs << _data;       // insert current data
        _ifstream >> _data;  // extract next data
    }
    return *this;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Extraction operator from FSHandler to T data.
 *
 * PRE-CONDITIONS:
 *  std::ifstream _ifstream: good state
 *
 * POST-CONDITIONS:
 *  std::ifstream _ifstream: good state/bad state
 *  T                 &data: set to extracted data
 *
 * RETURN:
 *  FSHandler<T> &
 ******************************************************************************/
template <typename T>
FSHandler<T> &FSHandler<T>::operator>>(T &data) {
    if(_ifstream) {          // when ifstream is good
        data = _data;        // insert current data
        _ifstream >> _data;  // extract next data
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
template <typename T>
void FSHandler<T>::clear() {
    _ifstream.close();
    std::remove(_name.c_str());
}

/*******************************************************************************
 * DESCRIPTION:
 *  Takes in data array, write to ofstream, open file with ifstream, and
 *  extract first element to _data.
 *
 * PRE-CONDITIONS:
 *  T *buffer       : array of data
 *  std::size_t size: array size
 *  char delim      : delimter for data
 *
 * POST-CONDITIONS:
 *  T _data                : holds first from buffer
 *  std::ifstream _ifstream: rest of data written to file associated w/ ifstream
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool FSHandler<T>::set_data(T *buffer, std::size_t size, char delim) {
    _delim = delim;
    _ifstream.close();
    std::remove(_name.c_str());

    if(buffer && size) {
        // write data array to file
        std::ofstream outs(_name.c_str(), std::ios::binary | std::ios::trunc);
        for(std::size_t i = 0; i < size; ++i) outs << buffer[i] << _delim;
        outs.close();

        _ifstream.open(_name.c_str(), std::ios::binary);  // open for reading
        _ifstream >> _data;  // extract first data element
    }

    return _ifstream.good();
}

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
FStreamSort<T>::FStreamSort(std::size_t size, short delim, std::string tname)
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
void FStreamSort<T>::set_delim(short delim) {
    assert(delim == endl || delim == space || delim == tab);
    _delim = char(delim);
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
    // generate unique file name
    std::string name = _tname + std::to_string(_fs_handlers.size());

    std::sort(buffer, buffer + size);  // sort before passing to FSHandler

    // pass buffer to FSHandler to create temp file
    _fs_handlers.emplace_back(new FSHandler<T>(name, buffer, size, _delim));
}

}  // namespace fstream_sort

#endif  // FSTREAM_SORT_H
