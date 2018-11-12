/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : fstream_handler
 * DESCRIPTION : This header defines a templated file stream handler for
 *      FStreamSort and a non-templated FStreamByteSort. These handler objects
 *      dump a buffer to an ofstream and keeps a link to it via ifstream.
 *      Additionally, retrieval of data from file is done through it's member
 *      _data, which stores one data element at a time.
 *
 *      EX: ofstream << handler;  or  handler >> data;  or  handler >> ofstream;
 *
 *      Every transfer of handler's _data to destination will internally call
 *      ifstream extraction to update member variable _data.
 *
 *      Explitcit boolean conversion of handler reports the internal ifstream's
 *      state. For example, once ifstream is exhausted, 'if(handler)' will
 *      report false.
 ******************************************************************************/
#ifndef FSTREAM_HANDLER_H
#define FSTREAM_HANDLER_H

#include <cstdio>   // remove()
#include <cstring>  // memcmp(), memcpy()
#include <fstream>  // ifstream, ofstream
#include <string>   // string, c_str()

namespace fstream_handler {

enum { BYTE_SIZE = 1024 };

template <typename T>
class FSHandler {
public:
    // CONSTRUCTOR
    FSHandler(std::string name, T *buffer = nullptr, std::size_t size = 0,
              char delim = '\n');

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
    bool set_data(T *buffer = nullptr, std::size_t size = 0, char delim = '\n');

    // FRIENDS
    friend std::ostream &operator<<(std::ostream &outs, FSHandler &h) {
        if(h._file) {
            outs << h._data;
            h._file >> h._data;
        }
        return outs;
    }

    friend bool operator<(const FSHandler &l, const FSHandler &r) {
        return l._data < r._data;
    }

private:
    char _delim;
    std::string _name;
    std::ifstream _file;
    T _data;
};

class FSByteHandler {
public:
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
    friend std::ostream &operator<<(std::ostream &outs, FSByteHandler &h) {
        if(h._file) {
            outs.write(h._data, h._byte_size);
            h._file.read(h._data, h._byte_size);
        }
        return outs;
    }

    friend bool operator<(const FSByteHandler &l, const FSByteHandler &r) {
        return std::memcmp(l._data, r._data, l._byte_size) < 0;
    }

private:
    std::size_t _byte_size;
    char *_data;
    std::string _name;
    std::ifstream _file;
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
 *  T _data            : extracted one data from file
 *  std::ifstream _file: buffer written to file
 *
 * RETURN:
 *  none
 ******************************************************************************/
template <typename T>
FSHandler<T>::FSHandler(std::string name, T *buffer, std::size_t size,
                        char delim)
    : _delim(delim), _name(name), _file(), _data() {
    if(buffer && size) {
        // write data array to file
        std::ofstream outs(_name.c_str(), std::ios::binary | std::ios::trunc);
        for(std::size_t i = 0; i < size; ++i) outs << buffer[i] << _delim;
        outs.close();  // close after writing

        _file.open(_name.c_str(), std::ios::binary);  // open for reading
        _file >> _data;                               // extract first data
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
 *  bool
 ******************************************************************************/
template <typename T>
FSHandler<T>::operator bool() {
    return _file.good();
}

/*******************************************************************************
 * DESCRIPTION:
 *  Extraction operator from FSHandler to ofstream.
 *
 * PRE-CONDITIONS:
 *  std::ifstream _file: file with data
 *
 * POST-CONDITIONS:
 *  std::ifstream _file: file's good if still has data, else bad flag
 *  std::ostream  &outs: extracted data transferred
 *
 * RETURN:
 *  FSHandler<T> &
 ******************************************************************************/
template <typename T>
FSHandler<T> &FSHandler<T>::operator>>(std::ostream &outs) {
    if(_file) {
        outs << _data;   // insert current data
        _file >> _data;  // extract next data
    }
    return *this;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Extraction operator from FSHandler to T data.
 *
 * PRE-CONDITIONS:
 *  std::ifstream _file: file with data
 *
 * POST-CONDITIONS:
 *  std::ifstream _file: file's good if still has data, else bad flag
 *  T             &data: set to extracted data
 *
 * RETURN:
 *  FSHandler<T> &
 ******************************************************************************/
template <typename T>
FSHandler<T> &FSHandler<T>::operator>>(T &data) {
    if(_file) {
        data = _data;    // insert current data
        _file >> _data;  // extract next data
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
    _file.close();
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
 *  T _data            : extra one data from file
 *  std::ifstream _file: buffer written to file
 *
 * RETURN:
 *  bool
 ******************************************************************************/
template <typename T>
bool FSHandler<T>::set_data(T *buffer, std::size_t size, char delim) {
    _delim = delim;
    _file.close();
    std::remove(_name.c_str());

    if(buffer && size) {
        // write data array to file
        std::ofstream outs(_name.c_str(), std::ios::binary | std::ios::trunc);
        for(std::size_t i = 0; i < size; ++i) outs << buffer[i] << _delim;
        outs.close();

        _file.open(_name.c_str(), std::ios::binary);  // open for reading
        _file >> _data;                               // extract first data
    }

    return _file.good();
}

// ----- FSByteHandler IMPLEMENTATIONS -----

/*******************************************************************************
 * DESCRIPTION:
 *  Default constructor. Takes in array of char arrays, write to ofstream,
 *  open file with ifstream, and extract first element to _data.
 *
 * PRE-CONDITIONS:
 *  std::string name     : file name for output creation
 *  char **block         : array of char arrays
 *  std::size_t size     : size of array
 *  std::size_t byte_size: size of char array (w/o NUL terminate)
 *
 * POST-CONDITIONS:
 *  char* _data        : extracted one char array from file
 *  std::ifstream _file: block written to file
 *
 * RETURN:
 *  none
 ******************************************************************************/
FSByteHandler::FSByteHandler(std::string name, char **block, std::size_t size,
                             std::size_t byte_size)
    : _byte_size(byte_size), _data(new char[_byte_size]), _name(name), _file() {
    if(block && size) {
        char *buffer = new char[size * _byte_size];
        std::size_t count = 0;  // buf offset

        std::ofstream outs(_name.c_str(), std::ios::binary | std::ios::trunc);
        for(std::size_t i = 0; i < size; ++i) {
            std::memcpy(buffer + count, block[i], byte_size);
            count += (_byte_size);  // update buf offset
        }
        outs.write(buffer, count);  // write entire buffer
        outs.close();               // close after writing

        _file.open(_name.c_str(), std::ios::binary);  // open reading
        _file.read(_data, _byte_size);                // to read first data

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
 *  bool
 ******************************************************************************/
FSByteHandler::operator bool() { return _file.good(); }

/*******************************************************************************
 * DESCRIPTION:
 *  Extraction operator from FSByteHandler to ofstream.
 *
 * PRE-CONDITIONS:
 *  std::ifstream _file: file with data
 *
 * POST-CONDITIONS:
 *  std::ifstream _file: file's good if still has data, else bad flag
 *  std::ostream  &outs: extracted data transferred
 *
 * RETURN:
 *  FSByteHandler &
 ******************************************************************************/
FSByteHandler &FSByteHandler::operator>>(std::ostream &outs) {
    if(_file) {
        outs.write(_data, _byte_size);
        _file.read(_data, _byte_size);
    }
    return *this;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Extraction operator from FSByteHandler to char* data.
 *
 * PRE-CONDITIONS:
 *  std::ifstream _file: file with data
 *
 * POST-CONDITIONS:
 *  std::ifstream _file: file's good if still has data, else bad flag
 *  char*         &data: memory copied to data
 *
 * RETURN:
 *  FSByteHandler &
 ******************************************************************************/
FSByteHandler &FSByteHandler::operator>>(char *&data) {
    if(_file) {
        std::memcpy(data, _data, _byte_size);
        _file.read(_data, _byte_size);
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
    _file.close();
    std::remove(_name.c_str());
    delete[] _data;
}

}  // namespace fstream_handler

#endif  // FSTREAM_HANDLER_H
