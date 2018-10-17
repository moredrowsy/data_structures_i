/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * EXERCISE 01 : Car Wash
 * DESCRIPTION : This application simulates a car wash wait times using Queue,
 *      Average, BoolSource, and Washer.
 ******************************************************************************/
#include <iostream>
#include "../include/washer.h"

void simulate_car_washer(unsigned int wash_time, double arrival_probability,
                         unsigned total_time);

int main() {
    // start simulator
    for(int i = 0; i < 5; ++i) {
        std::cout << "Simulation: " << i + 1 << std::endl;
        simulate_car_washer(240, 0.0025, 6000);
        std::cout << std::endl << std::endl;
    }

    return 0;
}

void simulate_car_washer(unsigned int wash_time, double arrival_p,
                         unsigned total_time) {
    using namespace washer;

    queue::Queue<unsigned int> arrival_times;  // list of arrival times
    BoolSource arrival(arrival_p);             // indicates when an arrival
    Washer washer(wash_time);                  // washing machine
    Averager wait_times;                       // average wait times
    unsigned int current_time;                 // current time for simulation

    std::cout << "Washer duration(sec): " << wash_time << std::endl
              << "Arrival probability per second: " << arrival_p << std::endl
              << "Total simulation(sec): " << total_time << std::endl;

    for(current_time = 0; current_time < total_time; current_time++) {
        // check for arrival
        if(arrival.query()) {
            arrival_times.push(current_time);
        }

        // check if washer can start washing car
        if(!washer.is_busy() && !arrival_times.empty()) {
            // add wait time to averager: current time - arrival time
            wait_times.next_number(current_time - arrival_times.pop());

            // start the washer
            washer.start_washing();
        }

        // update washer
        washer.one_second();
    }

    std::cout << "Number of arrivals: " << wait_times.count() << std::endl;
    if(wait_times.count() > 0) {
        std::cout << "Average wait times(sec): " << wait_times.average()
                  << std::endl;
    }
}
