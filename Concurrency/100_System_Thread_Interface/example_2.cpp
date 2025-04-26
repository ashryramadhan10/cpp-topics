#include <iostream>
#include <thread>

void hello() {
    try {
        // throw an exception
        throw std::exception();
    }
    catch (std::exception& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
}

int main(int argc, char* argv[]) {

    std::thread thr(hello);
    thr.join();

    std::cout << "Finished!" << std::endl;

    return 0;
}