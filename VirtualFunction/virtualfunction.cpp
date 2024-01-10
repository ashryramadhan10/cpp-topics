#include <iostream>

// This script will introduce you:
// 1. Inheritance
// 2. Virtual Function
// 3. Virtual Destructor
// 4. Abstract Method
// 5. Polymorphism

// In theory, when we call the constructor of subclass
// we actually call the superclass constructor
// then, both need to be destroyed

class Shape {
public:
    Shape() {}
    virtual ~Shape() {
        std::cout << "Shape Desctructor Called: " << this << std::endl;
    }

    // abstract method
    virtual void draw() const = 0;
};

class Triangle : public Shape {
public:
    int side;

    Triangle(): Shape(), side{0} {}
    ~Triangle() {
        std::cout << "Triange Desctructor Called: " << this << std::endl;
    }

    virtual void draw() const override {
        std::cout << "Draw a Triangle" << std::endl;
    }
};

int main(int argc, char* argv[]) {

    // Polymorphism
    Shape* shape = new Triangle();
    shape->draw();

    delete shape;

    return 0;
}