#include <iostream>

/*
The result should be like this:

result1: 705032704
result2: 5000000000

As we can see, the precedence play a vital role
*/

int main(int argc, char* argv[]) {

    unsigned short goal = 5;
    unsigned int reward = 1000000000;
    unsigned long long int coef = 1; // coef with unsigned int type

    unsigned long long int result1 = (goal * reward) * coef; // (goal * reward) will exceed the max 'unsigned int'
    unsigned long long int result2 = goal * (reward * coef); // this will convert reward (unsigned int) -> reward (unsigned long long int)

    std::cout << "result1: " << result1 << std::endl;
    std::cout << "result2: " << result2 << std::endl;

    return 0;
}