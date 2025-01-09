#include <iostream>
#include <optional>

/*
Null pointers have historically been one of the most dangerous sources of bugs in C++ programs.
Dereferencing a null pointer leads to undefined behavior, which can cause crashes, memory
corruption, or security vulnerabilities. Traditionally, programmers have used raw pointers and
manually checked for null before dereferencing them. However, this approach can easily lead to
mistakes. In C++17, the introduction of std::optional offers a safer, more expressive
alternative for handling values that may or may not be present.

std::optional is a wrapper template provided by the C++17 standard library, designed to
represent an object that may or may not hold a value.

reference: simplyfycpp.org
*/

std::optional<int> findValue(bool condition) {
    if (condition) {
        return 1;
    }
    return std::nullopt;
}

int main(int argc, char* argv[]) {
    auto result = findValue(true); // check if value exist in the runtime

    if (result) {
        std::cout << "Found value: " << *result << std::endl;
    } else {
        std::cout << "No value found" << std::endl;
    }
    
    return 0;
}