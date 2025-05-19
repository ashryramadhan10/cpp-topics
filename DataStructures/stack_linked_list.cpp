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
        if (head == tail) {
            delete head;
            head = nullptr;
            tail = nullptr;
        }

        // at least has 2 items in the linked list
        if (head != nullptr && size > 1) {
            Node* to_be_deleted = head;
            head = to_be_deleted->next;
            head->prev = nullptr;
            delete to_be_deleted;
        }

        size -= 1;
    }

    void deleteEnd() {
        if (size == 0) {
            std::cout << "No data to be deleted!" << std::endl;
        }

        // data size only 1
        if (tail == head) {
            delete head;
            head = nullptr;
            tail = nullptr;
        }

        if (tail != nullptr && size > 1) {
            Node* to_be_deleted = tail;
            tail = to_be_deleted->prev;
            tail->next = nullptr;
            delete to_be_deleted;
        }

        size -= 1;
    }

    void deleteAt(int&& pos) {
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

    void displayAllData() const {
        Node* current = head;
        while (current != nullptr) {
            std::cout << current->value << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
};

int main(int argc, char* argv[]) {

    LinkedList ll;
    ll.insert(5);
    ll.insert(6);
    ll.insert(7);

    ll.displayAllData();

    ll.deleteFront();

    ll.displayAllData();

    return 0;
}