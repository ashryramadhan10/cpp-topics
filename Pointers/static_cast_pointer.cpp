#include <iostream>
#include <memory>

int main(int argc, char* argv[]) {
    
    int* p = static_cast<int*>(::operator new(5 * sizeof(int)));

    p[0] = 1;
    p[1] = 2;

    printf("p: %d\n", p);
    printf("p + 0: %d\n", *(p + 0));
    printf("p + 1: %d\n", *(p + 1));

    delete[] p;

    return 0;
}