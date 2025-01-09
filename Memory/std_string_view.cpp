#include <iostream>
#include <string_view>

void printSubstring(std::string_view sv) {
    std::cout << sv << std::endl;
}

int main(int argc, char* argv[]) {
    
    std::string str = "Hello, world!";
    std::string_view sv = str.substr(7, 5);

    printSubstring(sv);
    return 0;
}