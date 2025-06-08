#include <iostream>

int binarySearch(int* arr, int x, int n, int left, int right) {
    int mid = (left + right)/ 2;
    
    if (arr[mid] == x) {
        return mid;
    }

    if (x < arr[mid]) {
        binarySearch(arr, x, n, left, mid - 1);
    } else {
        binarySearch(arr, x, n, mid + 1, right);
    }

    return -1;
}

int main(int argc, char* argv[]) {

    return 0;
}