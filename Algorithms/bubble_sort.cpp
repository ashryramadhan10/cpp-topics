#include <iostream>
#include <stdint.h>

void swap(uint16_t& a, uint16_t& b) {
    uint16_t temp = a;
    a = b;
    b = temp;
}

void buble_sort(uint16_t* arr, uint16_t sizeArr) {

    for (int i=0; i<sizeArr; i++) {
        for (int j=0; j<sizeArr-i-1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }

        for(int k=0; k<sizeArr; k++) {
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

    buble_sort(arr, arrSize);

    return 0;
}