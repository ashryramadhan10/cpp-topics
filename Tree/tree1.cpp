#include <iostream>

class Node {
public:
    Node* left;
    Node* right;
    int value;
};

void preOrder(Node* node) {
    if (node == NULL) {
        return;
    } else {
        printf("%d, ", node->value);
        preOrder(node->left);
        preOrder(node->right);
    }
}

void inOrder(Node* node) {
    if (node == NULL) {
        return;
    }

    inOrder(node->left);
    printf("%d, ", node->value);
    inOrder(node->right);
}

void postOrder(Node *node) {
    if (node == NULL) {
        return;
    }

    postOrder(node->left);
    postOrder(node->right);
    printf("%d, ", node->value);
}

int main(int argc, char* argv[]) {

    

    return 0;
}