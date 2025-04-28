#include <iostream>
#include <mutex>
#include <thread>
#include <string>

// Global mutex object
std::mutex task_mutex;

void task(const std::string&& str) {
    for (int i = 0; i < 5; ++i) {

        task_mutex.lock();

        // Enter the critical section which only one thread can be allowed
        // cout is a global variable, and we inserting it using << symbol
        std::cout << str[0] << str[1] << str[2] << std::endl;

        task_mutex.unlock();
    }
}

int main(int argc, char* argv[]) {

    std::thread thr1(task, std::move("abc"));
    std::thread thr2(task, std::move("def"));
    std::thread thr3(task, std::move("xyz"));
    
    thr1.join();
    thr2.join();
    thr3.join();
    
    return 0;
}