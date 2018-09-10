/*******************************************************************************
 * AUTHOR        : Thuan Tang
 * ID            : 00991588
 * CLASS         : CS008
 * ASSIGNMENT 02 : Airport Simulation
 * DESCRIPTION   : This application will simulate an airport. The simulation
 *      will take paramters of landing time, takeoff time, landing probability,
 *      takeoff probability, fuel limit before plane crashes and simulation's
 *      total time. Main program will call simulate_airport for multiple runs
 *      and with different parameters. Assumptions are: unlimited source of
 *      planes incoming and outgoing.
 ******************************************************************************/
#include <iomanip>                  // output formatting
#include <iostream>                 // output stream
#include "../include/my_airport.h"  // Airport/Averager/BoolSource class
#include "../include/my_queue.h"    // Queue class

// main airport simulation program
void simulate_airport(unsigned int landing_time, double landing_probability,
                      unsigned int takeoff_time, double takeoff_probability,
                      unsigned int fuel_limit, unsigned int total_time);

// print parameters with header format
void print_param(unsigned int landing_time, double landing_probability,
                 unsigned int takeoff_time, double takeoff_probability,
                 unsigned int fuel_limit, unsigned int total_time);

int main() {
    unsigned int landing_time = 0, takeoff_time = 0, fuel_limit, total_time = 0;
    double takeoff_probability = 0, landing_probability = 0;
    std::string header;

    // start simulation with default values
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

    // start simulation with 10k seconds duration
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

    // start simulation with 100k seconds duration
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

/*******************************************************************************
 * DESCRIPTION:
 *  This function implements the logic of the Airport, Averager, BoolSource,
 *  and Queue class to simulate the landing and takeoff wait times in an
 *  airport.
 *
 * PRE-CONDITIONS:
 *  unsigned int landing_time : time it takes for plane to land
 *  double landing_probability: probability of plane arriving to land
 *  unsigned int takeoff_time : time it takes for plane to takeoff
 *  double takeoff_probability: probability of plane departing to takeoff
 *  unsigned int fuel_limit   : time limit before plane crashes in landing queue
 *  unsigned int total_time   : total simulation time
 *
 * POST-CONDITIONS:
 *  Output for simulation results
 *
 * RETURN:
 *  none
 ******************************************************************************/
void simulate_airport(unsigned int landing_time, double landing_probability,
                      unsigned int takeoff_time, double takeoff_probability,
                      unsigned int fuel_limit, unsigned int total_time) {
    using namespace my_airport;

    my_queue::Queue<unsigned int> landing_queue,  // queue of landing times
        takeoff_queue;                            // queue of take times
    BoolSource arrival(landing_probability),      // probability to land
        takeoff(takeoff_probability);             // probability to takeoff
    Airport airport(landing_time, takeoff_time);  // airport simulator
    Averager landing_avg,                         // average landing wait times
        takeoff_avg;                              // average takeoff wait times
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
        // depart_plane but prioritize landing
        if(airport.has_clearance()) {
            if(!landing_queue.empty()) {
                airport.land_plane();
                landing_avg.next_number(current_time - landing_queue.pop());
            } else if(!takeoff_queue.empty()) {
                airport.depart_plane();
                takeoff_avg.next_number(current_time - takeoff_queue.pop());
            }
        }

        // sync Airport's timer for one second
        airport.one_second();
    }

    // print various simulation variables
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

/*******************************************************************************
 * DESCRIPTION:
 *  This function prints the header with initial parameters for the simulation
 *  output.
 *
 * PRE-CONDITIONS:
 *  unsigned int landing_time : time it takes for plane to land
 *  double landing_probability: probability of plane arriving to land
 *  unsigned int takeoff_time : time it takes for plane to takeoff
 *  double takeoff_probability: probability of plane departing to takeoff
 *  unsigned int fuel_limit   : time limit before plane crashes in landing queue
 *  unsigned int total_time   : total simulation time
 *
 * POST-CONDITIONS:
 *  Output for parameters header
 *
 * RETURN:
 *  none
 ******************************************************************************/
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
