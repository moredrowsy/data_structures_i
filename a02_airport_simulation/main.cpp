/*******************************************************************************
 * AUTHOR        : Thuan Tang
 * ID            : 00991588
 * CLASS         : CS008
 * ASSIGNMENT 02 : Airport Simulation
 * DESCRIPTION   : This application will simulate an airport. The simulation
 *      will take paramters of landing time, takeoff time, landing probability,
 *      takeoff probability, fuel limit before plane crashes and simulation's
 *      total time.
 ******************************************************************************/
#include <iomanip>
#include <iostream>
#include "../include/my_airport.h"
#include "../include/my_queue.h"

void simulate_airport(unsigned int landing_time, double landing_probability,
                      unsigned int takeoff_time, double takeoff_probability,
                      unsigned int fuel_limit, unsigned int total_time);
void print_param(unsigned int landing_time, double landing_probability,
                 unsigned int takeoff_time, double takeoff_probability,
                 unsigned int fuel_limit, unsigned int total_time);

int main() {
    unsigned int landing_time, takeoff_time, fuel_limit, total_time;
    double takeoff_probability, landing_probability;
    std::string header;

    // init parameters
    landing_time = 5;
    takeoff_time = 15;
    landing_probability = 0.1;
    takeoff_probability = 0.08;
    fuel_limit = 20;
    total_time = 1440;

    // print header
    header = "Simulation parameters with default values";
    std::cout << std::left;
    std::cout << header << std::endl;
    for(unsigned i = 0; i < header.size(); ++i) {
        std::cout << "-";
    }
    std::cout << std::endl;

    // run multiple simulations with default values
    print_param(landing_time, landing_probability, takeoff_time,
                takeoff_probability, fuel_limit, total_time);
    for(int i = 0; i < 5; ++i) {
        std::cout << std::right;
        std::cout << std::setw(2) << i + 1 << " ";
        simulate_airport(landing_time, landing_probability, takeoff_time,
                         takeoff_probability, fuel_limit, total_time);
        std::cout << std::endl << std::endl;
    }
    std::cout << std::endl << std::endl;

    // init parameters
    landing_time = 5;
    takeoff_time = 15;
    landing_probability = 0.1;
    takeoff_probability = 0.08;
    fuel_limit = 20;
    total_time = 10000;

    // print header
    header = "Simulation parameters with total time = 10k seconds";
    std::cout << std::left;
    std::cout << header << std::endl;
    for(unsigned i = 0; i < header.size(); ++i) {
        std::cout << "-";
    }
    std::cout << std::endl;

    // run multiple simulations with 0 landing and takeoff times
    print_param(landing_time, landing_probability, takeoff_time,
                takeoff_probability, fuel_limit, total_time);
    for(int i = 0; i < 5; ++i) {
        std::cout << std::right;
        std::cout << std::setw(2) << i + 1 << " ";
        simulate_airport(landing_time, landing_probability, takeoff_time,
                         takeoff_probability, fuel_limit, total_time);
        std::cout << std::endl << std::endl;
    }
    std::cout << std::endl << std::endl;

    // init parameters
    landing_time = 5;
    takeoff_time = 15;
    landing_probability = 0.1;
    takeoff_probability = 0.08;
    fuel_limit = 20;
    total_time = 100000;

    // print header
    header = "Simulation parameters with total time = 100k seconds";
    std::cout << std::left;
    std::cout << header << std::endl;
    for(unsigned i = 0; i < header.size(); ++i) {
        std::cout << "-";
    }
    std::cout << std::endl;

    // run multiple simulations with 0 landing and takeoff times
    print_param(landing_time, landing_probability, takeoff_time,
                takeoff_probability, fuel_limit, total_time);
    for(int i = 0; i < 5; ++i) {
        std::cout << std::right;
        std::cout << std::setw(2) << i + 1 << " ";
        simulate_airport(landing_time, landing_probability, takeoff_time,
                         takeoff_probability, fuel_limit, total_time);
        std::cout << std::endl << std::endl;
    }

    return 0;
}

void simulate_airport(unsigned int landing_time, double landing_probability,
                      unsigned int takeoff_time, double takeoff_probability,
                      unsigned int fuel_limit, unsigned int total_time) {
    using namespace my_airport;

    my_queue::Queue<unsigned int> landing_queue,  // list of times
        takeoff_queue;                            // list of times
    BoolSource arrival(landing_probability),      // probability landing
        takeoff(takeoff_probability);             // probability takeoff
    Airport airport(landing_time, takeoff_time);  // washing machine
    Averager landing_avg,                         // average wait times
        takeoff_avg;                              // average wait times
    unsigned int current_time,                    // current time for simulation
        crashes = 0,                              // number of planes crashed
        total_landings_in_queue = 0,              // total queue count
        total_takeoffs_in_queue = 0;              // total queue count

    for(current_time = 0; current_time < total_time; ++current_time) {
        // check for plane crashes
        if(!landing_queue.empty() &&
           (current_time - landing_queue.front()) == fuel_limit) {
            landing_queue.pop();
            ++crashes;
        }

        // add to landing queue when arrival's query is true
        if(arrival.query()) {
            landing_queue.push(current_time);
            ++total_landings_in_queue;
        }

        // add to takeoff queue when takeoff's query is true
        if(takeoff.query()) {
            takeoff_queue.push(current_time);
            ++total_takeoffs_in_queue;
        }

        // when airport has clearance and queue's not empty, call land_plane or
        // depart_plane
        if(airport.has_clearance()) {
            if(!landing_queue.empty()) {
                airport.land_plane();
                landing_avg.next_number(current_time - landing_queue.pop());
            } else if(!takeoff_queue.empty()) {
                airport.depart_plane();
                takeoff_avg.next_number(current_time - takeoff_queue.pop());
            }
        }

        // sync Airport's timer
        airport.one_second();
    }

    std::cout << std::right;
    std::cout << std::setw(6) << landing_avg.count() << " " << std::setw(8)
              << takeoff_avg.count() << " " << std::setw(7) << crashes << " ";
    if(landing_avg.count() > 0) {
        std::cout << std::fixed << std::setprecision(3) << std::setw(11)
                  << landing_avg.average() << " ";
    } else {
        std::cout << std::fixed << std::setprecision(3) << std::setw(11) << 0
                  << " ";
    }
    if(takeoff_avg.count() > 0) {
        std::cout << std::fixed << std::setprecision(3) << std::setw(11)
                  << takeoff_avg.average() << " ";
    } else {
        std::cout << std::fixed << std::setprecision(3) << std::setw(11) << 0
                  << " ";
    }
    if(total_landings_in_queue > landing_avg.count()) {
        std::cout << std::setw(16)
                  << total_landings_in_queue - landing_avg.count() - crashes
                  << " ";
    } else {
        std::cout << std::setw(16) << 0 << " ";
    }
    if(total_takeoffs_in_queue > takeoff_avg.count()) {
        std::cout << std::setw(16)
                  << total_takeoffs_in_queue - takeoff_avg.count() << " ";
    } else {
        std::cout << std::setw(16) << 0 << " ";
    }
}

void print_param(unsigned int landing_time, double landing_probability,
                 unsigned int takeoff_time, double takeoff_probability,
                 unsigned int fuel_limit, unsigned int total_time) {
    std::string seperator =
        "-- ------ -------- ------- ----------- ----------- ---------------- "
        "----------------";

    std::cout << std::left;
    std::cout << std::setw(22) << "Landing time"
              << ": " << landing_time << std::endl
              << std::setw(22) << "Takeoff time"
              << ": " << takeoff_time << std::endl
              << std::setw(22) << "Landing probability"
              << ": " << landing_probability << std::endl
              << std::setw(22) << "Takeoff probability"
              << ": " << takeoff_probability << std::endl
              << std::setw(22) << "Fuel limit time"
              << ": " << fuel_limit << std::endl
              << std::setw(22) << "Total simulation time"
              << ": " << total_time << std::endl
              << std::endl
              << "Simulation runs" << std::endl
              << seperator << std::endl;
    std::cout << std::right;
    std::cout << " # "
              << "Landed "
              << "Departed "
              << "Crashes "
              << "Avg Landing "
              << "Avg Takeoff "
              << "In Landing Queue "
              << "In Takeoff Queue " << std::endl
              << seperator << std::endl;
}
