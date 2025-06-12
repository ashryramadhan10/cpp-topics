#include <iostream>

class Node {
public:
    int value;
    Node* next;
    Node* prev;

    Node(int value_): value{value_}, next{nullptr}, prev{nullptr} {}
};

class LinkedList {
private:
    int size;
    Node* head;
    Node* tail;

public:

    LinkedList(): size{0}, head{nullptr}, tail{nullptr} {}
    ~LinkedList() {
        Node* current = head;
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
    }

    // prevent copy
    LinkedList(const LinkedList&) = delete;
    LinkedList& operator=(const LinkedList&) = delete;

    void insert(int&& value) {
        Node* newNode = new Node(std::move(value));

        if (head == nullptr && tail == nullptr) {
            head = newNode;
            tail = newNode;
            newNode = nullptr; // to prevent delete out of this function scope
        } else {
            newNode->prev = tail; // on second iteration, same as newNode->prev = head
            tail->next = newNode; // same as this on second iteration literally represent for head->next = newNode
            tail = newNode;
        }

        size += 1;
    }

    void deleteFront() {

        if (size == 0) {
            std::cout << "No data to be deleted!" << std::endl;
        }

        // data size only 1
        if (head == tail && (head != nullptr || head->next == nullptr)) {
            delete head;
            head = nullptr;
            tail = nullptr;
        }

        // at least has 2 items in the linked list
        if (head != nullptr && size > 1) {
            Node* to_be_deleted = head;
            head = to_be_deleted->next;
            delete to_be_deleted;
            head->prev = nullptr;
        }

        size -= 1;
    }

    void deleteEnd() {
        if (size == 0) {
            std::cout << "No data to be deleted!" << std::endl;
        }

        // data size only 1
        if (tail == head && (head != nullptr || head->next == nullptr)) {
            std::cout << head->value << std::endl;
            delete head;
            head = nullptr;
            tail = nullptr;
        }

        if (tail != nullptr && size > 1) {
            std::cout << tail->value << std::endl;
            Node* to_be_deleted = tail;
            tail = to_be_deleted->prev;
            delete to_be_deleted;
            tail->next = nullptr;
        }

        size -= 1;
    }

    void deleteAt(int pos) {
        if (pos == 0) {
            deleteFront();
        } else if (pos == size - 1) {
            deleteEnd();
        } else {
            if (pos < 0 || pos >= size) {
                std::cout << "Index out of bound" << std::endl;
                return;
            } else {
                Node* current = head;
                int i = 0;
                while (i < pos && current != nullptr) {
                    current = current->next;
                    i++;
                }
                current->prev->next = current->next;
                current->next->prev = current->prev;
                delete current;
            }
        }

        size -= 1;
    }

    void displayAllData(bool reverse = false) const {
        if (reverse) {
            Node* current = tail;
            while (current != nullptr) {
                std::cout << current->value << " ";
                current = current->prev;
            }
            std::cout << std::endl;
        } else {
            Node* current = head;
            while (current != nullptr) {
                std::cout << current->value << " ";
                current = current->next;
            }
            std::cout << std::endl;
        }
    }
};

class Stack {
private:
    int top;
    int size;
    LinkedList* list;

public:
    Stack(int size_): top{0}, size{size_} {
        list = new LinkedList();
    }

    ~Stack() {
        delete list;
        list = nullptr;
    }

    void push(int&& value) {
        if (top < size) {
            list->insert(std::move(value));
            top++;
        } else {
            std::cout << "Insert out of bound" << std::endl;
        }
    }

    void pop() {
        if (top > 0) {
            list->deleteEnd();
            top--;
        }
    }

    void displayAllData() const {
        list->displayAllData(true);
    }
};

int main(int argc, char* argv[]) {

    Stack stack{10};

    stack.push(1);
    stack.push(2);
    stack.push(3);

    stack.displayAllData();

    stack.pop();

    stack.displayAllData();

    stack.pop();
    
    stack.displayAllData();

    return 0;
}