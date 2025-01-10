#include <iostream>
#include <memory>

int main(int argc, char* argv[]) {
    // Create a unique pointer to an array of 5 integers
    std::unique_ptr<int[]> array = std::make_unique<int[]>(5);

    // Initialize the array
    for (int i = 0; i < 5; ++i) {
        array[i] = i + 1;
    }

    // Print the array values correctly
    for (int i = 0; i < 5; ++i) {
        // Dereference the raw pointer obtained by array.get() and use pointer arithmetic
        std::cout << "array[" << i << "]: " << *(array.get() + i) << std::endl;
    }

    return 0;
}