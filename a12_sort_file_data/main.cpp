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

    int random_size = 23;
    int buffer_size = 5;
    std::string in_file = "random_ints.txt", out_file = "results.txt";

    gen_rand_int_file(in_file, random_size);

    std::ifstream fin(in_file.c_str(), std::ios::binary);
    std::ofstream fout(out_file.c_str(), std::ios::binary);
    FileSort<int> fsort(buffer_size);

    fin >> fsort;
    fout << fsort;
    fout.close();

    fin.close();
    fin.open(in_file, std::ios::binary);
    std::cout << "Count for infile: " << count_file<int>(fin) << std::endl;

    fin.close();
    fin.open(out_file, std::ios::binary);
    std::cout << "Count for outfile: " << count_file<int>(fin) << std::endl;

    fin.close();
    fin.open(out_file, std::ios::binary);
    std::cout << "Output file is sorted: " << validate_sorted_file<int>(fin)
              << std::endl;
}
