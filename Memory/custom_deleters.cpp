#include <iostream>

/*
In addition to its basic functionality, smart pointers
ptr also supports custom deleters. A
custom deleter is a function, function object, or lambda expression that is invoked when the
object managed by the smart pointers
ptr is deleted. This is useful in scenarios where the
object requires special cleanup procedures, such as when dealing with non-standard resources
like file handles or database connections

reference: simplyfycpp.org
*/

int main(int argc, char* argv[]) {

    // Lambda function
    auto customDeleters = [](int* ptr) {
        std::cout << "Custom deleter called for pointer: " << ptr << std::endl;
        delete ptr;
    };

    std::unique_ptr<int, decltype(customDeleters)> ptr (new int(10), customDeleters);
    
    return 0;
}