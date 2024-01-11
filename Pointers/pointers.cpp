#include <iostream>

int main(int argc, char* argv[]) {

    int* arrOfPointers[5];
    int* pointerOfArrays = new int[5];
    int** doublePointer = new int*[5];

    for (int i = 0; i < 5; i++) {
        arrOfPointers[i] = new int(i);
    }

    for (int i = 0; i < 5; i++) {
        doublePointer[i] = new int[3];

        for (int j = 0; j < 3; j++) {
            doublePointer[i][j] = i * 3 + j;
        }
    }

    for (int i = 0; i < 5; i++) {
        delete[] doublePointer[i];
    }
    delete[] doublePointer;

    for (int i = 0; i < 5; i++) {
        delete arrOfPointers[i];
    }
    delete[] pointerOfArrays;

    return 0;
}