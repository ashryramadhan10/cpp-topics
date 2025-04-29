#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex task_mutex;

void print(const std::string&& str) {
    for(int i=0; i<5; i++) {
        try {
            // task_mutex.lock();

            // better call lock_guard
            // only this line your program will be much better
            std::lock_guard<std::mutex> lck_guard(task_mutex);

            // cout is actually a global variable, << symbol is used for inserting character to cout
            // this will make the output of the stream in terminal will be scrambled
            std::cout << str[0] << str[1] << str[2] << std::endl;
            
            // this cause the flow jump to catch below
            // catch doesn't have thread join()
            // this will make the program blocked
            // unlock never called
            // mutex remains locked
            throw std::exception();

            // task_mutex.unlock();

            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        catch (std::exception& e) {
            std::cout << "Exception: " << e.what() << std::endl;
        }
    }
}

int main(int argc, char* argv[]) {
    std::thread thr1(print, std::move("abc"));
    std::thread thr2(print, std::move("def"));
    std::thread thr3(print, std::move("xyz"));

    thr1.join();
    thr2.join();
    thr3.join();
    
    return 0;
}