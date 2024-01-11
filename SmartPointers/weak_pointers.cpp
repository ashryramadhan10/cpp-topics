#include <iostream>
#include <memory>

class B;

class A {
    private:
        std::shared_ptr<B> b_ptr;

    public:

        void set_B(std::shared_ptr<B>& b) {
            b_ptr = b;
        }

        A() { std::cout << "A Constructor" << std::endl; }
        ~A() { std::cout << "A Destructor" << std::endl; }
};

class B {
    private:
        // make this into weak to break the strong circular reference
        // std::shared_ptr<A> a_ptr;
        std::weak_ptr<A> a_ptr;

    public:

        void set_A(std::shared_ptr<A>& a) {
            a_ptr = a;
        }

        B() { std::cout << "B Constructor" << std::endl; }
        ~B() { std::cout << "B Destructor" << std::endl; }
};

int main(int argc, char* argv[]) {

    std::shared_ptr<A> a = std::make_shared<A>();
    std::shared_ptr<B> b = std::make_shared<B>();

    // this can cause the instance variable of class A and B never get destroyed, because if use_count of the shared pointer > 0
    // then the allocated memory on the heap will never get destroyed

    // a has b_ptr -> B
    // b has a_ptr -> A

    // first the use_count must be: 2 for each class
    // then, if a and b get deleted, use_count will be: 1
    // then we never get access to these pointer to delete them

    a->set_B(b);
    b->set_A(a);

    return 0;
}