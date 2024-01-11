#include <iostream>
#include <memory>
#include <vector>

int main(int argc, char* argv[]) {

    // std::shared_ptr<int> ptr1(new int(5));
    // std::shared_ptr<int> ptr2 {ptr1};

    // std::cout << ptr1.use_count() << std::endl; // 2

    // ptr1.reset(); // delete this pointer from shared variable

    // std::cout << *ptr2 << std::endl; // 5
    // std::cout << ptr1.use_count() << std::endl; // 0
    // std::cout << ptr2.use_count() << std::endl; // 1

    // example with vector
    std::shared_ptr<int> var1 = std::make_shared<int>(1);
    std::shared_ptr<int> var2 = std::make_shared<int>(2);
    std::shared_ptr<int> var3 = std::make_shared<int>(3);

    std::vector<std::shared_ptr<int>> integers;
    integers.push_back(var1);
    integers.push_back(var2);
    integers.push_back(var3);

    for (const auto& e : integers) {
        std::cout << *e << std::endl;
        std::cout << "Use Pointer: " << e.use_count() << std::endl;
    }

    return 0;
}