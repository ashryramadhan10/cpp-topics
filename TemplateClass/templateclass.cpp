#include <iostream>
#include <vector>

template <class T>
class SafeArray {
private:

    constexpr static uint32_t MAX_SIZE = 10;
    
    T* arr;
    uint32_t arrSize;

public:
    
    // constructors
    SafeArray();
    SafeArray(T* arr, uint32_t&& arrSize); // overloading constructor
    SafeArray(const SafeArray& safeArray); // copy constructor
    SafeArray(SafeArray&& safeArray); // move constructor

    // destructors
    ~SafeArray();

    // methods
    void printArray() const;
    T& operator[](const uint32_t index);
    SafeArray& operator=(const SafeArray& safeArray); // return SafeArray& that means we don't need to hold this object into another object variable
    
    operator int() const {
        int sum = 0;
        for (int i = 0; i < this->arrSize; i++) {
            sum += *(this->arr + i);
        }
        return sum;
    }


};

// empty constructor
template <class T>
SafeArray<T>::SafeArray(): arr{nullptr}, arrSize{0} {
    std::cout << "constructor called" << std::endl;
}

// overloading constructor
template <class T>
SafeArray<T>::SafeArray(T* _arr, uint32_t&& _arrSize): arr{nullptr}, arrSize{0} {
    if (_arr == nullptr) {
        this->arr = new T[1];
        this->arrSize = 1;
    } else {
        if (_arrSize < MAX_SIZE) {
            this->arr = new T[_arrSize];
            memcpy(this->arr, _arr, sizeof(*(_arr + 0)) * _arrSize);

            this->arrSize = _arrSize;
        } else {
            std::cout << "Array size exceeds the maximum size allowed" << std::endl;
        }
    }
}

// copy constructor
template <class T>
SafeArray<T>::SafeArray(const SafeArray& safeArray): SafeArray{safeArray.arr, uint32_t(safeArray.arrSize)} {
    std::cout << "copy constructor called" << std::endl;
}

// move constructor
template <class T>
SafeArray<T>::SafeArray(SafeArray&& safeArray): arr{safeArray.arr}, arrSize{safeArray.arrSize} {
    safeArray.arr = nullptr;
    std::cout << "move constructor called" << std::endl;
}

// destructor
template <class T>
SafeArray<T>::~SafeArray() {
    std::cout << "destructor called" << std::endl;
    delete [] this->arr;
}

template <class T>
void SafeArray<T>::printArray() const {
    for (int i = 0; i < this->arrSize; i++) {
        std::cout << *(this->arr + i) << " ";
    }
    std::cout << "\n";
}

template <class T>
T& SafeArray<T>::operator[](const uint32_t index) {
    if (!(index < 0) || !(index >= MAX_SIZE))
        return this->arr[index];
}

template <class T>
SafeArray<T>& SafeArray<T>::operator=(const SafeArray& safeArray) {
    std::cout << "assignment operator overloading called!" << std::endl;
    if (this == &safeArray) {
        return *this;
    } else {
        delete [] this->arr;
        this->arr = new T[safeArray.arrSize];
        memcpy(this->arr, safeArray.arr, sizeof(*(safeArray.arr + 0)) * safeArray.arrSize);
        this->arrSize = safeArray.arrSize;

        return *this;
    }
}

int main(int argc, char* argv[]) {

    // empty constructor
    SafeArray<int> sa1(new int[3]{1, 2, 3}, 3);
    SafeArray<int> sa2 = sa1;

    int sum1 = sa1;
    std::cout << sum1 << std::endl;

    int sum2 = sa2;
    std::cout << sum2 << std::endl;

    return 0;
}