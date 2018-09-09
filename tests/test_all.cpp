#include <iostream>
#include "test_my_airport.h"
#include "test_my_node.h"
#include "test_my_queue.h"
#include "test_my_stack.h"

int main() {
    test_my_node::test_all();
    std::cout << std::endl;
    test_my_queue::test_all();
    std::cout << std::endl;
    test_my_stack::test_all();
    std::cout << std::endl;
    test_my_airport::test_all();

    return 0;
}
