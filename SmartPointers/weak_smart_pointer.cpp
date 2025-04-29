#include <iostream>
#include <memory>

class B; // Forward declaration

class A {
public:
    std::shared_ptr<B> b_ptr;
    ~A() { 
        std::cout << "A destroyed" << std::endl; 
    }
};

class B {
public:
    std::weak_ptr<A> a_ptr;
    ~B() { 
        std::cout << "B destroyed" << std::endl; 
    }
};

int main() {
    std::shared_ptr<A> a = std::make_shared<A>();
    std::shared_ptr<B> b = std::make_shared<B>();

    std::cout << "a counting: " << a.use_count() << std::endl;
    std::cout << "b counting: " << b.use_count() << std::endl;

    // Heap Memory (FIFO), Stack Memory (LIFO)
    a->b_ptr = b;
    b->a_ptr = a;

    std::cout << "a counting: " << a.use_count() << std::endl;
    std::cout << "b counting: " << b.use_count() << std::endl;
}