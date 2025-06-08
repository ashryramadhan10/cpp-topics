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
    bool deleteFlag;

public:
    Trie() {
        root = new Node();
        deleteFlag = false;
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
        Node* searchedNode = searchNode(root, std::move(target), 0);
        if (searchedNode != nullptr) {
            std::cout << "valid entry" << std::endl;
        } else {
            std::cout << "invalid entry" << std::endl;
        }
    }

    int letterToIndex(char next_letter) {
        return std::tolower(next_letter) - 97;
    }

    Node* searchNode(Node* node, std::string&& target, int index) {
        // in search of correct string, first check the current index with target length
        // and check whether the node is a valid entry not
        if (index == target.length()) {
            if (node->is_entry) {
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

    void remove(std::string&& target) {
        this->deleteFlag = false;
        deleteNode(this->root, std::move(target), 0, deleteFlag);
        if (deleteFlag) {
            std::cout << "deleted successfully" << std::endl;
        } else {
            std::cout << "deleted failed" << std::endl;
        }
    }

    void deleteNode(Node* node, std::string&& target, int index, bool& delete_flag) {
        if (index == target.length()) {
            if (node->is_entry) {
                node->is_entry = false;
                return;
            }
        }

        char next_letter = target[index];
        int next_index = letterToIndex(next_letter);

        if (node->children[next_index] != nullptr) {
            deleteNode(node->children[next_index], std::move(target), index + 1, delete_flag);
        } else {
            return;
        }

        // after delete the is_entry true
        // it will automatically deletes its children as well
        if (node->children[next_index] != nullptr && node->children[next_index]->is_entry) {
            return;
        }

        for (auto child : node->children[next_index]->children) {
            if (child != nullptr) {
                return;
            }
        }

        delete node->children[next_index];
        delete_flag = true;
        node->children[next_index] = nullptr;
    }
};

int main(int argc, char* argv[]) {

    Trie trie;
    trie.insert("Ramadhan");
    trie.insert("Ramadhani");

    trie.search("Ramadhani");

    trie.remove("Ramadhani");

    trie.search("Ramadhan");
    trie.search("Ramadhani");

    trie.remove("Ramadhan");

    trie.search("Ramadhan");
    return 0;
}