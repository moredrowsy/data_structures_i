#include "../include/my_airport.h"  // my_airport declarations

namespace my_airport {

/*******************************************************************************
 * DESCRIPTION:
 *  Calculate average of all unsigned numbers.
 *
 * PRE-CONDITIONS:
 *  count is positive
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  Average number
 ******************************************************************************/
double Averager::average() const {
    assert(_count > 0);

    return _sum / _count;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Return count.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  size_t count
 ******************************************************************************/
std::size_t Averager::count() const { return _count; }

/*******************************************************************************
 * DESCRIPTION:
 *  Assign count and sum to zero.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  _count: assign to 0
 *  _sum  : assign to 0
 *
 * RETURN:
 *  none
 ******************************************************************************/
void Averager::clear() {
    _count = 0;
    _sum = 0;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Input a new number.
 *
 * PRE-CONDITIONS:
 *  value: a number of double type
 *
 * POST-CONDITIONS:
 *  _count: increment by one
 *  _sum  : increment by value
 *
 * RETURN:
 *  none
 ******************************************************************************/
void Averager::next_number(double value) {
    ++_count;
    _sum += value;
}

/*******************************************************************************
 * DESCRIPTION:
 *  Constructor for BoolSource. Checks for param. assertion.
 *
 * PRE-CONDITIONS:
 *  probability: between inclusive 0 and 1.
 *
 * POST-CONDITIONS:
 *  _probability: assign to param.
 *
 * RETURN:
 *  none
 ******************************************************************************/
BoolSource::BoolSource(double probability) : _probability(probability) {
    assert(_probability >= 0 && _probability <= 1);
}

/*******************************************************************************
 * DESCRIPTION:
 *  Check for probability from rand function.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  Boolean condition
 ******************************************************************************/
bool BoolSource::query() const { return rand() < _probability * RAND_MAX; }

/*******************************************************************************
 * DESCRIPTION:
 *  Check for airport's air clearance.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  Boolean condition
 ******************************************************************************/
bool Airport::has_clearance() const { return !is_landing() && !is_departing(); }

/*******************************************************************************
 * DESCRIPTION:
 *  Check for when a departing plane is occupying airspace.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  Boolean condition
 ******************************************************************************/
bool Airport::is_departing() const { return _takeoff_countdown > 0; }

/*******************************************************************************
 * DESCRIPTION:
 *  Check for when a landing plane is occupying airspace.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  Boolean condition
 ******************************************************************************/
bool Airport::is_landing() const { return _landing_countdown > 0; }

/*******************************************************************************
 * DESCRIPTION:
 *  Assert airport has clearance. Assign takeoff countdown to takeoff time.
 *
 * PRE-CONDITIONS:
 *  has_clearance is true
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
void Airport::depart_plane() {
    // assert has clearance
    assert(has_clearance());

    // start takeoff time when takeoff time is > 0
    if(_takeoff_time > 0) {
        _takeoff_countdown = _takeoff_time;
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Assert airport has clearance. Assign landing countdown to landing time.
 *
 * PRE-CONDITIONS:
 *  has_clearance is true
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
void Airport::land_plane() {
    // assert has clearance
    assert(has_clearance());

    // start landing time when landing time is > 0
    if(_landing_time > 0) {
        _landing_countdown = _landing_time;
    }
}

/*******************************************************************************
 * DESCRIPTION:
 *  Airport simulated one second. Decrement either landing or takeoff countdown
 *  by one.
 *
 * PRE-CONDITIONS:
 *  none
 *
 * POST-CONDITIONS:
 *  none
 *
 * RETURN:
 *  none
 ******************************************************************************/
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
