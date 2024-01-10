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

class PublicDerived : private Base {
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

    // std::cout << "Private: " << pd.getPvt() << std::endl;
    std::cout << "Protected: " << pd.getProt() << std::endl;
    std::cout << "Public: " << pd.getPub() << std::endl;
       
    return 0;
}