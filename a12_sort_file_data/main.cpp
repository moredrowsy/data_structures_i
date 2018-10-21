#include <fstream>                 // fstream objects
#include <iostream>                // stream objects
#include <string>                  // c_str()
#include "../include/file_sort.h"  // FileSort class

void gen_rand_int_file(std::string fname, int sample_osize = 250000);
void test_file_sort();

int main() {
    test_file_sort();

    return 0;
}

void gen_rand_int_file(std::string fname, int sample_osize) {
    std::ofstream fout(fname.c_str());

    for(int i = 0; i < sample_osize; ++i) {
        fout << rand() << std::endl;
    }
}

void test_file_sort() {
    using namespace file_sort;

    int random_size = 250000;
    int buffer_size = 2500;
    std::string in_file = "random_ints.txt", out_file = "results.txt";

    gen_rand_int_file(in_file, random_size);

    FileSort<int> fsort(in_file, out_file);

    fsort.sort();

    std::cout << "Count for infile: " << fsort.count_infile() << std::endl;

    std::cout << "Count for outfile: " << fsort.count_outfile() << std::endl;

    std::cout << "Output file is sorted: " << fsort.validate_sorted()
              << std::endl;
}
