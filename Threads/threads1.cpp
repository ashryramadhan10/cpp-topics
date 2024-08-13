#include <iostream>
#include <thread>
#include <stdexcept>

using namespace std;

class BackgroundTask {
    public:
        void operator()() const {
            try {
                printf("Do Something\n");
            } catch (const exception& e) {
                cout << "Exception " << e.what() << endl;
            }
        }
};

int main(int argc, char* argv[]) {
    BackgroundTask task;
    thread my_thread(task);
    my_thread.join();
    
    return 0;
}