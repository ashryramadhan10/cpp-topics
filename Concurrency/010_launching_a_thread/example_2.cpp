#include <iostream>
#include <thread>

class background_task_with_arguments {
public:
    // this function force the argument input as rvalue
    void operator () (std::string&& str) {
        std::cout << "The ownership of this \"" << str << "\" has been transfered to this thread" << std::endl;
    }
};

int main(int argc, char* argv[]) {

    background_task_with_arguments f;
    std::string str = "test string";

    std::thread thr(f, std::move(str));
    thr.join();

    std::cout << "Do I still have the data?" << std::endl;
    std::cout << (str.empty() ? "NO" : "YES") << "." << std::endl;

    return 0;
}