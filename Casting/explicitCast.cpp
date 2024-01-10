#include <iostream>

int main(int argc, char* argv[]) {

    unsigned int goal = 5;
    unsigned int reward = 1000000000;
    unsigned long long int result = static_cast<unsigned long long int>(goal) * reward;

    std::cout << "result: " << result << std::endl;

    return 0;
}