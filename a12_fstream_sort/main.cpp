#include <fstream>                     // fstream objects
#include <iomanip>                     // setw()
#include <iostream>                    // stream objects
#include <string>                      // c_str()

#include "../include/fstream_sort.h"   // FStreamSort class
#include "../include/fstream_utils.h"  // file stream utilities
#include "../include/timer.h"          // Timer class

void gen_rand_int_file(std::string fname, int sample_osize = 250000);
void gen_records(std::string fname, int sample_osize = 250000,
                 std::size_t byte_size = 1024, bool add_delim = true);
void test_single_fstream_sort(int sample_size = 2500000,
                              int buffer_size = 250000);
void test_single_fstream_byte_sort(int sample_size = 2500000,
                                   int buffer_size = 250000,
                                   std::size_t byte_size = 1024);
void test_multiple_fstream_sort(int sample_size);
double* test_fstream_times(std::size_t data_count = 2500000,
                           std::size_t buffer_size = 250000,
                           int sample_size = 10);

int main() {
    std::size_t sample_size = 1000000;
    std::size_t byte_size = 1024;
    gen_records("records.txt", sample_size, 1024, true);

    std::cout << "FileStream Sort via insertion/extraction op" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    test_single_fstream_sort(sample_size, 64000);

    std::cout << std::endl;

    std::cout << "FileStream Sorting by BYTE" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    test_single_fstream_byte_sort(sample_size, 64000, byte_size + 1);

    // test_multiple_fstream_sort(10);

    return 0;
}

void gen_rand_int_file(std::string fname, int sample_osize) {
    std::ofstream fout(fname.c_str(), std::ios::binary);

    srand(time(nullptr));
    for(int i = 0; i < sample_osize; ++i) fout << rand() << '\n';
}

void gen_records(std::string fname, int sample_osize, std::size_t byte_size,
                 bool add_delim) {
    char* data = add_delim ? new char[byte_size + 1] : new char[byte_size];
    std::ofstream fout(fname.c_str(), std::ios::binary);

    for(int i = 0; i < sample_osize; ++i) {
        for(std::size_t j = 0; j < byte_size; ++j) data[j] = '0' + rand() % 10;
        if(add_delim) data[byte_size] = '\n';
        fout.write(data, add_delim ? byte_size + 1 : byte_size);
    }

    delete[] data;
}

void test_multiple_fstream_sort(int sample_size) {
    const int B_SIZE = 4, C_SIZE = 3;
    std::size_t buffer_sizes[B_SIZE + 1] = {100000, 250000, 500000, 1000000};
    std::size_t data_counts[C_SIZE] = {1, 10, 100};  // data counts for each run
    double* data[B_SIZE][C_SIZE];

    // increase counts to corresponding MB
    for(int i = 0; i < C_SIZE; ++i) data_counts[i] *= 100000;

    // loop buffer sizes
    for(int i = 0; i < B_SIZE + 1; ++i) {
        // loop data counts
        for(int j = 0; j < C_SIZE; ++j) {
            data[i][j] = test_fstream_times(data_counts[j], buffer_sizes[i],
                                            sample_size);
        }
    }

    // delete all data
    for(int i = 0; i < B_SIZE + 1; ++i) {
        // loop data counts
        for(int j = 0; j < C_SIZE; ++j) {
            delete[] data[i][j];
        }
    }
}

double* test_fstream_times(std::size_t data_count, std::size_t buffer_size,
                           int sample_size) {
    using namespace fstream_sort;
    using namespace fstream_utils;

    const int byte_size = 7;
    bool is_sorted = false;
    int in_count = 0, out_count = 0;
    double in_time = 0, out_time = 0, gen_time = 0, gen_file_size = 0;
    std::string in_file = "in_file.txt", out_file = "out_file.txt";
    timer::ChronoTimer chrono;

    for(int i = 0; i < sample_size; ++i) {
        chrono.start();
        gen_rand_int_file(in_file, data_count);
        chrono.stop();
        gen_time += chrono.seconds();

        gen_file_size += filesize(in_file.c_str()) / 1000000.0;

        std::ifstream fin(in_file.c_str(), std::ios::binary);
        std::ofstream fout(out_file.c_str(), std::ios::binary);
        FStreamSort<int> fsort(buffer_size);

        chrono.start();
        fin >> fsort;
        chrono.stop();
        in_time += chrono.seconds();
        fin.close();

        chrono.start();
        fout << fsort;
        chrono.stop();
        out_time += chrono.seconds();
        fout.close();

        in_count += count_file<int>(in_file);
        out_count += count_file<int>(out_file);
        is_sorted = is_sorted && verify_sorted_file<int>(out_file);

        std::remove(in_file.c_str());
        std::remove(out_file.c_str());
    }

    gen_time /= (double)sample_size;
    gen_file_size /= (double)sample_size;
    in_time /= (double)sample_size;
    out_time /= (double)sample_size;
    in_count /= (double)sample_size;
    out_count /= (double)sample_size;

    return new double[byte_size]{
        gen_file_size,    gen_time,          in_time,          out_time,
        (double)in_count, (double)out_count, (double)is_sorted};
}

void test_single_fstream_sort(int sample_size, int buffer_size) {
    using namespace fstream_sort;
    using namespace fstream_utils;

    double in_time = 0, out_time = 0, gen_time = 0, in_file_size = 0;
    std::string in_file = "records.txt", out_file = "fs_results.txt";
    timer::ChronoTimer chrono;

    chrono.start();
    gen_records(in_file, sample_size);
    chrono.stop();
    gen_time = chrono.seconds();

    in_file_size = filesize(in_file.c_str());

    std::ifstream fin(in_file.c_str(), std::ios::binary);
    std::ofstream fout(out_file.c_str(), std::ios::binary);
    FStreamSort<std::string> fsort(buffer_size);

    chrono.start();
    fin >> fsort;
    chrono.stop();
    in_time = chrono.seconds();
    fin.close();

    chrono.start();
    fout << fsort;
    chrono.stop();
    out_time = chrono.seconds();
    fout.close();

    int width = 25;
    std::cout << std::setw(width)
              << "Random file size : " << in_file_size / 1000000.0 << " MB"
              << std::endl
              << std::setw(width) << "Random generation time : " << gen_time
              << " sec" << std::endl
              << std::setw(width) << "Extraction time : " << in_time << " sec"
              << std::endl
              << std::setw(width) << "Insertion time : " << out_time << " sec"
              << std::endl
              << std::setw(width)
              << "Count for infile : " << count_file<std::string>(in_file)
              << std::endl
              << std::setw(width)
              << "Count for outfile : " << count_file<std::string>(out_file)
              << std::endl
              << std::setw(width) << "Output file is sorted : ";
    if(verify_sorted_file<std::string>(out_file))
        std::cout << "true" << std::endl;
    else
        std::cout << "false" << std::endl;
}

void test_single_fstream_byte_sort(int sample_size, int buffer_size,
                                   std::size_t byte_size) {
    using namespace fstream_utils;
    using namespace fstream_sort;

    double in_time = 0, out_time = 0, gen_time = 0, in_file_size = 0,
           out_file_size = 0;
    std::string in_file = "records.txt", out_file = "fsb_result.txt";
    timer::ChronoTimer chrono;

    chrono.start();
    gen_records(in_file, sample_size, byte_size, false);
    chrono.stop();
    gen_time = chrono.seconds();

    in_file_size = filesize(in_file.c_str());

    std::ifstream fin(in_file.c_str(), std::ios::binary);
    std::ofstream fout(out_file.c_str(), std::ios::binary);
    FStreamByteSort fsb_sort(byte_size, buffer_size);

    chrono.start();
    fin >> fsb_sort;
    chrono.stop();
    in_time = chrono.seconds();
    fin.close();

    chrono.start();
    fout << fsb_sort;
    chrono.stop();
    out_time = chrono.seconds();
    fout.close();

    out_file_size = filesize(out_file.c_str());

    int width = 25;
    std::cout.setf(std::ios::fixed);
    std::cout.precision(0);
    std::cout << std::setw(width)
              << "Random file size : " << in_file_size / 1000000.0 << " MB"
              << std::endl
              << std::setw(width) << "Random generation time : " << gen_time
              << " sec" << std::endl
              << std::setw(width) << "Extraction time : " << in_time << " sec"
              << std::endl
              << std::setw(width) << "Insertion time : " << out_time << " sec"
              << std::endl
              << std::setw(width)
              << "Count for infile : " << in_file_size / (float)byte_size
              << std::endl
              << std::setw(width)
              << "Count for outfile : " << out_file_size / (float)byte_size
              << std::endl
              << std::setw(width) << "Output file is sorted : ";
    if(verify_sorted_byte_file(out_file, byte_size))
        std::cout << "true" << std::endl;
    else
        std::cout << "false" << std::endl;
}
