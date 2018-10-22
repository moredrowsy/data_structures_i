#include "../include/file_sort.h"
#include "../lib/catch.hpp"

void gen_rand_int_file(std::string fname, int sample_osize = 250000);

SCENARIO("FileSort", "[fsort]") {
    using namespace file_sort;

    bool is_sorted = false;
    int random_size = 250000, buffer_size = 2500, in_count = -1, out_count = -1;
    FileSort<int> fsort;
    std::ifstream fin;
    std::ofstream fout;
    std::string test_file = "test.txt", result_file = "result.txt";

    GIVEN("varying data sizes, varying block sizes > 0") {
        int random_size_limit = 50;
        int buffer_size_limit = 100;

        for(int i = 0; i < random_size_limit; ++i) {
            random_size = i;

            gen_rand_int_file(test_file, random_size);

            for(int j = 1; j < buffer_size_limit; ++j) {
                buffer_size = j;

                std::ifstream fin(test_file.c_str(), std::ios::binary);
                std::ofstream fout(result_file.c_str(), std::ios::binary);
                fsort.set_buffer(buffer_size);

                fin >> fsort;
                fin.close();

                fout << fsort;
                fout.close();

                in_count = count_file<int>(test_file);
                if(i != 0) REQUIRE(in_count > 0);

                out_count = count_file<int>(result_file);
                if(i != 0) REQUIRE(out_count > 0);

                is_sorted = validate_sorted_file<int>(result_file);
                REQUIRE(is_sorted == true);

                // clean up perepare for next iteration
                in_count = 0;
                out_count = 0;
                is_sorted = false;
            }
        }
    }

    std::remove(test_file.c_str());
    std::remove(result_file.c_str());
}

void gen_rand_int_file(std::string fname, int sample_osize) {
    std::ofstream fout(fname.c_str());

    srand(time(nullptr));
    for(int i = 0; i < sample_osize; ++i) fout << rand() << std::endl;
}
