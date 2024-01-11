#include <iostream>

int main(int argc, char* argv[]) {

    // 1. char variable by default
    char character = 'a';
    std::cout << character << std::endl;

    // 2. char as string or 1D array to a string
    char str[] = {"Hello"};
    str[0] = 'E';
    std::cout << str << std::endl;

    // or

    char str1[] = {'H', 'e', 'l', 'l', 'o', '\0'};
    std::cout << str1 << std::endl;

    // 3. char as 2D array to string 1D array
    constexpr char constChar[][5+1] = {
        "Ashry",
        "Hello",
        "World"
    };
    std::cout << constChar << std::endl;

    // 4. char pointer
    const char* value = "Char Pointer Value";
    char* charPtr = new char[18+1]; // 5 characters + 1 null '\0'
    memcpy(charPtr, value, sizeof(*charPtr)*18+1);
    
    std::cout << charPtr << std::endl;

    // 5. char double pointer
    char alphabet[] = {'a', 'b', 'c', 'd', 'e', '\0'};
    char** charDoublePtr = new char*[3];

    for (int r=0; r<3; r++) {

        charDoublePtr[r] = new char[(5+1)];
        for (int c=0; c<5; c++) {
            charDoublePtr[r][c] = alphabet[c];
        }
        charDoublePtr[r][5] = '\0'; // add null terminated

        std::cout << charDoublePtr[r] << std::endl;
    }

    // deleting the pointers
    delete [] charPtr;

    for (int r=0; r<3; r++) {
        delete [] charDoublePtr[r];
    }
    delete [] charDoublePtr;

    return 0;
}