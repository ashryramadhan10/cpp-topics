#include <iostream>

/*
The output should be look like this:

a: 1 + b: 2 = 15
a: 10 + b: 5 = 15
a: 10 + b: 5 = 15
result4: 3
result5: 3
result6: 3

*/

// 1. local variable as argument
int addition1(int a, int b) {
    a += 9;
    b += 3;
    return a + b;
}

// 2. reference variable as argument
int addition2(int& a, int& b) {
    a += 9;
    b += 3;
    return a + b;
}

// 3. pointer variable as argument
int addition3(int* a, int* b) {
    *a += 9;
    *b += 3;
    return *a + *b;
}

// 4. const variable as argument
int addition4(const int a, const int b) {
    return a + b;
}

// 5. rvalue as argument
int addition5(int&& a, int&& b) {
    return a + b;
}

// 6. default value
int addition6(int a=1, int b=2) {
    return a + b;
}

int main(int argc, char* argv[]) {

    int a = 1;
    int b = 2;

    int result1 = addition1(a, b);
    std::cout << "a: " << a << " + b: " << b << " = " << result1 << std::endl;

    a = 1; 
    b = 2;

    int result2 = addition2(a, b);
    std::cout << "a: " << a << " + b: " << b << " = " << result2 << std::endl;

    a = 1; 
    b = 2;

    int result3 = addition3(&a, &b);
    std::cout << "a: " << a << " + b: " << b << " = " << result3 << std::endl;

    int result4 = addition4(1, 2);
    std::cout << "result4: " << result4 << std::endl;

    int result5 = addition5(int(1), int(2));
    std::cout << "result5: " << result5 << std::endl;

    int result6 = addition6();
    std::cout << "result6: " << result6 << std::endl;

    return 0;
}