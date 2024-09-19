#include <iostream>

int main(int argc, char* argv[]) {

    int x = 0;
    int *p = new int;
    *p = 20;

    int &refInt1 = x;
    int &refInt2 = *p;

    std::cout << x << " " << *p << std::endl;
    std::cout << refInt1 << " " << refInt2 << std::endl;

    x++;
    (*p)++;

    std::cout << x << " " << *p << std::endl;
    std::cout << refInt1 << " " << refInt2 << std::endl;

    delete p;

    return 0;
}