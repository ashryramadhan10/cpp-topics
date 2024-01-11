#include <iostream>

class CartesianCoordinate {
private:
    int x;
    int y;

public:
    CartesianCoordinate(): x{0}, y{0} {}
    CartesianCoordinate(int _x, int _y): x{_x}, y{_y} {}

    int getX() const {
        return this->x;
    }

    int getY() const {
        return this->y;
    }

    void getInfo() const {
        std::cout << "(" << this->x << ", " << this->y << ")" << std::endl;
    }

    // prefix ascending
    void operator++() {
        ++this->x;
        ++this->y;
    }

    // postfix ascending
    void operator++(int) {
        this->x++;
        this->y++;
    }

    void operator+=(int i) {
        this->x += i;
        this->y += i;
    }

    void operator-=(int i) {
        this->x -= i;
        this->y -= i;
    }

    bool operator > (const CartesianCoordinate& other) {
        if (this->x > other.x && this->y > other.y) {
            return true;
        } else {
            return false;
        }
    }

    bool operator == (const CartesianCoordinate& other) {
        if (this->x == other.x && this->y == other.y) {
            return true;
        } else {
            return false;
        }
    }

    friend std::ostream& operator << (std::ostream& os, const CartesianCoordinate& cartesian);
};

std::ostream& operator << (std::ostream& os, const CartesianCoordinate& cartesian) {
    os << "(" << cartesian.getX() << ", " << cartesian.getY() << ")";
    return os;
}

int main(int argc, char* argv[]) {

    CartesianCoordinate coord {8, 8};
    coord += 2;
    coord++;
    coord.getInfo();

    CartesianCoordinate coord2 {8, 8};

    if (coord2 > coord) {
        std::cout << "True" << std::endl;
    } else {
        std::cout << "False" << std::endl;
    }

    std::cout << coord2 << std::endl;

    return 0;
}