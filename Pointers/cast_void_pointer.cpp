#include <iostream>

int main(int argc, char* argv[]) {

    void* generalPtr = nullptr;

    generalPtr = new int(17);

    std::cout << *(static_cast<int*>(generalPtr)) << std::endl;

    delete static_cast<int*>(generalPtr);

    return 0;
}