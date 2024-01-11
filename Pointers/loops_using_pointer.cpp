#include <iostream>

int main(int argc, char* argv[]) {

    int rows = 3;
    int cols = 3;

    int arr[rows][cols] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    for (int* p = &arr[0][0]; p != &arr[0][0] + cols * rows; p++) {
        std::cout << *p << " ";
    }

    return 0;
}