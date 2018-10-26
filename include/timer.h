#ifndef TIMER_H
#define TIMER_H

#include <chrono>  // high_resolution_clock
#include <ctime>   // clock()

namespace timer {

class ClockTimer {
public:
    // CONSTRUCTORS
    ClockTimer() : _start(0), _stop(0) {}

    // ACCESSORS
    std::clock_t clock_ticks() const { return _stop - _start; }
    double seconds() const { return (double)(_stop - _start) / CLOCKS_PER_SEC; }

    // MUTATORS
    void start() { _start = std::clock(); }
    void stop() { _stop = std::clock(); }

private:
    std::clock_t _start;
    std::clock_t _stop;
};

class ChronoTimer {
public:
    // CONSTRUCTORS
    ChronoTimer() : _start(), _stop() {}

    // ACCESSORS
    double seconds() const {
        using namespace std::chrono;
        return duration_cast<duration<double>>(_stop - _start).count();
    };

    // MUTATORS
    void start() { _start = std::chrono::high_resolution_clock::now(); }
    void stop() { _stop = std::chrono::high_resolution_clock::now(); }

private:
    std::chrono::high_resolution_clock::time_point _start;
    std::chrono::high_resolution_clock::time_point _stop;
};

}  // namespace timer

#endif  // TIMER_H
