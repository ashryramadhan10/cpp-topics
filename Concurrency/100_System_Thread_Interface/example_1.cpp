#include <iostream>
#include <thread>

class DummyClass {
public:
    void operator () () {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Hello from DummyClass running from thread id:" << std::this_thread::get_id() << std::endl;
    }
};

int main(int argc, char* argv[]) {
    std::cout << "Hello from main thread: " << std::this_thread::get_id() << std::endl;

    DummyClass f;
    std::thread thr(f);

    std::cout << "thr thread id: " << thr.get_id() << std::endl;

    std::cout << "Native handle: " << thr.native_handle() << "\n";

    thr.join();

    std::cout << "Native handle after join: " << thr.native_handle() << "\n";

    std::cout << "thr thread id: " << thr.get_id() << std::endl;

    return 0;
}