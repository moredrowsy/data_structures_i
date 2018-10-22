#include <fstream>                 // fstream objects
#include <iomanip>                 // setw()
#include <iostream>                // stream objects
#include <string>                  // c_str()
#include "../include/file_sort.h"  // FileSort class
#include "../include/timer.h"      // Timer class

void gen_rand_int_file(std::string fname, int sample_osize = 250000);
void test_file_sort();

int main() {
    test_file_sort();

    return 0;
}

void gen_rand_int_file(std::string fname, int sample_osize) {
    std::ofstream fout(fname.c_str());

    srand(time(nullptr));
    for(int i = 0; i < sample_osize; ++i) fout << rand() << std::endl;
}

void test_file_sort() {
    using namespace file_sort;

    int random_size = 2500000;
    int buffer_size = 250000;
    int in_time = 0, out_time = 0, gen_time = 0, gen_file_size = 0;
    std::string in_file = "random_ints.txt", out_file = "results.txt";
    timer::Timer timer;

    timer.start();
    gen_rand_int_file(in_file, random_size);
    timer.stop();
    gen_time = timer.duration();

    gen_file_size = filesize(in_file.c_str());

    std::ifstream fin(in_file.c_str(), std::ios::binary);
    std::ofstream fout(out_file.c_str(), std::ios::binary);
    FileSort<int> fsort(buffer_size);

    timer.start();
    fin >> fsort;
    timer.stop();
    in_time = timer.duration();
    fin.close();

    timer.start();
    fout << fsort;
    timer.stop();
    out_time = timer.duration();
    fout.close();

    int width = 25;
    std::cout << std::setw(width) << "Random generation time : " << gen_time
              << std::endl
              << std::setw(width) << "Random file size : " << gen_file_size
              << std::endl
              << std::setw(width) << "Extraction time : " << in_time
              << std::endl
              << std::setw(width) << "Insertion time : " << out_time
              << std::endl
              << std::setw(width)
              << "Count for infile : " << count_file<int>(in_file) << std::endl
              << std::setw(width)
              << "Count for outfile : " << count_file<int>(out_file)
              << std::endl
              << std::setw(width) << "Output file is sorted : "
              << validate_sorted_file<int>(out_file) << std::endl;
}
