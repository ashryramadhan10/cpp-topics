#include <iostream>

class Node {
public:
    Node* left;
    Node* right;
    int value;
    bool isThreadedLeft;
    bool isThreadedRight;

    Node(int val) 
        : left{nullptr}, right{nullptr}, value{val}, 
          isThreadedLeft{false}, isThreadedRight{false} {}

    ~Node() {
        // Only delete non-threaded pointers
        if (!isThreadedLeft) delete left;
        if (!isThreadedRight) delete right;
        left = right = nullptr;
    }
};

class ThreadedBinaryTree {
private:
    Node* root;

    // Get leftmost node starting from given node
    Node* leftmost(Node* node) {
        if (node == nullptr) return nullptr;
        while (node->left != nullptr && !node->isThreadedLeft) {
            node = node->left;
        }
        return node;
    }

    // Find inorder successor using threads
    Node* inorderSuccessor(Node* node) {
        if (node == nullptr) return nullptr;

        // If right pointer is threaded, it directly points to successor
        if (node->isThreadedRight) {
            return node->right;
        }

        // Otherwise, find the leftmost node in right subtree
        return leftmost(node->right);
    }

public:
    ThreadedBinaryTree() : root{nullptr} {}

    void insert(int value) {
        // Handle first node
        if (root == nullptr) {
            root = new Node(value);
            return;
        }

        // Find position to insert
        Node* current = root;
        Node* parent = nullptr;

        while (current != nullptr) {
            parent = current;

            if (value < current->value) {
                if (!current->isThreadedLeft && current->left != nullptr) {
                    current = current->left;
                } else {
                    break;
                }
            } else if (value > current->value) {
                if (!current->isThreadedRight && current->right != nullptr) {
                    current = current->right;
                } else {
                    break;
                }
            } else {
                // Value already exists
                return;
            }
        }

        // Create new node
        Node* newNode = new Node(value);

        // Insert as left child
        if (value < parent->value) {
            newNode->left = parent->left;  // Maintain left thread
            newNode->right = parent;       // Thread to parent (successor)
            newNode->isThreadedLeft = parent->isThreadedLeft;
            newNode->isThreadedRight = true;
            parent->left = newNode;
            parent->isThreadedLeft = false;
        }
        // Insert as right child
        else {
            newNode->right = parent->right;  // Maintain right thread
            newNode->left = parent;          // Thread to parent (predecessor)
            newNode->isThreadedRight = parent->isThreadedRight;
            newNode->isThreadedLeft = true;
            parent->right = newNode;
            parent->isThreadedRight = false;
        }
    }

    void inorderTraversal() {
        Node* current = leftmost(root);
        while (current != nullptr) {
            std::cout << current->value << " ";
            current = inorderSuccessor(current);
        }
        std::cout << std::endl;
    }

    // Utility function to print tree structure
    void printTree(Node* node, std::string prefix = "", bool isLeft = false) {
        if (node == nullptr) return;

        std::cout << prefix;
        std::cout << (isLeft ? "├──" : "└──");
        std::cout << node->value;
        
        // Print thread information
        if (node->isThreadedLeft || node->isThreadedRight) {
            std::cout << " (";
            if (node->isThreadedLeft) {
                std::cout << "L→" << (node->left ? node->left->value : 0);
            }
            if (node->isThreadedLeft && node->isThreadedRight) std::cout << ", ";
            if (node->isThreadedRight) {
                std::cout << "R→" << (node->right ? node->right->value : 0);
            }
            std::cout << ")";
        }
        std::cout << std::endl;

        // Recurse on actual children (not threads)
        if (!node->isThreadedLeft) {
            printTree(node->left, prefix + (isLeft ? "│   " : "    "), true);
        }
        if (!node->isThreadedRight) {
            printTree(node->right, prefix + (isLeft ? "│   " : "    "), false);
        }
    }

    void printTree() {
        printTree(root);
    }

    ~ThreadedBinaryTree() {
        delete root;
    }

};

int main() {
    ThreadedBinaryTree tree;

    // Insert some values
    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);
    tree.insert(60);
    tree.insert(80);

    std::cout << "Tree structure (threads shown in parentheses):\n";
    tree.printTree();

    std::cout << "\nInorder traversal using threads:\n";
    tree.inorderTraversal();

    return 0;
}