#include <iostream>
#include <thread>

using namespace std;

class background_task {
public:
    // this will be executed automatically when we call the instance of this class
    void operator () () const {
        cout << "Hello from background_task operator () routine" << endl;
    }
};

class another_background_task {
public:

    void operator () () const {
        cout << "Hello from another_background_task operator () routine" << endl;
    }
};

int main(int argc, char* argv[]) {

    background_task f;
    thread thr1(f);

    another_background_task af;
    thread thr2(af);

    thr1.join();
    thr2.join();

    return 0;
}