#ifndef TEST_MY_WASHER_H
#define TEST_MY_WASHER_H

#include <cassert>
#include <iostream>
#include "../include/my_washer.h"

namespace test_my_washer {

void test_averager_basic();
void test_bool_source_basic();
void test_washer_basic();
void test_all();

void test_averager_basic() {
    std::cout << "test_averager_basic: ";

    my_washer::Averager avg;
    int a = 0, b = 1, c = 2, d = 3, e = 4, f = 5;

    avg.next_number(a);
    assert(avg.count() == 1);
    assert(avg.average() == 0);

    avg.next_number(b);
    assert(avg.count() == 2);
    assert(avg.average() == 0.5);

    avg.next_number(c);
    assert(avg.count() == 3);
    assert(avg.average() == 1);

    avg.next_number(d);
    assert(avg.count() == 4);
    assert(avg.average() == 1.5);

    avg.next_number(e);
    assert(avg.count() == 5);
    assert(avg.average() == 2);

    avg.next_number(f);
    assert(avg.count() == 6);
    assert(avg.average() == 2.5);

    std::cout << "passed." << std::endl;
}

void test_bool_source_basic() {
    std::cout << "test_bool_source_basic: ";
    my_washer::Averager avg;
    my_washer::BoolSource bs0(0), bs25(0.25), bs50, bs75(0.75), bs100(1);

    // test for always false/true in bs0 and bs100
    for(long i = 0; i < 1000000; ++i) {
        assert(bs0.query() == false);
        assert(bs100.query() == true);
    }

    // test for probability average 25%
    for(long i = 0; i < 1000000; ++i) {
        avg.next_number(static_cast<int>(bs25.query()));
    }
    assert(avg.average() >= 0.249 && avg.average() <= 0.251);
    avg.clear();

    // test for probability average 50%
    for(long i = 0; i < 1000000; ++i) {
        avg.next_number(static_cast<int>(bs50.query()));
    }
    assert(avg.average() >= 0.499 && avg.average() <= 0.501);
    avg.clear();

    // test for probability average 75%
    for(long i = 0; i < 1000000; ++i) {
        avg.next_number(static_cast<int>(bs75.query()));
    }
    assert(avg.average() >= 0.749 && avg.average() <= 0.751);
    avg.clear();

    std::cout << "passed." << std::endl;
}

void test_washer_basic() {
    std::cout << "test_washer_basic: ";

    my_washer::Washer washer1,  // washing time is 60 seconds
        washer2(0),             // washing time is 0 seconds
        washer3(1),             // washing time is 1 seconds
        washer4(100);           // washing time is 100 seconds

    // test initial states
    assert(washer1.is_busy() == false && washer2.is_busy() == false &&
           washer3.is_busy() == false && washer4.is_busy() == false);

    // start washer
    washer1.start_washing();
    washer2.start_washing();
    washer3.start_washing();
    washer4.start_washing();
    assert(washer1.is_busy() == true && washer2.is_busy() == false &&
           washer3.is_busy() == true && washer4.is_busy() == true);

    // call one_second()
    for(int i = 0; i < 1000; ++i) {
        washer1.one_second();
    }
    for(int i = 0; i < 1000; ++i) {
        washer2.one_second();
    }
    for(int i = 0; i < 1000; ++i) {
        washer3.one_second();
    }
    for(int i = 0; i < 1000; ++i) {
        washer4.one_second();
    }
    assert(washer1.is_busy() == false && washer2.is_busy() == false &&
           washer3.is_busy() == false && washer4.is_busy() == false);

    // start washer again
    washer1.start_washing();
    washer2.start_washing();
    washer3.start_washing();
    washer4.start_washing();
    assert(washer1.is_busy() == true && washer2.is_busy() == false &&
           washer3.is_busy() == true && washer4.is_busy() == true);

    std::cout << "passed." << std::endl;
}

void test_all() {
    std::cout << "Unit tests for my_washer." << std::endl;

    test_averager_basic();
    test_bool_source_basic();
    test_washer_basic();
}

}  // namespace test_my_washer

#endif  // TEST_MY_WASHER_H
