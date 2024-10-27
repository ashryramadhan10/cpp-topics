#include <iostream>
#include <vector>

class Node {
public:
    Node* left;
    Node* right;
    int value;
    int index;

    Node(int val, int idx): left{nullptr}, right{nullptr}, value{val}, index{idx} {}

    ~Node() {
        delete this->left;
        delete this->right;
        this->left = nullptr;
        this->right = nullptr;
    }
};

class BinaryTree {
public:
    Node* root;
    std::vector<Node*> nodeArray;  // Added to store nodes in array form

    // Empty constructor
    BinaryTree() : root {nullptr} {}

    // Get node by index
    Node* getNodeByIndex(int index) {
        if (index < 0 || index >= nodeArray.size()) {
            return nullptr;
        }
        return nodeArray[index];
    }

    // Get left child index
    int getLeftChildIndex(int parentIndex) {
        int leftIndex = 2 * parentIndex + 1;
        return (leftIndex < nodeArray.size()) ? leftIndex : -1;
    }

    // Get right child index
    int getRightChildIndex(int parentIndex) {
        int rightIndex = 2 * parentIndex + 2;
        return (rightIndex < nodeArray.size()) ? rightIndex : -1;
    }

    // Get parent index
    int getParentIndex(int childIndex) {
        if (childIndex <= 0) return -1;
        return (childIndex - 1) / 2;
    }

    // Insert a new node with a defined value
    void insert(int&& value) {
        int newIndex = nodeArray.size();  // Get next available index
        this->root = this->insertRec(this->root, value, newIndex);
    }

    void deleteAllSubNodes() {
        this->deleteAllSubNodes(this->root);
        nodeArray.clear();
    }

    void show() const {
        printTree(this->root);
    }

    // Added: Print tree with indices
    void showWithIndices() const {
        for (size_t i = 0; i < nodeArray.size(); i++) {
            if (nodeArray[i] != nullptr) {
                printf("Index %zu: Value %d\n", i, nodeArray[i]->value);
            }
        }
    }

private:
    // insert the binary tree
    Node* insertRec(Node* node, int value, int index) {
        if (node == nullptr) {
            Node* newNode = new Node(value, index);
            // Ensure vector has enough space
            if (index >= nodeArray.size()) {
                nodeArray.resize(index + 1, nullptr);
            }
            nodeArray[index] = newNode;
            return newNode;
        }

        if (value < node->value) {
            node->left = insertRec(node->left, value, 2 * node->index + 1);
        } else if (value > node->value) {
            node->right = insertRec(node->right, value, 2 * node->index + 2);
        }

        return node;
    }

    // delete binary tree using post order traversal
    void deleteAllSubNodes(Node* node) {
        if (node == nullptr) {
            return;
        }

        deleteAllSubNodes(node->left);
        deleteAllSubNodes(node->right);

        if (node->left != nullptr) {
            delete node->left;
            node->left = nullptr;
        }

        if (node->right != nullptr) {
            delete node->right;
            node->right = nullptr;
        }
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

int main(int argc, char* argv[]) {

    BinaryTree* binaryTree = new BinaryTree();

    binaryTree->insert(50);
    binaryTree->insert(30);
    binaryTree->insert(20);
    binaryTree->insert(40);
    binaryTree->insert(70);
    binaryTree->insert(60);
    binaryTree->insert(80);
    
    printf("Tree in pre-order traversal:\n");
    binaryTree->show();
    printf("\n\nTree with indices:\n");
    binaryTree->showWithIndices();

    // Example of using indices
    Node* node = binaryTree->getNodeByIndex(0);  // Get root
    if (node) {
        int leftIdx = binaryTree->getLeftChildIndex(node->index);
        int rightIdx = binaryTree->getRightChildIndex(node->index);
        
        printf("\nRoot (index 0): %d\n", node->value);
        if (leftIdx != -1) {
            printf("Left child (index %d): %d\n", leftIdx, binaryTree->getNodeByIndex(leftIdx)->value);
        }
        if (rightIdx != -1) {
            printf("Right child (index %d): %d\n", rightIdx, binaryTree->getNodeByIndex(rightIdx)->value);
        }
    }

    binaryTree->deleteAllSubNodes();
    delete binaryTree;

    return 0;
}