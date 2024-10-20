#include <iostream>
#include <stdint.h>

void swap(uint16_t& a, uint16_t& b) {
    uint16_t temp = a;
    a = b;
    b = temp;
}

void insertion_sort1(uint16_t* arr, const uint16_t arrSize) {
    // [1] 23, 1, 10, 5, 2 -> 1, 23, 10, 5, 2
    // [2] 1, 23, 10, 5, 2 -> 1, 10, 23, 5, 2
    // [3] 1, 10, 23, 5, 2 -> 1, 5, 10, 23, 2
    // [4] 1, 5, 10, 23, 2 -> 1, 2, 5, 10, 23

    for (int i = 1; i < arrSize; i++) {
        
        int key = arr[i];
        int j = i - 1;

        while(j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;

        for (uint16_t k = 0; k < arrSize; k++) {
            printf("%d ", arr[k]);
        }
        printf("\n");
    }
}

void insertion_sort2(uint16_t* arr, uint16_t arrSize) {
    for (int i = 1; i < arrSize; i++) {
        int j = i - 1;

        while(j >= 0 && arr[j] > arr[j + 1]) {
            swap(arr[j + 1], arr[j]);
            j--;
        }

        for (uint16_t k = 0; k < arrSize; k++) {
            printf("%d ", arr[k]);
        }
        printf("\n");
    }
}

int main(int argc, char* argv[]) {

    uint16_t arr[] = {23, 1, 10, 5, 2};
    const uint16_t arrSize = sizeof(arr) / sizeof(arr[0]);

    for (uint16_t i = 0; i < arrSize; i++) {
            printf("%d ", arr[i]);
    }
    printf("\n");

    insertion_sort2(arr, arrSize);

    return 0;
}