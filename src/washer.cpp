#include "../include/washer.h"

namespace washer {

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
    _count++;
    _sum += value;
}

/* Implementation for class BoolSource */
BoolSource::BoolSource(double probability) : _probability(probability) {
    assert(_probability >= 0 && _probability <= 1);
}

bool BoolSource::query() const { return rand() < _probability * RAND_MAX; }

/* Implementation for class Washer */
bool Washer::is_busy() const { return _wash_time_left > 0; }

void Washer::one_second() {
    if(is_busy()) {
        _wash_time_left--;
    }
}

void Washer::start_washing() {
    if(!is_busy()) {
        _wash_time_left = _seconds_per_wash;
    }
}

}  // namespace washer
