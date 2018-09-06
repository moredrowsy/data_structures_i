#include "../include/my_airport.h"
#include <cassert>
#include <cstdlib>

namespace my_airport {

/* Implementation for class Averager */
double Averager::average() const {
    assert(_count > 0);

    return _sum / _count;
}

void Averager::clear() {
    _count = 0;
    _sum = 0;
}

std::size_t Averager::count() const { return _count; }

void Averager::next_number(double value) {
    ++_count;
    _sum += value;
}

/* Implementation for class BoolSource */
BoolSource::BoolSource(double probability) {
    assert(probability >= 0 && probability <= 1);
    _probability = probability;
}

bool BoolSource::query() const { return rand() < _probability * RAND_MAX; }

/* Implementation for class Airport */
bool Airport::has_clearance() const { return !is_landing() && !is_departing(); }

bool Airport::is_departing() const { return _takeoff_countdown > 0; }

bool Airport::is_landing() const { return _landing_countdown > 0; }

void Airport::depart_plane() {
    // assert has clearance
    assert(has_clearance());

    // start takeoff time when takeoff time is > 0
    if(_takeoff_time > 0) {
        _takeoff_countdown = _takeoff_time;
    }
}

void Airport::land_plane() {
    // assert has clearance
    assert(has_clearance());

    // start landing time when landing time is > 0
    if(_landing_time > 0) {
        _landing_countdown = _landing_time;
    }
}

void Airport::one_second() {
    if(is_landing()) {
        assert(_landing_countdown != 0);
        --_landing_countdown;
    }

    if(is_departing()) {
        assert(_takeoff_countdown != 0);
        --_takeoff_countdown;
    }
}

}  // namespace my_airport
