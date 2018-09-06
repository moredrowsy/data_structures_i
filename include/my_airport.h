#ifndef MY_AIRPORT_H
#define MY_AIRPORT_H

#include <cstdlib>

namespace my_airport {

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

class Airport {
public:
    Airport(unsigned int landing = 5, unsigned int takeoff = 15)
        : _landing_time(landing),
          _takeoff_time(takeoff),
          _landing_countdown(0),
          _takeoff_countdown(0) {}

    bool has_clearance() const;
    bool is_departing() const;
    bool is_landing() const;

    void depart_plane();
    void land_plane();
    void one_second();

    // private:
    unsigned int _landing_time;       // time to land in seconds
    unsigned int _takeoff_time;       // time to takeoff in seconds
    unsigned int _landing_countdown;  // countdown to landing
    unsigned int _takeoff_countdown;  // countdown to takeoff
};

}  // namespace my_airport

#endif  // MY_AIRPORT_H
