#include <iostream>

/*
Insertion Rules:
- If tree is empty, create new node as root node with color BLACK
- If tree is not empty, create new node as leafnode with color RED
- If parent of the new node is BLACK then exit (done)
- If parent of the new node is RED then check the color of parent's sibling of new node:
    * If color is BLACK or NIL than do suitable ROTATION and RECOLOR
    * if color is RED then recolor the parent and parent's sibling, and also check if grandparent of the new node (not root node) then RECOLOR & RECGHECK


*/

class Node {
public:
    Node* left;
    Node* right;
    Node* parent;  // Added parent pointer for rotations
    int value;
    bool isRed;    // true for red, false for black

    Node(int val)
        : left{nullptr}, right{nullptr}, parent{nullptr}, 
          value{val}, isRed{true} {}  // New nodes are always red

    ~Node() {
        delete left;
        delete right;
        left = nullptr;
        right = nullptr;
    }
};

class RedBlackTree {
public:
    Node* root;

    RedBlackTree() : root{nullptr} {}

    void insert(int&& value) {
        Node* node = insertBST(value);  // First do normal BST insert
        fixRedBlackPropertiesAfterInsert(node);  // Then fix RB properties
    }

    void show() const {
        printf("Red-Black Tree (R: Red, B: Black):\n");
        printTree(root, "", true);
    }

    void deleteAllSubNodes() {
        delete root;
        root = nullptr;
    }

private:
    Node* insertBST(int value) {
        // If tree is empty
        if (root == nullptr) {
            root = new Node(value);
            root->isRed = false;  // Root must be black
            return root;
        }

        Node* current = root;
        Node* parent = nullptr;

        // Find the insertion point
        while (current != nullptr) {
            parent = current;
            if (value < current->value)
                current = current->left;
            else if (value > current->value)
                current = current->right;
            else
                return current;  // Value already exists
        }

        // Create new node
        Node* newNode = new Node(value);
        newNode->parent = parent;

        // Insert the node
        if (value < parent->value)
            parent->left = newNode;
        else
            parent->right = newNode;

        return newNode;
    }

    void rotateLeft(Node* node) {
        Node* rightChild = node->right;
        node->right = rightChild->left;

        if (rightChild->left != nullptr)
            rightChild->left->parent = node;

        rightChild->parent = node->parent;

        if (node->parent == nullptr)
            root = rightChild;
        else if (node == node->parent->left)
            node->parent->left = rightChild;
        else
            node->parent->right = rightChild;

        rightChild->left = node;
        node->parent = rightChild;
    }

    void rotateRight(Node* node) {
        Node* leftChild = node->left;
        node->left = leftChild->right;

        if (leftChild->right != nullptr)
            leftChild->right->parent = node;

        leftChild->parent = node->parent;

        if (node->parent == nullptr)
            root = leftChild;
        else if (node == node->parent->right)
            node->parent->right = leftChild;
        else
            node->parent->left = leftChild;

        leftChild->right = node;
        node->parent = leftChild;
    }

    void fixRedBlackPropertiesAfterInsert(Node* node) {
        // Case 1: Node is root
        if (node == root) {
            node->isRed = false;
            return;
        }

        // Case 2: Parent is black - nothing to do
        if (!node->parent->isRed)
            return;

        // For remaining cases, we need uncle and grandparent
        Node* parent = node->parent;
        Node* grandparent = parent->parent;
        if (grandparent == nullptr)
            return;

        Node* uncle = (grandparent->left == parent) ? 
                      grandparent->right : grandparent->left;

        // Case 3: Both parent and uncle are red
        if (uncle != nullptr && uncle->isRed) {
            parent->isRed = false;
            uncle->isRed = false;
            grandparent->isRed = true;
            fixRedBlackPropertiesAfterInsert(grandparent);
            return;
        }

        // Case 4: Parent is red, uncle is black (or null), and node is "inner grandchild"
        if (node == parent->right && parent == grandparent->left) {
            rotateLeft(parent);
            node = node->left;
        } else if (node == parent->left && parent == grandparent->right) {
            rotateRight(parent);
            node = node->right;
        }

        // Case 5: Parent is red, uncle is black (or null), and node is "outer grandchild"
        parent = node->parent;
        grandparent = parent->parent;
        parent->isRed = false;
        grandparent->isRed = true;
        if (node == parent->left)
            rotateRight(grandparent);
        else
            rotateLeft(grandparent);
    }

    void printTree(Node* node, std::string prefix, bool isLeft) const {
        if (node == nullptr)
            return;

        std::cout << prefix;
        std::cout << (isLeft ? "├──" : "└──");
        std::cout << node->value << (node->isRed ? "(R)" : "(B)") << std::endl;

        printTree(node->left, prefix + (isLeft ? "│   " : "    "), true);
        printTree(node->right, prefix + (isLeft ? "│   " : "    "), false);
    }
};

int main() {
    RedBlackTree* tree = new RedBlackTree();

    // Insert same values as your original example
    tree->insert(50);
    tree->insert(30);
    tree->insert(20);
    tree->insert(40);
    tree->insert(70);
    tree->insert(60);
    tree->insert(80);

    tree->show();

    tree->deleteAllSubNodes();
    delete tree;

    return 0;
}