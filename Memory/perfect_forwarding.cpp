#include <iostream>
#include <vector>
#include <utility>

/*
std::forward is designed for use in template functions that forward arguments to another
function, while preserving their original value category. It allows functions to forward their
arguments with zero overhead in terms of copies, making it an essential tool for writing
efficient and flexible code.    
*/

template <typename T>
void push_back_value(std::vector<T>& v, T&& value) {
    v.push_back(std::forward<T>(value));
}

int main(int argc, char* argv[]) {

    std::vector<int> vec;
    push_back_value(vec, 20);
    
    return 0;
}