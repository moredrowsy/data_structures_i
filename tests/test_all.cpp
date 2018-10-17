#include <iostream>
#include "test_airport.h"
#include "test_ftokenizer.h"
#include "test_node.h"
#include "test_queue.h"
#include "test_stack.h"
#include "test_state_machine.h"
#include "test_stokenizer.h"
#include "test_washer.h"

int main() {
    test_node::test_all();
    std::cout << std::endl;
    test_queue::test_all();
    std::cout << std::endl;
    test_stack::test_all();
    std::cout << std::endl;
    test_washer::test_all();
    std::cout << std::endl;
    test_airport::test_all();
    std::cout << std::endl;
    test_state_machine::test_all();
    std::cout << std::endl;
    test_stokenizer::test_all();
    std::cout << std::endl;
    test_ftokenizer::test_all();

    return 0;
}
