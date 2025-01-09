#include <iostream>
#include <map>

/*
Structured bindings, introduced in C++17, provide a mechanism for unpacking tuple-like objects
into individual variables. This feature is particularly useful for simplifying code that works with
complex data structures like tuples, pairs, and maps. It improves code clarity, enhances
readability, and reduces the boilerplate code required to access individual elements.

Structured bindings allow you to decompose complex types like tuples, pairs, and other
aggregate types into individual named variables. This feature reduces the need for manual
indexing and improves the readability of the code.

reference: simplyfycpp.org
*/

int main(int argc, char* argv[]) {
    std::map<int, std::string> myMap = {{1, "One"}, {2, "Two"}, {3, "Three"}};

    for (const auto& [key, value] : myMap) {
        std::cout << "Key: " << key << ", Value: " << value << std::endl;
    }

    return 0;
}