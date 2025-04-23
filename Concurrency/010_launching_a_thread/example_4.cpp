#include <iostream>
#include <thread>

class Greet {
public:
    void hello() {
        std::cout << "Hello from hello() function in Greet class" << std::endl;
    }
};

int main(int argc, char* argv[]) {

    Greet greet;

    std::thread thr(&Greet::hello, &greet);

    thr.join();

    return 0;
}