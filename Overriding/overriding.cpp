#include <iostream>

class Animal {
private:
    int age;

public:
    Animal(): age{0} {}
    virtual ~Animal() { std::cout << "Animal Destroyed" << std::endl;}

    virtual void showAge() const {
        std::cout << "Age: " << this->age << std::endl;
    }

    int getAge() const {
        return this->age;
    }
};

class Cat : public Animal {
private:
    int life;

public:
    Cat(): Animal(), life{9} {}
    ~Cat() { std::cout << "Cat Destroyed" << std::endl; }

    void showAge() const override {
        std::cout << "Age: " << this->getAge() << ", Life: " << this->life << std::endl;
    }
};

int main(int argc, char* argv[]) {

    Cat cat1;
    cat1.showAge();

    Animal* cat2 = new Cat();
    cat2->showAge();
    delete cat2;

    return 0;
}