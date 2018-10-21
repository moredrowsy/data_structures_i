#include "../include/file_sort.h"
#include "../lib/catch.hpp"

void gen_rand_int_file(std::string fname, int sample_osize = 250000);

SCENARIO("FileSort", "[fsort]") {
    using namespace file_sort;

    bool is_sorted = false;
    int random_size = 250000, buffer_size = 2500, in_count = -1, out_count = -1;
    std::ifstream fin;
    std::ofstream fout;
    std::string test_file = "test.txt", result_file = "result.txt";

    GIVEN("varying data sizes, varying block sizes > 0") {
        FileSort<int> fsort;

        for(int i = 0; i < 25; ++i) {
            random_size = i;

            for(int j = 1; j < 50; ++j) {  // BUFFER SIZE CAN NOT BE ZERO!
                // open and close fstreams in binary mode!
                fin.close();
                fout.close();

                gen_rand_int_file(test_file, random_size);

                buffer_size = j;
                fsort.set_buffer(buffer_size);

                fin.open(test_file, std::ios::binary);
                fout.open(result_file, std::ios::binary);

                fin >> fsort;
                fout << fsort;

                fin.close();
                fout.close();

                fin.open(test_file, std::ios::binary);
                in_count = count_file<int>(fin);
                fin.close();

                fin.open(result_file, std::ios::binary);
                out_count = count_file<int>(fin);
                is_sorted = validate_sorted_file<int>(fin);
                fin.close();

                if(i != 0) {
                    REQUIRE(in_count != 0);
                    REQUIRE(out_count != 0);
                }

                REQUIRE(in_count == out_count);
                REQUIRE(is_sorted == true);

                // clean up associated files to repare for next iteration
                std::remove(test_file.c_str());
                std::remove(result_file.c_str());
            }
        }
    }

    std::remove(test_file.c_str());
    std::remove(result_file.c_str());
}

void gen_rand_int_file(std::string fname, int sample_osize) {
    std::ofstream fout(fname.c_str(), std::ios::binary);

    for(int i = 0; i < sample_osize; ++i) {
        fout << rand() << std::endl;
    }
}