#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

template <typename Key, int ORDER = 4>
class BTree {
private:
    struct Node {
        std::vector<Key> keys;
        std::vector<Node*> children;
        bool is_leaf;
        
        Node() : is_leaf(true) {
            keys.reserve(ORDER - 1);
            children.reserve(ORDER);
        }
    };

    Node* root;

    // Split a child node during insertion
    void split_child(Node* parent, int index) {
        int midpoint = (ORDER - 1) / 2;
        Node* child = parent->children[index];
        Node* new_child = new Node();

        // Move upper half keys to new node
        new_child->keys.assign(
            child->keys.begin() + midpoint + 1, 
            child->keys.end()
        );
        child->keys.resize(midpoint);

        // Handle children for non-leaf nodes
        if (!child->is_leaf) {
            new_child->is_leaf = false;
            new_child->children.assign(
                child->children.begin() + midpoint + 1, 
                child->children.end()
            );
            child->children.resize(midpoint + 1);
        }

        // Insert median key into parent
        parent->keys.insert(
            parent->keys.begin() + index, 
            child->keys[midpoint]
        );
        parent->children.insert(
            parent->children.begin() + index + 1, 
            new_child
        );
    }

    // Insert into a non-full node
    void insert_non_full(Node* node, const Key& key) {
        int i = node->keys.size() - 1;

        if (node->is_leaf) {
            // Simple insertion into leaf
            node->keys.push_back(key);
            std::sort(node->keys.begin(), node->keys.end());
        } else {
            // Find the child to descend into
            while (i >= 0 && key < node->keys[i]) {
                i--;
            }
            i++;

            // If child is full, split it
            if (node->children[i]->keys.size() == ORDER - 1) {
                split_child(node, i);
                
                // Adjust descent path if necessary
                if (key > node->keys[i]) {
                    i++;
                }
            }

            insert_non_full(node->children[i], key);
        }
    }

    // Recursive search
    bool search_key(Node* node, const Key& key) const {
        if (!node) return false;

        // Find the first key >= target
        auto it = std::lower_bound(node->keys.begin(), node->keys.end(), key);
        
        // If found in current node
        if (it != node->keys.end() && *it == key) 
            return true;

        // If leaf and not found, return false
        if (node->is_leaf) 
            return false;

        // Recurse into appropriate child
        int index = std::distance(node->keys.begin(), it);
        return search_key(node->children[index], key);
    }

    // Recursive print
    void print_tree(Node* node, int depth = 0) const {
        if (!node) return;

        for (size_t i = 0; i < node->keys.size(); ++i) {
            // First recurse into left child if not leaf
            if (!node->is_leaf && i < node->children.size()) {
                print_tree(node->children[i], depth + 1);
            }
            
            // Print current key with indentation
            std::cout << std::string(depth * 2, ' ') 
                      << node->keys[i] << " ";
        }

        // Recurse into last child if not leaf
        if (!node->is_leaf && !node->children.empty()) {
            print_tree(node->children.back(), depth + 1);
        }
    }

    // Find index of key in a node
    int find_key_index(Node* node, const Key& key) const {
        auto it = std::find(node->keys.begin(), node->keys.end(), key);
        return it != node->keys.end() ? std::distance(node->keys.begin(), it) : -1;
    }

    // Recursive delete
    bool delete_key(Node* node, const Key& key) {
        int index = find_key_index(node, key);

        if (index != -1) {
            if (node->is_leaf) {
                // Simple removal from leaf
                node->keys.erase(node->keys.begin() + index);
                return true;
            }

            // Key is in internal node - replace with predecessor
            Node* predecessor_child = node->children[index];
            if (!predecessor_child->is_leaf) {
                while (!predecessor_child->is_leaf) {
                    predecessor_child = predecessor_child->children.back();
                }
            }
            
            if (!predecessor_child->keys.empty()) {
                Key predecessor = predecessor_child->keys.back();
                node->keys[index] = predecessor;
                return delete_key(node->children[index], predecessor);
            }
        }

        // If key not found in this node, descend
        if (node->is_leaf) return false;

        // Find appropriate child to descend
        int child_index = 0;
        while (child_index < node->keys.size() && key > node->keys[child_index]) {
            child_index++;
        }

        return delete_key(node->children[child_index], key);
    }

public:
    // Constructor
    BTree() : root(new Node()) {}

    // Insert a key
    void insert(const Key& key) {
        Node* r = root;

        // If root is full, create new root and split
        if (r->keys.size() == ORDER - 1) {
            Node* new_root = new Node();
            new_root->is_leaf = false;
            new_root->children.push_back(root);
            root = new_root;
            split_child(new_root, 0);
        }

        insert_non_full(root, key);
    }

    // Update a key
    void update(const Key& old_key, const Key& new_key) {
        if (!search(old_key)) {
            throw std::runtime_error("Key to update not found");
        }
        
        // Remove old key and insert new key
        remove(old_key);
        insert(new_key);
    }

    // Remove a key
    void remove(const Key& key) {
        if (!delete_key(root, key)) {
            throw std::runtime_error("Key not found");
        }

        // If root is empty and has only one child, replace it
        if (root->keys.empty() && !root->is_leaf) {
            Node* old_root = root;
            root = root->children[0];
            delete old_root;
        }
    }

    // Search for a key
    bool search(const Key& key) const {
        return search_key(root, key);
    }

    // Print the tree
    void print() const {
        std::cout << "B-Tree Contents:\n";
        print_tree(root);
        std::cout << std::endl;
    }
};

int main() {
    BTree<int> btree;

    // Insert values
    btree.insert(10);
    btree.insert(20);
    btree.insert(5);
    btree.insert(15);
    btree.insert(30);

    std::cout << "Initial B-Tree:\n";
    btree.print();

    // Update a value
    std::cout << "After updating 20 to 25:\n";
    btree.update(20, 25);
    btree.print();

    // Remove a value
    std::cout << "After removing 5:\n";
    btree.remove(5);
    btree.print();

    return 0;
}