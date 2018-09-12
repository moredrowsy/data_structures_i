#include <iostream>
#include "test_my_airport.h"
#include "test_my_node.h"
#include "test_my_queue.h"
#include "test_my_stack.h"
#include "test_my_washer.h"
#include "test_state_machine.h"

int main() {
    test_my_node::test_all();
    std::cout << std::endl;
    test_my_queue::test_all();
    std::cout << std::endl;
    test_my_stack::test_all();
    std::cout << std::endl;
    test_my_washer::test_all();
    std::cout << std::endl;
    test_my_airport::test_all();
    std::cout << std::endl;
    test_state_machine::test_all();

    return 0;
}
