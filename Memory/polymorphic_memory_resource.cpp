#include <iostream>
#include <vector>
#include <memory_resource>

/*
A Polymorphic Memory Resource (PMR) is a part of the C++ standard library introduced in C++17 as part of the C++ Standard Library Extensions (often associated with C++20). The purpose of a Polymorphic Memory Resource is to provide a flexible and efficient way to manage memory allocation in C++ programs, especially for custom memory management strategies, without requiring the use of fixed memory allocation strategies tied to specific containers or other components.

In simpler terms, PMRs allow you to manage memory allocation in a polymorphic (i.e., runtime-configurable) manner, making it possible to swap out different memory management strategies during execution.
*/

int main() {
    // Create a memory resource
    std::pmr::monotonic_buffer_resource pool_resource{1024}; // 1 KB pool

    // Use the memory resource with a polymorphic vector
    std::pmr::vector<int> vec(&pool_resource);

    // Insert some elements
    vec.push_back(10);
    vec.push_back(20);

    // Print elements
    for (const auto& el : vec) {
        std::cout << el << " ";
    }

    return 0;
}