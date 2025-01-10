#include <iostream>
#include<variant>

/*
Unions in C++ were once commonly used to store values of different types in the same memory
space. However, unions suffered from significant safety issues, such as type punning (where
you interpret a value of one type as a different type), leading to undefined behavior. C++17
introduced std::variant, a type-safe alternative to unions that enforces stronger guarantees
regarding which type is currently active.

std::variant is a template class that can hold one of several types but ensures that only one
type is active at any given time. It acts as a type-safe union, offering type safety when accessing
the contained type. Unlike traditional unions, which rely on raw memory access and can lead to
type errors, std::variant ensures that you can only access the current active type, thus
avoiding undefined behavior.

reference: simplyfycpp.org
*/

using my_variant = std::variant<int,double,std::string>;

void printVariant(const my_variant& v){
    std::visit([](auto&& arg) {
        std::cout << arg << std::endl;
        },
    v);
}

int main(int argc, char* argv[]) {

    my_variant v1=10; //Holdsan int
    my_variant v2=3.14; //Holdsadouble
    my_variant v3="Hello,world!"; //Holdsastring
    printVariant(v1); //Prints:10
    printVariant(v2); //Prints:3.14
    printVariant(v3); //Prints:Hello,world!

    return 0;
}