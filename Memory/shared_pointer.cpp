#include <iostream>
#include <memory>

int main(int argc, char* argv[]) {

    std::shared_ptr<int> ptr1 = std::make_shared<int>(10);
    std::shared_ptr<int> ptr2 = ptr1;

    std::cout << ptr1.use_count() << std::endl;
    return 0;
}