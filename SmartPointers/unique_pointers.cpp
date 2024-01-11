#include <iostream>
#include <memory>
#include <vector>

class Test {
private:
    int data;
public:
    Test(): data{0} { std::cout << "Empty Test Constructor, data: " << data << std::endl; }
    Test(int _data): data{_data} { std::cout << "1 Argument Test Constructor, data: " << data << std::endl; }
    int getData() const { return data; }
    ~Test() { std::cout << "Test Destructor, data: " << data << std::endl; }
};

int main(int argc, char* argv[]) {

    // ==================== classical way ====================
    // this code will not going to be deleted without the `delete` keyword
    // Test* t = new Test{1000};
    // we need to call the code below to delete the `t` object
    // delete t 

    // ==================== smart unique pointer way ====================
    // using only make_unique, then the t will be std::unique_pointer<Test> type (smart pointer)
    // this will call the 1 Argument Test Constructor and the Test Destructor automatically

    // origin code examples:
    // 1). std::unique_pointer<Test> t = new Test{1000}
    // 2). std::unique_pointer<Test> t = std::make_unique<Test>(1000)

    // efficient alter: 
    auto t = std::make_unique<Test>(1000);
    std::cout << "t->getData(): " << t->getData() << std::endl;

    std::cout << "====================" << std::endl << std::endl;

    // ==================== smart unique pointer rules ====================
    // 1). we can't just assign it to another pointer because it was unique
    // 2). we can only move it
    // 3). we can use it in polymorphism
    // 4). we can use it with vector

    // example rule (1):
    // std::unique_ptr<Test> t_ptr;
    // t_ptr = t; --> this will cause an Error
    // std::unique_ptr<Test> t_ptr = t; --> this will cause an Error also

    // example rule (2):
    std::unique_ptr<Test> t_ptr = std::move(t);
    std::cout << "t_ptr->getData(): " << t_ptr->getData() << std::endl;
    // std::cout << "t->getData(): " << t->getData() << std::endl; // --> this will cause an Error, then stop the program to call `Test Destructor`
    if (!t) {
        std::cout << "t is nullptr" << std::endl;
    }

    // example rule (4)
    std::vector<std::unique_ptr<Test>> testVec;

    testVec.push_back(std::make_unique<Test>(1));
    testVec.push_back(std::make_unique<Test>(2));
    testVec.push_back(std::make_unique<Test>(3));

    // below code will cause an Error, because we are copying `testVec` to `e`
    // for (auto e: testVec) {
    //     std::cout << e->getData << " ";
    // }
    // std::cout << "\n";

    // we need to make `e` a const reference of the testVec
    for (const auto &e: testVec) {
        std::cout << e->getData() << " ";
    }
    std::cout << "\n";

    return 0;
}