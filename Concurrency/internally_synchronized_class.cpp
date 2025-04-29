#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <chrono>

using namespace std::literals;

template <typename T>
class Vector {
private:
    std::mutex mut;
    std::vector<T> vec;

public:
    void push_back(const T& e) {
        mut.lock();

        std::cout << "push by thread_id: " << std::this_thread::get_id() << " ";
        vec.push_back(e);
        std::cout << "\n";
        
        mut.unlock();
    }

    void print() {
        mut.lock();

        std::cout << "print by thread_id: " << std::this_thread::get_id() << " {\n";
        for (auto e : vec) {
            std::cout << e << " ";
        }
        std::cout << "}\n";

        mut.unlock();
    }
};

void func(Vector<int>& vec) {
    for (int i = 0; i <= 5; ++i) {
        vec.push_back(i);
        std::this_thread::sleep_for(50ms);
        vec.print();
        std::this_thread::sleep_for(50ms);
    }
}

int main(int argc, char* argv[]) {

    // vec shared across three threads
    Vector<int> vec;
    std::thread thr1(func, std::ref(vec));
    std::thread thr2(func, std::ref(vec));
    std::thread thr3(func, std::ref(vec));

    thr1.join(); thr2.join(); thr3.join();

    return 0;
}