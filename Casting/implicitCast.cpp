#include <iostream>

int main(int argc, char* argv[]) {

    unsigned int goal = 5;
    unsigned long long int reward = 1000000000;
    unsigned long long int result = goal * reward; // the result will be the highest data type byte size

    std::cout << "result: " << result << std::endl;

    return 0;
}