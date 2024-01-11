#include <iostream>
#include <utility>

class SafeArray {
private:
    static constexpr int MAXSIZE = 5;
    int* arr[MAXSIZE];

public:

    // empty constructor
    SafeArray() {
        for (int i = 0; i < MAXSIZE; i++) {
            arr[i] = nullptr;
        }
    }

    // overloading constructor
    SafeArray(const int* initialValues) {
        for (int i = 0; i < MAXSIZE; ++i) {
            try {
                arr[i] = new int(initialValues[i]);  // Allocate memory with exception handling
            } catch (const std::bad_alloc& e) {
                // Handle allocation failure, e.g., log error and clean up allocated memory
                std::cerr << "Memory allocation failed: " << e.what() << std::endl;
                for (int j = 0; j < i; ++j) {
                    delete arr[j];
                }
                throw;  // Rethrow the exception to propagate the error
            }
        }
    }

    // Copy constructor (deep copy)
    SafeArray(const SafeArray& other) {
        for (int i = 0; i < MAXSIZE; ++i) {
            try {
                arr[i] = new int(*other.arr[i]);  // Allocate memory and copy values
            } catch (const std::bad_alloc& e) {
                // Handle allocation failure as in parameterized constructor
                std::cerr << "Memory allocation failed: " << e.what() << std::endl;
                for (int j = 0; j < i; ++j) {
                    delete arr[j];
                }
                throw;
            }
        }
    }

    // Move constructor (transfer ownership)
    SafeArray(SafeArray&& other) noexcept {
        for (int i = 0; i < MAXSIZE; ++i) {
            arr[i] = std::exchange(other.arr[i], nullptr);  // Transfer ownership
        }
    }

    // Copy assignment operator
    SafeArray& operator=(const SafeArray& other) {
        if (this != &other) {  // Self-assignment check
            for (int i = 0; i < MAXSIZE; ++i) {
                delete[] arr[i];  // Deallocate existing memory
                try {
                    arr[i] = new int(*other.arr[i]);  // Allocate new memory and copy values
                } catch (const std::bad_alloc& e) {
                    // Handle allocation failure as in parameterized constructor
                }
            }
        }
        return *this;
    }

    SafeArray& operator=(SafeArray&& other) {
        if(this != &other) {
            for (int i = 0; i < MAXSIZE; i++) {
                delete[] this->arr[i];
                this->arr[i] = std::exchange(other.arr[i], nullptr);
            }
        }
        return *this;
    }

    int& operator[](const int index) {
        if (index < 0 || index >= MAXSIZE) {
            std::cout << "Index out of range" << std::endl;
            exit(1);
        }
        return *this->arr[index];
    }

    void displayElements() const {
        for (int i = 0; i < MAXSIZE; i++) {
            std::cout << *this->arr[i] << " ";
        }
        std::cout << "\n";
    }

    ~SafeArray() {
        std::cout << "Destructor Called" << std::endl;
        for (int i = 0; i < MAXSIZE; ++i) {
            delete[] this->arr[i];  // Deallocate memory for each element
            this->arr[i] = nullptr;
        }
    }
};

int main(int argc, char* argv[]) {

    int arr[] = {1, 2, 3, 4, 5};
    SafeArray sa{arr};
    sa.displayElements();
    std::cout << sa[2] << std::endl;

    int arr1[] = {5, 4, 3, 2, 1};
    SafeArray sa1 = SafeArray{arr1};
    sa1.displayElements();

    return 0;
}