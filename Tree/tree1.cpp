#include <iostream>

class Node {
public:
    Node* left;
    Node* right;
    int value;

    Node(int val) : left(NULL), right(NULL), value(val) {}
};

class BinaryTree {
public:
    Node* root;

    BinaryTree() : root(NULL) {}

    // Insert a value into the tree
    void insert(int value) {
        root = insertRec(root, value);
    }

    // Delete a value from the tree
    void deleteValue(int value) {
        root = deleteRec(root, value);
    }

    // Pre-order traversal
    void preOrder(Node* node) {
        if (node == NULL) {
            return;
        }
        std::cout << node->value << ", ";
        preOrder(node->left);
        preOrder(node->right);
    }

    // In-order traversal
    void inOrder(Node* node) {
        if (node == NULL) {
            return;
        }
        inOrder(node->left);
        std::cout << node->value << ", ";
        inOrder(node->right);
    }

    // Post-order traversal
    void postOrder(Node* node) {
        if (node == NULL) {
            return;
        }
        postOrder(node->left);
        postOrder(node->right);
        std::cout << node->value << ", ";
    }

private:
    Node* insertRec(Node* node, int value) {
        if (node == NULL) {
            return new Node(value);
        }

        if (value < node->value) {
            node->left = insertRec(node->left, value);
        } else if (value > node->value) {
            node->right = insertRec(node->right, value);
        }

        return node;
    }

    Node* deleteRec(Node* node, int value) {
        if (node == NULL) {
            return node;
        }

        if (value < node->value) {
            node->left = deleteRec(node->left, value);
        } else if (value > node->value) {
            node->right = deleteRec(node->right, value);
        } else {
            // Node with only one child or no child
            if (node->left == NULL) {
                Node* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == NULL) {
                Node* temp = node->left;
                delete node;
                return temp;
            }

            // Node with two children: Get the inorder successor (smallest in the right subtree)
            Node* temp = minValueNode(node->right);

            // Copy the inorder successor's value to this node
            node->value = temp->value;

            // Delete the inorder successor
            node->right = deleteRec(node->right, temp->value);
        }
        return node;
    }

    Node* minValueNode(Node* node) {
        Node* current = node;

        // Loop down to find the leftmost leaf
        while (current && current->left != NULL) {
            current = current->left;
        }

        return current;
    }
};

int main() {
    BinaryTree tree;

    // Insert values into the tree
    tree.insert(50);
    tree.insert(30);
    tree.insert(20);
    tree.insert(40);
    tree.insert(70);
    tree.insert(60);
    tree.insert(80);

    // Perform tree traversals
    std::cout << "In-order traversal: ";
    tree.inOrder(tree.root);
    std::cout << std::endl;

    // Delete a value
    tree.deleteValue(20);
    std::cout << "In-order traversal after deleting 20: ";
    tree.inOrder(tree.root);
    std::cout << std::endl;

    tree.deleteValue(30);
    std::cout << "In-order traversal after deleting 30: ";
    tree.inOrder(tree.root);
    std::cout << std::endl;

    tree.deleteValue(50);
    std::cout << "In-order traversal after deleting 50: ";
    tree.inOrder(tree.root);
    std::cout << std::endl;

    return 0;
}
