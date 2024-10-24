#include <iostream>
#include <atomic>
#include <thread>

class Spinlock {
private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;

public:
    // true condition
    void lock() {
        while (flag.test_and_set());
    }

    // false condition
    void unlock() {
        flag.clear();
    }
};

Spinlock spin;

void workOnResource() {
    spin.lock();
    // fast and short activity
    printf("Hello World!\n");
    spin.unlock();
}

int main(int argc, char* argv[]) {

    std::thread t(workOnResource);
    std::thread t2(workOnResource);

    t.join();
    t2.join();

    return 0;
}