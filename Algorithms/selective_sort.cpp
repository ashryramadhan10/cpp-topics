#include <iostream>

void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

int main(int argc, char* argv[]) {

    int x = 10;
    int arr[] = {1, 2, 3, 10, 5};
    int arrSize = sizeof(arr) / sizeof(arr[0]);

    for (int i = 0; i < arrSize; i++) {
        int s = i;
        for (int j = i+1; j < arrSize; j++) {
            if (arr[s] > arr[j]) {
                swap(arr[s], arr[j]);
            }
        }
    }

    for (int i = 0; i < arrSize; i++) {
        printf("%d", arr[i]);
    }

    return 0;
}