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
        if (deleteNode(this->root, std::move(target), 0)) {
            std::cout << "string completely deleted" << std::endl;
        } else {
            std::cout << "string partially deleted" << std::endl;
        }
    }

    bool deleteNode(Node* node, std::string&& target, int index) {
        if (index == target.length()) {
            if (node->is_entry) {
                node->is_entry = false;
                return true;
            }
        }

        char next_letter = target[index];
        int next_index = letterToIndex(next_letter);

        if (node->children[next_index] != nullptr) {
            if (deleteNode(node->children[next_index], std::move(target), index + 1)) {
                delete node->children[next_index];
                node->children[next_index] = nullptr;
            }
        } else {
            return false;
        }

        if (node->is_entry) {
            return false;
        }

        for (auto child : node->children) {
            if (child != nullptr) {
                return false;
            }
        }

        // current node will be a child on next stack of recursion
        // it will be deleted because this one return true;
        return true;
    }
};

int main(int argc, char* argv[]) {

    Trie trie;
    trie.insert("Ramadhan");
    trie.insert("Ramadhani");

    trie.search("Ramadhan");

    trie.remove("Ramadhan");

    trie.search("Ramadhan");
    trie.search("Ramadhani");

    trie.remove("Ramadhan");

    trie.search("Ramadhan");

    // this can be compared if we don't use a Trie structure
    // If we use Trie, we don't have to check it like this
    std::vector<std::string> vec;
    vec.emplace_back("Ena");
    vec.emplace_back("Enb");
    vec.emplace_back("Enc");

    const std::string input_string = "End";
    int flag = 0;
    for (auto str : vec) {
        flag = 0;

        for (int i = 0; i < str.length(); i++) {
            if (input_string[i] != str[i]) {
                flag += 1;
                break;
            }
        }

        if (flag == 0) {
            break;
        }
    }

    if (flag) {
        std::cout << "\nInput string is not listed in the Vec" << std::endl;
    } else {
        std::cout << "\nInput string found!" << std::endl;
    }

    return 0;
}