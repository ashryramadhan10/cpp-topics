#include <iostream>
#include <vector>

class Stack {
private:
    int size;
    int top;
    std::vector<int> vec;

public:

    Stack(): size{50}, top{0} {
       vec = std::vector<int>(size, 0);
    }

    Stack(int size_): size{size_}, top{0} {
       vec = std::vector<int>(size, 0);
    }

    void push(int&& value) {
        if (top < size) {
            // top 0 - insert 1
            // top 1 - insert 2
            // top 2 - insert 3
            // top 3 - can't insert, correct
           vec[top++] = std::move(value);
        } else {
            std::cout << "Data Oversize" << std::endl;
        }
    }

    int pop() {
        if (top > 0) {
            top--;
            int result = vec[top];
            vec[top] = 0; // flush the data
            return result;
        } else {
            top--;
            std::cout << "No data left" << std::endl;
            exit(1);
        }
    }

    ~Stack() = default;
};

int main(int argc, char* argv[]) {

    Stack stack{50};

    stack.push(1);
    stack.push(2);

    int pop = stack.pop();
    std::cout << pop << " ";

    pop = stack.pop();
    std::cout << pop << " ";

    return 0;
}