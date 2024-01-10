#define ARRAYSIZE 10

class SafeArray {
private:
    int arr[ARRAYSIZE];

public:
    SafeArray() {
        for (int i = 0; i < ARRAYSIZE; i++) {
            this->arr[i] = 0;
        }
    }

    ~SafeArray() {}

    // 1. return void
    void info() const {
        for (int i = 0; i < ARRAYSIZE; i++) {
            std::cout << this->arr[i] << std::endl;
        }
    }

    // 2. return data type
    int at(const int index) {
        if (index < 0 || index > ARRAYSIZE)
            return -1;
        return this->arr[index];
    }

    // 3. return reference data type
    int& operator [] (int index) {
        if (index < 0 || index > ARRAYSIZE) {
            std::cout << "Array index out of bound!" << std::endl;
            exit(1);
        }
        return this->arr[index];
    }
};
