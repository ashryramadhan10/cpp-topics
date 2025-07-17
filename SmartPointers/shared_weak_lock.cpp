//
// Created by ashry on 7/17/2025.
//
#include <iostream>
#include <memory>

class MyClass {
public:
    void greet() {
        std::cout << "Hello from MyClass!" << std::endl;
    }
};

int main() {
    std::shared_ptr<MyClass> sp = std::make_shared<MyClass>();
    std::weak_ptr<MyClass> wp = sp;  // weak_ptr observing shared_ptr

    if (auto locked = wp.lock()) {  // try to lock weak_ptr
        locked->greet();  // safe to use
    } else {
        std::cout << "Object no longer exists." << std::endl;
    }

    sp.reset();  // release ownership

    if (auto locked = wp.lock()) {
        locked->greet();
    } else {
        std::cout << "Object no longer exists." << std::endl;
    }

    return 0;
}

