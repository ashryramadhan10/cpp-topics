#include <iostream>
#include "Counter.h"
#include "SafeArray.h"
#include "Point.h"

std::ostream& operator << (std::ostream& os, const Counter& counter) {
    // e.g: os << "MyClass(value1 = " << obj.value1 << ", value2 = " << obj.value2 << ")";
    os << counter.getCounter();
    return os;
}

std::ostream& operator << (std::ostream& os, SafeArray& safeArray) {
    char result[ARRAYSIZE * 2 + 5];
    int charCounter = 0;
    int start = 0;

    for (int i = 0; i < ARRAYSIZE; i++) {
        int num = safeArray[i];
        int temp = num;
        int len = 0;

        while (temp > 0) {
            len++;
            temp /= 10;
        }

        for (int j = len - 1; j >= 0; j--) {
            result[start + j] = num % 10 + '0';
            num /= 10;
        }

        start += len;
        result[start] = ' ';
        start += 1;
    }

    result[start] = '\0';

    os << "[SAFE ARRAY]: " << result;

    return os;
}

int main(int argc, char* argv[]) {

    SafeArray sa;
    for (int i = 0; i < ARRAYSIZE; i++) {
        sa[i] = i + 1;
    }
    std::cout << sa << std::endl;

    // ---

    Counter ctr{5};
    std::cout << "Counter: " << ++ctr << std::endl;

    // ---

    Point* point = createPoint(10, 10);
    std::cout << "point: (" << point->x << " , " << point->y << ")" << std::endl;
    deletePointObject(&point);

    return 0;
}