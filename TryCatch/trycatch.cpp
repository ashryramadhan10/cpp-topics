#include <iostream>

template <class Type> class Stack {
    private:
        Type data;
        Stack<Type>* next;
        Stack<Type>* prev;
    public:

        // exceptions
        class FullException {
            public:
                std::string origin;
                unsigned int length;

                FullException(const std::string _origin, unsigned int _length): origin(_origin), length(_length) {}
        };
        class EmptyException {
            public:
                std::string origin;
                unsigned int length;

                EmptyException(const std::string _origin, unsigned int _length): origin(_origin), length(_length) {}
        };

        Stack<Type>* top;
        static unsigned int length;
        static unsigned int STACK_MAX_SIZE;

        Stack();
        ~Stack();
        void push(Type data);
        Type pop();
        void clear();
};

template <class Type>
unsigned int Stack<Type>::length = 0;

template <class Type>
unsigned int Stack<Type>::STACK_MAX_SIZE = 5;

template <class Type> 
Stack<Type>::Stack(): next(nullptr), prev(nullptr), top(nullptr) {}

template <class Type>
Stack<Type>::~Stack() {}

template <class Type>
void Stack<Type>::push(Type _data) {

    if (length >= STACK_MAX_SIZE)
        throw FullException("void Stack<Type>::push(Type _data)", length);

    Stack<Type>* stack = new Stack<Type>();         // create new stack
    stack->data = _data;
    stack->next = nullptr;
    stack->prev = top;

    if (top != nullptr) {
        top->next = stack;
    }
    top = stack;
    ++length;
}

template <class Type>
Type Stack<Type>::pop() {

    if (length <= 0)
        throw EmptyException("Type Stack<Type>::pop()", length);

    Type topData = top->data;
    
    if (top->prev != nullptr) {
        top = top->prev;
        delete top->next;
    } else {
        delete top;
        top = nullptr;
    }
    --length;

    if (length == 0) {
        top = nullptr;
        next = nullptr;
        prev = nullptr;
    }

    return topData;
}

template <class Type>
void Stack<Type>::clear() {
    Stack<Type>* current = top;
    while(current->prev != nullptr) {
        current = current->prev;
        delete current->next;
    }
    delete current;
    top = nullptr;
    next = nullptr;
    prev = nullptr;
    length = 0;
}

int main(int argc, char* argv[]) {

    Stack<int>* stack = new Stack<int>();

    try {
        stack->push(1);
        stack->push(2);
        stack->push(3);
        stack->push(4);
        stack->push(5);
        stack->push(7);
        // stack->pop();
    } catch (Stack<int>::FullException fullException) {
        std::cout << "Excpetion: Stack is full!" << std::endl;
        std::cout << "Origin: " << fullException.origin << ", Stack length: " << fullException.length << std::endl;
    } catch (Stack<int>::EmptyException emptyException) {
        std::cout << "Excpetion: Stack is empty!" << std::endl;
        std::cout << "Origin: " << emptyException.origin << ", Stack length: " << emptyException.length << std::endl;
    }

    stack->clear();

    return 0;
}