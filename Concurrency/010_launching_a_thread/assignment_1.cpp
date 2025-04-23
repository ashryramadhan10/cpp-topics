#include <iostream>
#include <thread>

class fizzbuzz_on_background {
public:
    void operator () (int n) {
        if (n % 3 == 0 && n % 5 == 0) {
            std::cout << "FizzBuzz" << std::endl;
        } else if (n % 3 == 0) {
            std::cout << "Fizz" << std::endl;
        } else if (n % 5 == 0) {
            std::cout << "Buzz" << std::endl;
        }
    }
};

int main(int argc, char* argv[]) {

    fizzbuzz_on_background f;
    std::thread thr(f, 15);

    thr.join();

    return 0;
}