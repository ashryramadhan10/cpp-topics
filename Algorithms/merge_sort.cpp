#include <iostream>
#include <vector>

using namespace std;

void merge(vector<int>& arr, int left, int mid, int right) {

    int n1 = (mid - left) + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];

    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0;
    int k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
    }
}

void merge_sort(vector<int>& arr, int left, int right) {

    for (int i = left; i <= right; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    if (left >= right){
        printf("---\n");
        return;
    }

    int mid = left + (right - left) / 2;
    merge_sort(arr, left, mid);
    merge_sort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

// Function to test the merge function
void testMerge() {
    // Original array with two sorted subarrays
    vector<int> arr = {1, 3, 5, 0, 2, 4}; // left: {1, 3, 5}, right: {0, 2, 4}
    
    // Define the indices for merging
    int leftStart = 0;
    int rightEnd = 5; // Last index of the second sorted subarray

    // Call merge function
    merge_sort(arr, leftStart, rightEnd);

    // Output the result
    cout << "Merged array: ";
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;
}

int main(int argc, char* argv[]) {

    testMerge();
    return 0;
}