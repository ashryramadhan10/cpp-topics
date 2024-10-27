#include <iostream>
#include <algorithm>

class Node {
public:
    Node* left;
    Node* right;
    int value;
    int height;  // Added height field for AVL

    Node(int val) 
        : left{nullptr}, right{nullptr}, value{val}, height{1} {}  // Height starts at 1

    ~Node() {
        delete left;
        delete right;
        left = nullptr;
        right = nullptr;
    }
};

class AVLTree {
public:
    Node* root;

    AVLTree() : root{nullptr} {}

    // Get height of a node (handles null case)
    int getHeight(Node* node) {
        return (node == nullptr) ? 0 : node->height;
    }

    // Get balance factor of a node
    int getBalanceFactor(Node* node) {
        if (node == nullptr) return 0;
        return getHeight(node->left) - getHeight(node->right);
    }

    // Update height of a node based on its children
    void updateHeight(Node* node) {
        if (node == nullptr) return;
        node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
    }

    void insert(int value) {
        root = insertRec(root, value);
    }

    void show() const {
        printTree(this->root);
    }

private:
    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        // Perform rotation
        x->right = y;
        y->left = T2;

        // Update heights
        updateHeight(y);  // Update y first as it's now lower
        updateHeight(x);  // Then update x as it depends on y's height

        return x;  // New root of this subtree
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        // Perform rotation
        y->left = x;
        x->right = T2;

        // Update heights
        updateHeight(x);  // Update x first as it's now lower
        updateHeight(y);  // Then update y as it depends on x's height

        return y;  // New root of this subtree
    }

    Node* insertRec(Node* node, int value) {
        // 1. Perform standard BST insert
        if (node == nullptr) {
            return new Node(value);
        }

        if (value < node->value)
            node->left = insertRec(node->left, value);
        else if (value > node->value)
            node->right = insertRec(node->right, value);
        else
            return node;  // Duplicate values not allowed

        // 2. Update height of current node
        updateHeight(node);

        // 3. Get balance factor to check if node became unbalanced
        int balance = getBalanceFactor(node);

        // 4. If unbalanced, there are 4 cases:

        // Left Left Case
        if (balance > 1 && value < node->left->value) {
            printf("Left Left Case: Performing right rotation at %d\n", node->value);
            return rotateRight(node);
        }

        // Right Right Case
        if (balance < -1 && value > node->right->value) {
            printf("Right Right Case: Performing left rotation at %d\n", node->value);
            return rotateLeft(node);
        }

        // Left Right Case
        if (balance > 1 && value > node->left->value) {
            printf("Left Right Case: Performing left-right rotation at %d\n", node->value);
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        // Right Left Case
        if (balance < -1 && value < node->right->value) {
            printf("Right Left Case: Performing right-left rotation at %d\n", node->value);
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

     // pre-order traversal
    void printTree(Node* node) const {
        if (node == nullptr) {
            return;
        }

        printf("%d ", node->value);
        printTree(node->left);
        printTree(node->right);
    }
};

int main() {
    AVLTree tree;

    // Example insertions to demonstrate different rotation cases
    printf("Inserting 10\n");
    tree.insert(10);
    tree.show();
    printf("\n");

    printf("Inserting 20\n");
    tree.insert(20);
    tree.show();
    printf("\n");

    printf("Inserting 30\n");  // Will trigger left rotation
    tree.insert(30);
    tree.show();
    printf("\n");

    printf("Inserting 15\n");
    tree.insert(15);
    tree.show();
    printf("\n");

    printf("Inserting 5\n");
    tree.insert(5);
    tree.show();
    printf("\n");

    printf("Inserting 25\n");
    tree.insert(25);
    tree.show();
    printf("\n");

    return 0;
}