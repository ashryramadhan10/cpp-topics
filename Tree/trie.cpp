#include <iostream>
#include <vector>
#include <string>

class Node {
public:
    bool is_entry;
    std::vector<Node*> children;

    Node(): is_entry{false} {
        children = std::vector<Node*>(27, nullptr);
    }
    ~Node() {
        for (auto child : children) {
            delete child;
        }
    }
};

class Trie {
private:
    Node* root;

public:
    Trie() {
        root = new Node();
    }

    ~Trie() {
        delete root;
    }

    void insert(std::string&& new_value) {
        insertNode(root, std::move(new_value), 0);
    }

    void insertNode(Node* node, std::string&& new_value, int index) {
        if (index == new_value.length()) {
            node->is_entry = true;
            return;
        } else {
            char next_letter = new_value[index];
            int next_index = letterToIndex(next_letter);

            if (node->children[next_index] == nullptr) {
                node->children[next_index] = new Node();
                insertNode(node->children[next_index], std::move(new_value), index + 1);
            } else {
                insertNode(node->children[next_index], std::move(new_value), index + 1);
            }
        }
    }

    void search(std::string&& target) {
        root = searchNode(root, std::move(target), 0);
    }

    int letterToIndex(char next_letter) {
        return std::tolower(next_letter) - 97;
    }

    Node* searchNode(Node* node, std::string&& target, int index) {
        // in search of correct string, first check the current index with target length
        // and check whether the node is a valid entry not
        if (index == target.length()) {
            if (node->is_entry) {
                std::cout << "valid entry" << std::endl;
                return node;
            } else {
                return nullptr;
            }
        }

        // index start from 0, and it will increase by index + 1
        char next_letter = target[index];
        int next_index = letterToIndex(next_letter);
        Node* next_child = node->children[next_index];

        if (next_child == nullptr) {
            return nullptr;
        } else {
            return searchNode(next_child, std::move(target), index + 1);
        }
    }
};

int main(int argc, char* argv[]) {

    Trie trie;
    trie.insert("Ramadhan");
    trie.insert("Ramadhani");

    return 0;
}