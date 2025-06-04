#include <iostream>
#include <stdint.h>

template <class T>
class SafeArray {
    private:
        constexpr static uint32_t ARRAY_MAX_SIZE = 100;

        T* array;
        uint32_t arraySize;

    public:

        SafeArray();
        SafeArray(T* arr, uint32_t&& arraySize);
        SafeArray(const SafeArray& safeArray);
        SafeArray(SafeArray&& safeArray) noexcept;

        ~SafeArray();

        void printArray() const;
        T& operator[](uint32_t&& index);
        SafeArray& operator=(const SafeArray& safeArray);
        SafeArray& operator=(SafeArray&& safeArray);
};

template <class T>
SafeArray<T>::SafeArray(): array{nullptr}, arraySize{0} {
    std::cout << "Empty Constructor Called!" << std::endl;
}

template <class T>
SafeArray<T>::SafeArray(T* arr, uint32_t&& arrSize) {
    if (arr == nullptr) {
        exit(1);
    } else {
        if (arrSize <= SafeArray<T>::ARRAY_MAX_SIZE) {
            this->array = new T[arrSize];
            memcpy(this->array, arr, sizeof(*(this->array + 0)) * arrSize);
            
            this->arraySize = arrSize;
        } else {
            exit(1);
        }
    }
}

template <class T>
SafeArray<T>::SafeArray(const SafeArray& safeArray): SafeArray{safeArray.array, safeArray.arraySize} {
    std::cout << "Copy Constructor Called!" << std::endl;
}

template <class T>
SafeArray<T>::SafeArray(SafeArray&& safeArray) noexcept : array{safeArray.array}, arraySize{safeArray.arraySize} {
    safeArray.array = nullptr;
    std::cout << "Move Constructor Called!" << std::endl;
}

template <class T>
SafeArray<T>::~SafeArray() {
    std::cout << "Destructor Called!" << std::endl;
    delete [] this->array;
}

template <class T>
void SafeArray<T>::printArray() const {
    for (int i = 0; i < this->arraySize; i++) {
        std::cout << *(this->array + i) << " ";
    }
    std::cout << "\n";
}

template <class T>
T& SafeArray<T>::operator[](uint32_t&& index) {
    if (!(index < 0) || !(index >= SafeArray<T>::ARRAY_MAX_SIZE))
        return this->array[index];
}

template <class T>
SafeArray<T>& SafeArray<T>::operator=(const SafeArray& safeArray) {
    if(this == &safeArray) {
        return *this;
    } else {
        delete [] this->array;
        this->array = new T[safeArray.arraySize];
        memcpy(this->array, safeArray.array, sizeof(*(this->array + 0)) * safeArray.arraySize);
    }
}

template <class T>
SafeArray<T>& SafeArray<T>::operator=(SafeArray&& safeArray) {
	if(this == &safeArray) {
		return *this;
	} else {
		delete [] this->array;
		this->array = safeArray.array;
		safeArray.array = nullptr;

		this->arraySize = safeArray.arraySize;
	}
}

int main(int argc, char* argv[]) {
	SafeArray<int> sa1(new int[3]{1, 2, 3}, 3);
	sa1.printArray();

    	return 0;
}