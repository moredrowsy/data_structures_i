#ifndef TIMER_H
#define TIMER_H

#include <ctime>  // clock()

namespace timer {

class Timer {
public:
    // CONSTRUCTORS
    Timer() : _start(0), _end(0) {}

    // ACCESSORS
    std::clock_t duration() const { return _end - _start; }

    // MUTATORS
    void start() { _start = std::clock(); }
    void stop() { _end = std::clock(); }

private:
    std::clock_t _start, _end;
};

}  // namespace timer

#endif  // TIMER_H
