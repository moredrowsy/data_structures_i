/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : airport
 * DESCRIPTION : This header declares three classes: Averager, BoolSource, and
 *      Airport. All three classes work in conjunction to simulate an airport.
 *      Averager will count and calculate unsigned seconds for time.
 *      BoolSource will return boolean for a given probability.
 *      Airport will keep track of landing and takeoff.
 ******************************************************************************/
#ifndef AIRPORT_H
#define AIRPORT_H

#include <cassert>  // assertions
#include <cstdlib>  // provides std::size_t and random generator

namespace airport {

class Averager {
public:
    // CONSTRUCTORS
    Averager() : _count(0), _sum(0) {}

    // ACCESSORS
    double average() const;     // calculates and return average
    std::size_t count() const;  // return count

    // MUTATORS
    void clear();                    // clear all values
    void next_number(double value);  // input number into averager

private:
    std::size_t _count;  // count of all numbers
    double _sum;         // sum of all numbers
};

class BoolSource {
public:
    // CONSTRUCTORS
    BoolSource(double probability = 0.5);

    // ACCESSORS
    bool query() const;  // return boolean

private:
    double _probability;  // probability from 0 to 1
};

class Airport {
public:
    // CONSTRUCTORS
    Airport(unsigned int landing = 5, unsigned int takeoff = 15)
        : _landing_time(landing),
          _takeoff_time(takeoff),
          _landing_countdown(0),
          _takeoff_countdown(0) {}

    // ACCESSORS
    bool has_clearance() const;  // return boolean for air clearance
    bool is_departing() const;   // return boolean when plane is departing
    bool is_landing() const;     // return boolean when plane is landing

    // MUTATORS
    void depart_plane();  // set countdown to takeoff time
    void land_plane();    // set countdown to landing time
    void one_second();    // start countdown by one second

private:
    unsigned int _landing_time;       // time to land in seconds
    unsigned int _takeoff_time;       // time to takeoff in seconds
    unsigned int _landing_countdown;  // countdown to landing
    unsigned int _takeoff_countdown;  // countdown to takeoff
};

}  // namespace airport

#endif  // AIRPORT_H
