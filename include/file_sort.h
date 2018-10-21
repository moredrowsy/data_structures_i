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
    FileInfo(std::string name, int fpos = 0)
        : _name(name), _if(_name), _fpos(fpos) {
        assert(_if.is_open());
    }

    // FRIENDS
    friend std::ostream &operator<<(std::ostream &outs, const FileInfo &f) {
        return outs << f._name << ":" << f._fpos;
    }

    std::string _name;
    std::ifstream _if;  // must initialize with existing file or fail!
    int _fpos;
};

template <typename T>
class FileSort {
public:
    enum { MAX_BLOCK = 250000 };

    // CONSTRUCTOR & DESTRUCTOR
    FileSort(std::string in = "in_file.txt", std::string out = "out_file.txt",
             std::size_t s = MAX_BLOCK);
    ~FileSort();

    // ACCESSORS
    std::string in_filename() const;   // return current in-file name
    std::string out_filename() const;  // return current out-file name

    // MUTATORS
    int count_infile();      // total data elements in in-file
    int count_outfile();     // total data elements in out-file
    bool validate_sorted();  // validate outfile is sorted
    void cleanup();          // clean up all stream objects and temp files
    void remove_in_file();   // remove out-file
    void remove_out_file();  // remove out-file
    bool set_ifname(std::string ifname);    // change in-file name
    void set_ofname(std::string ofname);    // change out-file name
    void set_block_size(std::size_t size);  // change max block size
    void sort();                            // process input file and sort

private:
    std::string _ifname;                // in-file stream name
    std::string _ofname;                // out-file stream name
    std::ifstream _if;                  // in-stream object
    std::ofstream _of;                  // out-stream object
    std::size_t _tsize;                 // number of temporary file outputs
    std::size_t _max_block;             // maximum size for memory allocation
    std::vector<FileInfo> _file_infos;  // holds file name, pos and ifstream

    // MUTATORS
    void _parse_infile();     // start in-file via _if
    void _parse_tempfiles();  // sort temporary files and output via _of
    void _output_block(int *block, std::size_t size);  // create temp files
};

template <typename T>
FileSort<T>::FileSort(std::string in, std::string out, std::size_t s)
    : _ifname(in), _ofname(out), _if(_ifname), _tsize(0), _max_block(s) {
    assert(!_ifname.empty());
    assert(!_ofname.empty());
    assert(_max_block > 0);
}

template <typename T>
FileSort<T>::~FileSort() {
    for(auto &a : _file_infos) std::remove(a._name.c_str());  // delete all temp
}

template <typename T>
std::string FileSort<T>::in_filename() const {
    return _ifname;
}

template <typename T>
std::string FileSort<T>::out_filename() const {
    return _ofname;
}

template <typename T>
int FileSort<T>::count_infile() {
    _if.close();
    std::ifstream read(_ifname);

    int count = 0;
    T temp;
    while(read >> temp) ++count;

    return count;
}

template <typename T>
int FileSort<T>::count_outfile() {
    _of.close();
    std::ifstream read(_ofname);

    int count = 0;
    T temp;
    while(read >> temp) ++count;

    return count;
}

template <typename T>
bool FileSort<T>::validate_sorted() {
    _of.close();

    std::ifstream fin(_ofname.c_str());
    bool is_sorted = fin.is_open();
    T prev, current;

    fin >> prev;
    while(fin >> current) {
        if(prev > current) {
            is_sorted = false;
            break;
        }

        prev = current;
    }

    return is_sorted;
}

template <typename T>
void FileSort<T>::cleanup() {
    for(auto &a : _file_infos) std::remove(a._name.c_str());  // delete all temp

    _file_infos.clear();
    _tsize = 0;
    _if.close();
    _of.close();
}

template <typename T>
void FileSort<T>::remove_in_file() {
    _if.close();
    std::remove(_ifname.c_str());
}

template <typename T>
void FileSort<T>::remove_out_file() {
    _of.close();
    std::remove(_ofname.c_str());
}

template <typename T>
bool FileSort<T>::set_ifname(std::string ifname) {
    _ifname = ifname;
    _if.close();
    _if.open(_ifname);

    return _if.is_open();
}

template <typename T>
void FileSort<T>::set_ofname(std::string ofname) {
    _ofname = ofname;
}

template <typename T>
void FileSort<T>::set_block_size(std::size_t size) {
    assert(size > 0);
    _max_block = size;
}

template <typename T>
void FileSort<T>::sort() {
    if(!_if.is_open()) _if.open(_ifname.c_str());  // reopen file if not open!
    assert(_if.is_open());
    _parse_infile();
    _parse_tempfiles();
}

template <typename T>
void FileSort<T>::_parse_infile() {
    int i = 0;
    T *block = nullptr;

    block = new T[_max_block];

    while(_if >> block[i]) {
        ++i;

        if(i == (int)_max_block) {  // file output if block is full
            _output_block(block, i);
            i = 0;
        }
    }
    if(i) _output_block(block, i);  // output last block if has data

    delete[] block;
}

template <typename T>
void FileSort<T>::_parse_tempfiles() {
    T current, min;
    int min_index = -1, fpos = -1;

    if(_file_infos.size()) _of.open(_ofname);

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
        _of << min << std::endl;
        _file_infos[min_index]._fpos = fpos;

        // remove block file if file read fails via fpos -1
        if(fpos < 0) {
            std::remove(_file_infos[min_index]._name.c_str());
            _file_infos.erase(_file_infos.begin() + min_index);
            --_tsize;
        }
    }

    _of.close();
}

template <typename T>
void FileSort<T>::_output_block(int *block, std::size_t size) {
    // generate unique block file name
    std::string name = _ifname + "." + std::to_string(_tsize++);

    std::sort(block, block + size);  // sort block

    std::ofstream fout(name, std::ofstream::out | std::ofstream::trunc);
    for(std::size_t i = 0; i < size; ++i) {
        fout << block[i];

        if(i != size - 1) fout << std::endl;
    }
    fout.close();

    _file_infos.push_back(name);  // must push back name after file creation!!!
}

}  // namespace file_sort

#endif  // FILE_SORT_H
