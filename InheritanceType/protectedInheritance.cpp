#include <iostream>

class Base {
private:
    int pvt = 1;
protected:
    int prot = 2;
public:
    int pub = 3;

    // function to access private data member
    int getPvt() {
        return this->pvt;
    }
};

class PublicDerived : protected Base {
public:

    int getProt() {
        return prot;
    }

    int getPub() {
        return pub;
    }
};

int main(int argc, char* argv[]) {

    PublicDerived pd;

    // anything under the private in Base Class is not allowed
    // std::cout << "Private: " << pd.getPvt() << std::endl;
    std::cout << "Protected: " << pd.getProt() << std::endl;

    // public can't access directly anymore
    std::cout << "Public: " << pd.getPub() << std::endl;
       
    return 0;
}