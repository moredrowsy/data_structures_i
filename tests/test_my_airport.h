#ifndef TEST_MY_AIRPORT_H
#define TEST_MY_AIRPORT_H

#include <cassert>
#include <iostream>
#include "../include/my_airport.h"

namespace test_my_airport {

void test_averager_basic();
void test_bool_source_basic();
void test_airport_basic();
void test_all();

void test_averager_basic() {
    std::cout << "test_averager_basic: ";

    my_airport::Averager avg;
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
    my_airport::Averager avg;
    my_airport::BoolSource bs0(0), bs25(0.25), bs50, bs75(0.75), bs100(1);

    // test for always false/true in bs0 and bs100
    for(long i = 0; i < 1000000; i++) {
        assert(bs0.query() == false);
        assert(bs100.query() == true);
    }

    // test for probability average 25%
    for(long i = 0; i < 1000000; i++) {
        avg.next_number(static_cast<int>(bs25.query()));
    }
    assert(avg.average() >= 0.249 && avg.average() <= 0.251);
    avg.clear();

    // test for probability average 50%
    for(long i = 0; i < 1000000; i++) {
        avg.next_number(static_cast<int>(bs50.query()));
    }
    assert(avg.average() >= 0.499 && avg.average() <= 0.501);
    avg.clear();

    // test for probability average 75%
    for(long i = 0; i < 1000000; i++) {
        avg.next_number(static_cast<int>(bs75.query()));
    }
    assert(avg.average() >= 0.749 && avg.average() <= 0.751);
    avg.clear();

    std::cout << "passed." << std::endl;
}

void test_airport_basic() {
    std::cout << "test_airport_basic: ";

    std::cout << "case 1...";
    // test airport with landing time = 0, takeoff time = 0
    my_airport::Airport ap1(0, 0);

    // expect default values
    assert(ap1.has_clearance() == true);
    assert(ap1.is_departing() == false);
    assert(ap1.is_landing() == false);

    // expect zero airplane
    ap1.one_second();
    assert(ap1.has_clearance() == true);
    assert(ap1.is_departing() == false);
    assert(ap1.is_landing() == false);

    // expect default values when 0 landing time
    ap1.land_plane();
    assert(ap1.has_clearance() == true);
    assert(ap1.is_departing() == false);
    assert(ap1.is_landing() == false);

    // expect one airplane
    ap1.one_second();
    assert(ap1.has_clearance() == true);
    assert(ap1.is_departing() == false);
    assert(ap1.is_landing() == false);

    // expect default values when 0 takeoff time
    ap1.depart_plane();
    assert(ap1.has_clearance() == true);
    assert(ap1.is_departing() == false);
    assert(ap1.is_landing() == false);

    // expect zero airplane
    ap1.one_second();
    assert(ap1.has_clearance() == true);
    assert(ap1.is_departing() == false);
    assert(ap1.is_landing() == false);

    // expect two airplanes landed
    ap1.land_plane();
    ap1.land_plane();
    assert(ap1.has_clearance() == true);
    assert(ap1.is_departing() == false);
    assert(ap1.is_landing() == false);

    // expect no airplanes
    ap1.depart_plane();
    ap1.depart_plane();
    assert(ap1.has_clearance() == true);
    assert(ap1.is_departing() == false);
    assert(ap1.is_landing() == false);

    std::cout << "passed, case 2...";

    // test airport with default values(landing time = 5, takeoff time = 15)
    my_airport::Airport ap2;

    // expect default values
    assert(ap2.has_clearance() == true);
    assert(ap2.is_departing() == false);
    assert(ap2.is_landing() == false);

    // expect zero airplane
    ap1.one_second();
    assert(ap2.has_clearance() == true);
    assert(ap2.is_departing() == false);
    assert(ap2.is_landing() == false);

    // expect no has_clearance and is_landing true
    ap2.land_plane();
    assert(ap2.has_clearance() == false);
    assert(ap2.is_departing() == false);
    assert(ap2.is_landing() == true);

    // expect same values after one second
    ap2.one_second();
    assert(ap2.has_clearance() == false);
    assert(ap2.is_departing() == false);
    assert(ap2.is_landing() == true);

    // expect same values after four seconds
    ap2.one_second();
    ap2.one_second();
    ap2.one_second();
    assert(ap2.has_clearance() == false);
    assert(ap2.is_departing() == false);
    assert(ap2.is_landing() == true);

    // expect values after five seconds:
    // with one airplane, has clearance and not is_landing
    ap2.one_second();
    assert(ap2.has_clearance() == true);
    assert(ap2.is_departing() == false);
    assert(ap2.is_landing() == false);

    // expect values after six seoncds: same as above
    ap2.one_second();
    assert(ap2.has_clearance() == true);
    assert(ap2.is_departing() == false);
    assert(ap2.is_landing() == false);

    // expect values after calling depart_plane
    ap2.depart_plane();
    assert(ap2.has_clearance() == false);
    assert(ap2.is_departing() == true);
    assert(ap2.is_landing() == false);

    // expect values after one second
    ap2.one_second();
    assert(ap2.has_clearance() == false);
    assert(ap2.is_departing() == true);
    assert(ap2.is_landing() == false);

    // expect values after fourteen seconds
    for(int i = 0; i < 13; ++i) {
        ap2.one_second();
    }
    assert(ap2.has_clearance() == false);
    assert(ap2.is_departing() == true);
    assert(ap2.is_landing() == false);

    // epected values after fifteen seconds
    ap2.one_second();
    assert(ap2.has_clearance() == true);
    assert(ap2.is_departing() == false);
    assert(ap2.is_landing() == false);

    // epected same values after sixteen seconds
    ap2.one_second();
    assert(ap2.has_clearance() == true);
    assert(ap2.is_departing() == false);
    assert(ap2.is_landing() == false);

    std::cout << "passed." << std::endl;
}

void test_all() {
    std::cout << "Unit tests for my_airport." << std::endl;

    test_averager_basic();
    test_bool_source_basic();
    test_airport_basic();
}

}  // namespace test_my_airport

#endif  // TEST_MY_AIRPORT_H
