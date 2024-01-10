#include <iostream>
#include <utility>

class Shape {
private:
    int* data;

public:
    Shape(): data{new int(0)} {}
    Shape(int _data): data{new int(_data)} {}
    Shape(const Shape& other): Shape(*other.data) {}
    Shape(Shape&& other) noexcept : data{std::exchange(other.data, nullptr)} {}

    virtual ~Shape() {
        delete this->data;
        std::cout << "Shape Destructor" << std::endl;
    }

    int getData() const {
        return *this->data;
    }

    virtual void draw() const = 0;
};

class Triangle : public Shape {
private:
    int* side;

public:

    Triangle(): Shape(), side{new int(0)} {}
    Triangle(int data, int _side): Shape{data}, side{new int(_side)} {}
    Triangle(const Triangle& other): Shape{other}, side{new int(*other.side)} {}
    Triangle(Triangle&& other) noexcept : Shape(std::move(other)), side{std::exchange(other.side, nullptr)} {}

    virtual ~Triangle() {
        delete this->side;
        std::cout << "Triangle Destructor" << std::endl;
    }

    virtual void draw() const override {
        std::cout << "Draw a Triangle: (" << this->getData() << ", " << *this->side << ")" << std::endl;
    }
};

int main(int argc, char* argv[]) {

    int data = 1, side = 2;
    Shape* shape = new Triangle(data, side);
    shape->draw();

    delete shape;

    return 0;
}