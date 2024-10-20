#include <iostream>

void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

void buble_sort(int* arr, int sizeArr) {

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

int binary_search(int* arr, int x, int l, int r) {

    if (arr[r] == x) {
        return r;
    }

    if (arr[l] == x) {
        return l;
    }
    
    int m = (l + r) / 2;

    if (arr[m] == x)
        return m;

    if (x < arr[m]) {
        return binary_search(arr, x, l, m - 1);
    } else {
        return binary_search(arr, x, m + 1, r);
    }

}

int main(int argc, char* argv[]) {

    int arr[] = {1, 2, 7, 10, 55, 100, 5, 500, 32, 72, 95, 22};
    int n = sizeof(arr) / sizeof(arr[0]);
    int l = 0;
    int r = n-1;
    int x = 72;

    buble_sort(arr, n);

    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    int index_x = binary_search(arr, x, l, r);
    printf("%d", index_x);

    return 0;
}