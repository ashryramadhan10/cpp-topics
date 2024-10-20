#include <iostream>

int linear_search(int* arr, int n, int x) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == x) {
            return i;
        }
    }
    return -1;
}

int main(int argc, char* argv[]) {

    int arr[] = {1, 2, 3, 4, 5};
    int n = 5;
    int x = 3;

    int index = linear_search(arr, n, x);
    if (index != -1)
        printf("index: %d, arr[%d]: %d", index, index, arr[index]);

    return 0;
}