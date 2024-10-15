#include <iostream>

struct S {
    char a; // memory location #1
    int b : 5; // memory location #2
    int c : 11, // memory location #2 (continued)
    : 0,
    d : 8; // memory location #3
    int e; // memory location #4
    double f; // memory location #5
    std::string g; // several memory locations
};

int main(int argc, char* argv[]) {

    
    return 0;
}