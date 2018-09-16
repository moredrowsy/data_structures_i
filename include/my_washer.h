#include <cassert>  // assertions
#include <cstdlib>  // random generator
#include "../include/my_queue.h"

namespace my_washer {

class Averager {
public:
    Averager() : _count(0), _sum(0) {}

    double average() const;
    void clear();
    std::size_t count() const;
    void next_number(double value);

    // private:
    std::size_t _count;  // count of all numbers
    double _sum;         // sum of all numbers
};

class BoolSource {
public:
    BoolSource(double probability = 0.5);

    bool query() const;

private:
    double _probability;  // probability from 0 to 1
};

class Washer {
public:
    Washer(unsigned int seconds = 60)
        : _seconds_per_wash(seconds), _wash_time_left(0) {}

    bool is_busy() const;
    void one_second();
    void start_washing();

private:
    unsigned int _seconds_per_wash;  // duration for one wash cycle in seconds
    unsigned int _wash_time_left;    // current washing time in seconds
};

}  // namespace my_washer
