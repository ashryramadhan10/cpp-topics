#include <iostream>
#include <thread>

int main(int argc, char* argv[]) {

    int i = 3;
    std::thread thr ([&i](int coef){
        i *= coef;
    }, 3);
    thr.join();

    std::cout << "Value of i now: " << i << std::endl;

    return 0;
}