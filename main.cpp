#include <iostream>

class Node {
public:
    Node* left;
    Node* right;
    int value;

    Node(int val): left{nullptr}, right{nullptr}, value{val} {}
};

class BinaryTree {
public:
    Node* root;

    // Empty constructor
    BinaryTree() : root {nullptr} {}

    // Insert a new node with a defined value
    void insert(int&& value) {
        this->root = this->insertRec(this->root, value);
    }

    void show() const {
        printTree(this->root);
    }

private:
    Node* insertRec(Node* node, int value) {
        if (node == nullptr) {
            return new Node(value);
        }

        if (value < node->value) {
            node->left = insertRec(node->left, value);
        } else if (value > node->value) {
            node->right = insertRec(node->right, value);
        }

        return node;
    }

    void printTree(Node* node) const {
        if (node == nullptr) {
            return;
        }

        printTree(node->left);
        printf("%d ", node->value);
        printTree(node->right);
    }
};

int main(int argc, char* argv[]) {

    BinaryTree* binaryTree = new BinaryTree();

    binaryTree->insert(50);
    binaryTree->insert(30);
    binaryTree->insert(20);
    binaryTree->insert(40);
    binaryTree->insert(70);
    binaryTree->insert(60);
    binaryTree->insert(80);
    
    binaryTree->show();

    delete binaryTree;

    return 0;
}