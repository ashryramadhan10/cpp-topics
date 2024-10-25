#include <iostream>
#include <thread>

struct func
{
    /* data */
    int& i;
    func (int& i_): i{i_} {}
    void operator() () {
        for (unsigned int j = 0; j < 100000; j++) {
            // do_something()
        }
    }
};
