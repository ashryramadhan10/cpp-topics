#include <iostream>
#include <thread>

void print(std::string str) {
    for(int i=0; i<5; i++) {
        // cout is actually a global variable, << symbol is used for inserting character to cout
        // this will make the output of the stream in terminal will be scrambled
        std::cout << str[0] << str[1] << str[2] << std::endl;
    }
}

int main(int argc, char* argv[]) {
    std::thread thr1(print, "abc");
    std::thread thr2(print, "def");
    std::thread thr3(print, "xyz");

    thr1.join();
    thr2.join();
    thr3.join();
    
    return 0;
}