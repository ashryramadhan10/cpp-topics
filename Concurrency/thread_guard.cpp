#include <iostream>
#include <thread>

class ThreadGuard {
private:
    std::thread& t;

public:
    explicit ThreadGuard(std::thread& t_): t{t_} {}

    ~ThreadGuard() {
        if (this->t.joinable()) {
            this->t.join();
        }
    }

    ThreadGuard(const ThreadGuard&) = delete;
    ThreadGuard& operator=(const ThreadGuard&) = delete;
};

struct func
 {
    int& i;
    func(int& i_):i(i_){}
    
    void operator()()
    {
        std::cout << "Hello World : " << this->i << std::endl;
    }
 };

void do_something_in_current_thread(bool is_throw) {
    // option to throw
    if (is_throw)
        throw std::exception();

    std::cout << "Do something in current thread!" << std::endl;
}

void f() {
    int some_local_state=0;
    func my_func(some_local_state);
    std::thread t(my_func);
    ThreadGuard g(std::ref(t));

    do_something_in_current_thread(true);
}

int main(int argc, char* argv[]) {
    // even the do_something_in_current_thread() throw an exception
    // the thread is safe because when the ThreadGuard class guard destroyed
    // the thread will be joined as well, remember that our thread was passed
    // by reference not value
    f();
    return 0;
}