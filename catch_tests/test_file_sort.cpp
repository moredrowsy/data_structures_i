#include "../include/file_sort.h"
#include "../lib/catch.hpp"

void gen_rand_int_file(std::string fname, int sample_osize = 250000);

SCENARIO("FileSort", "[fsort]") {
    using namespace file_sort;

    int random_size = 250000, buffer_size = 2500;
    std::string test_file = "test.txt", result_file = "result.txt";

    GIVEN("varying data sizes, varying block sizes > 0") {
        FileSort<int> fsort(test_file, result_file);

        random_size = 1;
        buffer_size = 1;

        for(int i = 0; i < 25; ++i) {
            random_size = i;
            for(int j = 1; j < 50; ++j) {  // BUFFER SIZE CAN NOT BE ZERO!
                buffer_size = j;
                gen_rand_int_file(test_file, random_size);

                fsort.set_block_size(buffer_size);
                fsort.sort();

                // assert data count from infile matches outfile
                REQUIRE(fsort.count_infile() == fsort.count_outfile());

                // if outfile has data, assert that data is sorted
                if(fsort.count_infile())
                    REQUIRE(fsort.validate_sorted() == true);

                // clean up associated files to repare for next iteration
                fsort.remove_in_file();
                fsort.remove_out_file();
                fsort.cleanup();
            }
        }
    }
}

void gen_rand_int_file(std::string fname, int sample_osize) {
    std::ofstream fout(fname.c_str(), std::ios::binary);

    for(int i = 0; i < sample_osize; ++i) {
        fout << rand() << std::endl;
    }
}