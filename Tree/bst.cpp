#include <iostream>
#include <stdlib.h>

class Node {
public:
    int value;
    int height;
    Node* left;
    Node* right;
    Node* parent;

    Node(int&& _value): value{_value}, height{1}, left{nullptr}, right{nullptr}, parent {nullptr} {}
};

class AVLTree {
private:
    Node* root;

public:
    AVLTree(): root {nullptr} {}

    ~AVLTree() {
        deleteAllData(root);
    }

    void deleteAllData(Node* node) {
        if (node == nullptr) {
            return;
        }

        deleteAllData(node->left);
        deleteAllData(node->right);

        Node* to_be_deleted = node;
        node->left = nullptr;
        node->right = nullptr;
        node = nullptr;
        delete to_be_deleted;
    }

    Node* rotateRight(Node** root, Node* y) {
        Node* x = y->left;
        Node* t2 = x->right;

        x->parent = y->parent;
        if (y->parent == nullptr) {
            (*root) = x;
        } else if (y->parent->left == y) {
            y->parent->left = x;
        } else {
            y->parent->right = x;
        }

        x->right = y;
        y->parent = x;
        y->left = t2;

        if (t2 != nullptr) {
            t2->parent = y;
        }

        updateNodeHeight(y);
        updateNodeHeight(x);

        return x;
    }

    Node* rotateLeft(Node** root, Node* y) {
        Node* x = y->right;
        Node* t2 = x->left;

        x->parent = y->parent;
        if (y->parent == nullptr) {
            (*root) = x;
        } else if (y->parent->left == y) {
            y->parent->left = x;
        } else {
            y->parent->right = x;
        }

        x->left = y;
        y->parent = x;
        y->right = t2;

        if (t2 != nullptr) {
            t2->parent = y;
        }

        updateNodeHeight(y);
        updateNodeHeight(x);

        return x;
    }

    int getNodeHeight(Node* node) {
        if (node == nullptr) {
            return 0;
        }

        return node->height;
    }

    void updateNodeHeight(Node* node) {
        int leftNodeHeight = getNodeHeight(node->left);
        int rightNodeHeight = getNodeHeight(node->right);

        node->height = std::max(leftNodeHeight, rightNodeHeight) + 1;
    }

    int getBalanceFactor(Node* node) {
        int leftNodeHeight = getNodeHeight(node->left);
        int rightNodeHeight = getNodeHeight(node->right);
        return leftNodeHeight - rightNodeHeight;
    }

    void insert(int&& value) {
        root = insertNode(&root, root, std::move(value));
    }

    Node* insertNode(Node** root, Node* node, int&& value) {
        if (node == nullptr) {
            return new Node(std::move(value));
        }

        if (value < node->value) {
            node->left = insertNode(root, node->left, std::move(value));
            node->left->parent = node;
        } else {
            node->right = insertNode(root, node->right, std::move(value));
            node->right->parent = node;
        }

        updateNodeHeight(node);
        int balanceFactor = getBalanceFactor(node);

        // LL rotation
        if (balanceFactor > 1 && value < node->left->value) {
            return rotateRight(root, node);
        }

        // LR rotation
        if (balanceFactor > 1 && value > node->left->value) {
            node->left = rotateLeft(root, node->left);
            return rotateRight(root, node);
        }

        // RR rotation
        if (balanceFactor < -1 && value > node->right->value) {
            return rotateLeft(root, node);
        }

        // RL rotation
        if (balanceFactor < -1 && value < node->right->value) {
            node->right = rotateRight(root, node->right);
            return rotateLeft(root, node);
        }

        // no need for rotation
        return node;
    }

    void preOrderTraversalNode(Node* node) const {
        if (node == nullptr) {
            return;
        }

        std::cout << node->value << " ";
        preOrderTraversalNode(node->left);
        preOrderTraversalNode(node->right);
    }

    void inOrderTraversalNode(Node* node) const {
        if (node == nullptr) {
            return;
        }

        inOrderTraversalNode(node->left);
        std::cout << node->value << " ";
        inOrderTraversalNode(node->right);
    }

    void postOrderTraversalNode(Node* node) const {
        if (node == nullptr) {
            return;
        }

        postOrderTraversalNode(node->left);
        postOrderTraversalNode(node->right);
        std::cout << node->value << " ";
    }

    void preOrderTraveral() const {
        preOrderTraversalNode(root);
    }

    void inOrderTraversal() const {
        inOrderTraversalNode(root);
    }

    void postOrderTraversal() const {
        postOrderTraversalNode(root);
    }

    void findValue(int&& value) const {
        if (root == nullptr) {
            std::cout << "No data in the tree" << std::endl;
        } else {
            Node* resultNode = findValueNode(root, std::move(value));
            if (resultNode != nullptr) {
                std::cout << resultNode->value << std::endl;
            } else {
                std::cout << "nullptr" << std::endl;
            }
        }
    }

    Node* findValueNode(Node* node, int&& value) const {
        if (node == nullptr) {
            return nullptr;
        }

        if (node->value == value) {
            return node;
        }

        if (value < node->value && node->left != nullptr) {
            return findValueNode(node->left, std::move(value));
        }

        if (value > node->value && node->right != nullptr) {
            return findValueNode(node->right, std::move(value));
        }
        
        return nullptr;
    }

    void deleteValue(int&& value) {
        Node* node = findValueNode(root, std::move(value));
        if (node != nullptr) {
            deleteNode(&root, node);
        } else {
            std::cout << "Data not found!" << std::endl;
        }
    }

    void deleteNode(Node** root, Node* node) {
        // 1. delete a leaf node
        // 2. delete a parent node with single child
        // 3. delete a parent node with two children, need to find successor
        // the key to find successor is always be located at leftmost of the first right node
        // the successor need to be a leaf node, why? because it will be easier to swap a leaf node
        // rather than a parent node
        // in this deletion only work for AVLTree or Self Balanced Tree

        if ((*root) == nullptr || node == nullptr) {
            return;
        }

        // 1. case deleting a leaf node
        if (node->left == nullptr && node->right == nullptr) {
            // delete itself
            if (node->parent == nullptr) {
                (*root) = nullptr;
            } else if (node->parent->left == node) { // then this is a left node of the parent
                node->parent->left = nullptr;
            } else {
                node->parent->right = nullptr;
            }
            delete node;
            return;
        }

        // 2. case deleting a parent node with single child
        if (node->left == nullptr || node->right == nullptr) {
            // get the correct child first
            Node* child = node->left;
            if (child == nullptr) {
                child = node->right;
            }
            
            // set node parent same as child parent
            child->parent = node->parent;

            // a root node
            if (node->parent == nullptr) {
                // just for safety transfer the child first to the root
                // actually if we delete node first, it will not deletes its child automatically
                // because we didn't define anything inside the Node's class destructor
                (*root) = child;
            } else if(node->parent->left == node) {
                // same here
                node->parent->left = child;
            } else {
                // same here
                node->parent->right = child;
            }
            delete node;
            return;
        }

        // 3. deleting with two children
        // find the successor first and delete it
        Node* successor = node->right;
        while (successor->left != nullptr) {
            successor = successor->left;
        }

        // before delete the successor, allocate a new memory with its data
        Node* sucessorCopy = new Node(std::move(successor->value));

        // delete the successor
        deleteNode(root, successor);

        // change the successor parent pov first
        // swap it first, then delete
        if (node->parent == nullptr) {
            (*root) = sucessorCopy;
        } else if (node->parent->left == node) {
            node->parent->left = sucessorCopy;
        } else {
            node->parent->right = sucessorCopy;
        }

        // change the successor children pov afterwards
        sucessorCopy->parent = node->parent;
        sucessorCopy->left = node->left;
        node->left->parent = sucessorCopy;

        sucessorCopy->right = node->right;
        // there is no way we can set nullptr parent, because nullptr is not an object
        // there is a possibility node->right is a nullptr while searching the successor
        // because after searching the program will delete the successor
        if (node->right != nullptr) {
            node->right->parent = sucessorCopy;
        }

        sucessorCopy->height = node->height;

        delete node;
    }
};

int main(int argc, char* argv[]) {

    AVLTree avltree;
    avltree.insert(50);
    avltree.insert(23);
    avltree.insert(67);
    avltree.insert(14);
    avltree.insert(38);
    avltree.insert(60);
    avltree.insert(81);
    avltree.insert(6);
    avltree.insert(17);
    avltree.insert(27);
    avltree.insert(42);
    avltree.insert(59);
    avltree.insert(63);
    avltree.insert(78);
    avltree.insert(92);
    avltree.insert(1);
    avltree.insert(7);
    avltree.insert(21);
    avltree.insert(29);
    avltree.insert(58);
    avltree.insert(91);
    avltree.insert(95);

    avltree.preOrderTraveral();

    printf("\n");
    avltree.findValue(99);

    printf("\n");
    avltree.deleteValue(81);
    avltree.preOrderTraveral();

    return 0;
}