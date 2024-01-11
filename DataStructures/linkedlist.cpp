#include <iostream>
#include <math.h>

class Node {
private:
    int data;
    Node* next;
    Node* prev;
public:

    Node(): data(0), next(NULL), prev(NULL) {}
    ~Node() {}

    void insertAtBeginning(int data, Node** head, Node** tail, unsigned int& length) {
        Node* newNode = new Node();
        newNode->data = data;
        newNode->prev = NULL;
        newNode->next = (*head);

        if ((*head) == NULL && (*tail) == NULL) {
            (*tail) = newNode;
            (*head) = newNode;
        } else {
            (*head)->prev = newNode;
            (*head) = newNode;
        }
        length += 1;
    }

    void insertAtEnd(int data, Node** head, Node** tail, unsigned int& length) {
        Node* newNode = new Node();
        newNode->data = data;
        newNode->prev = (*tail);
        newNode->next = NULL;

        if ((*head) == NULL && (*tail) == NULL) {
            (*head) = newNode;
            (*tail) = newNode;
        } else {
            (*tail)->next = newNode;
            (*tail) = newNode;
        }
        length += 1;
    }

    void insertAt(unsigned int pos, int data, Node** head, Node** tail, unsigned int& length) {
        if (length < 2) {
            std::cout << "At least, the Linkedlist length is greater than or equal to 2!" << std::endl;
        } else {
            // at least this is more faster than if we tend to start it from head or tail randomly
            if (pos < ceil((length / 2))) {
                Node* current = (*head);

                unsigned int iter = 1;
                while (iter < pos && current->next != NULL) {
                    current = current->next;
                    ++iter;
                }

                Node* newNode = new Node();
                newNode->data = data;
                newNode->next = current->next;
                newNode->prev = current;

                if (current->next != NULL) {
                    current->next->prev = newNode;
                }
                current->next = newNode;

            } else {
                Node* current = (*tail);

                unsigned int iter = length;
                while (iter >= pos && current->prev != NULL) {
                    current = current->prev;
                    iter--;
                }

                Node* newNode = new Node();
                newNode->data = data;
                newNode->next = current->next;
                newNode->prev = current;

                if (current->next != NULL) {
                    current->next->prev = newNode;
                }

                current->next = newNode;
            }
        }
        length += 1;
    }

    void deleteAtBeginning(Node** head, Node** tail, unsigned int& length) {
        if (length > 0) {
            Node* current_head = (*head);

            if (current_head->next != NULL) {

                current_head->next->prev = NULL;
                (*head) = current_head->next;

                length = length - 1;
                delete current_head;

            } else {
                length = length - 1;
                delete current_head;

                (*head) = NULL;
                (*tail) = NULL;
            }
        } else {
            std::cout << "Linkedlist is empty!" << std::endl;
        }
    }

    void deleteAtEnd(Node** head, Node** tail, unsigned int& length) {
        if (length > 0) {
            Node* current_tail = (*tail);

            if (current_tail->prev != NULL) {

                current_tail->prev->next = NULL;
                (*tail) = current_tail->prev;

                length = length - 1;
                delete current_tail;

            } else {

                length = length - 1;
                delete current_tail;

                (*head) = NULL;
                (*tail) = NULL;
            }
        } else {
            std::cout << "Linkedlist is empty!" << std::endl;
        }
    }

    void deleteAllNodes(Node** head, Node** tail, unsigned int& length) {
        if (length > 0) {
            Node* current = (*head);
            while (current->next != NULL) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
            delete current;

            (*head) = NULL;
            (*tail) = NULL;
            length = 0;
        } else {
            std::cout << "Linkedlist is empty!" << std::endl;
        }
    }

    void display(Node** head, Node** tail, unsigned int& length) {
        
        std::cout << "================== LINKED LIST ==================" << std::endl;
        std::cout << "numbers of node: " << length << std::endl; 

        Node* current = (*head);

        if ((*head) == NULL && (*tail) == NULL && length == 0) {
            std::cout << "\nLinked List is Empty!" << std::endl;
        } else {
            while (current->next != NULL) {
                std::cout << current->data << std::endl;
                current = current->next;
            }
            std::cout << current->data << std::endl;
        }
    }
};

class LinkedList {
public:
    unsigned int elementLength;
    Node* head;
    Node* tail;
    Node* nodes;

    LinkedList(): elementLength{0}, head{nullptr}, tail{nullptr} {
        nodes = new Node();
    }

    ~LinkedList() {
        delete nodes;
    }

    void insertAtBeginning(int data) {
        nodes->insertAtBeginning(data, &this->head, &this->tail, this->elementLength);
    }

    void insertAtEnd(int data) {
        nodes->insertAtEnd(data, &this->head, &this->tail, this->elementLength);
    }

    void insertAt(unsigned int pos, int data) {
        nodes->insertAt(pos, data, &this->head, &this->tail, this->elementLength);
    }

    void deleteAtBeginning() {
        nodes->deleteAtBeginning(&this->head, &this->tail, this->elementLength);
    }

    void deleteAtEnd() {
        nodes->deleteAtEnd(&this->head, &this->tail, this->elementLength);
    }

    void deleteAllNodes() {
        nodes->deleteAllNodes(&this->head, &this->tail, this->elementLength);
    }

    void display() {
        nodes->display(&this->head, &this->tail, this->elementLength);
    }
};

int main(int argc, char* argv[]) {

    LinkedList* ls1 = new LinkedList();

    ls1->insertAtBeginning(5);
    ls1->insertAtBeginning(3);
    ls1->deleteAtBeginning();
    ls1->display();
    ls1->deleteAtBeginning();
    ls1->display();

    delete ls1;

    return 0;
}