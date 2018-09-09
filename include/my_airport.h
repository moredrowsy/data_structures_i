/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : MY_AIRPORT
 * DESCRIPTION : This header defines three classes: Averager, BoolSource, and
 *      Airport. All three classes work in conjunction to simulate an airport.
 *      Averager will count and calculate unsigned seconds for time.
 *      BoolSource will return boolean for a given probability.
 *      Airport will keep track of landing and takeoff.
 ******************************************************************************/
#ifndef MY_AIRPORT_H
#define MY_AIRPORT_H

#include <cstdlib>

namespace my_airport {

class Averager {
public:
    Averager() : _count(0), _sum(0) {}

    double average() const;     // calculates and return average
    std::size_t count() const;  // return count

    void clear();                    // clear all values
    void next_number(double value);  // input number into averager

    // private:
    std::size_t _count;  // count of all numbers
    double _sum;         // sum of all numbers
};

class BoolSource {
public:
    BoolSource(double probability = 0.5);

    bool query() const;  // return boolean

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

    bool has_clearance() const;  // return boolean for air clearance
    bool is_departing() const;   // return boolean when plane is departing
    bool is_landing() const;     // return boolean when plane is landing

    void depart_plane();  // set countdown to takeoff time
    void land_plane();    // set countdown to landing time
    void one_second();    // start countdown by one second

    // private:
    unsigned int _landing_time;       // time to land in seconds
    unsigned int _takeoff_time;       // time to takeoff in seconds
    unsigned int _landing_countdown;  // countdown to landing
    unsigned int _takeoff_countdown;  // countdown to takeoff
};

}  // namespace my_airport

#endif  // MY_AIRPORT_H
