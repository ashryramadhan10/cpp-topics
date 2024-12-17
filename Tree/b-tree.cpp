#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

// A complete B-Tree implementation for a minimum degree `T`.
// NOTE: For a B-Tree, minimum degree T means:
// - Every node (except root) has at least T-1 keys.
// - Every node can have at most 2T-1 keys.
//
// This implementation supports:
//   - Insert
//   - Remove
//   - Search
//   - Print traversal

template <typename Key, int T = 3>  // T is the minimum degree, must be >= 2
class BTree {
    static_assert(T >= 2, "Minimum degree T must be at least 2");

private:
    struct Node {
        bool is_leaf;
        std::vector<Key> keys;
        std::vector<Node*> children; 

        Node(bool leaf) : is_leaf(leaf) {
            // Maximum keys: 2T-1
            // Maximum children: 2T (for non-leaf)
            keys.reserve(2*T - 1);
            children.reserve(2*T);
        }
    };

    Node* root;

    // Utility function to search a key in a subtree rooted with this node.
    // Returns true if present.
    bool search_internal(Node* node, const Key& key) const {
        if (!node) return false;

        // Find the first key greater than or equal to key
        int i = 0;
        while (i < (int)node->keys.size() && key > node->keys[i]) {
            i++;
        }

        // If the found key is equal to key, return true
        if (i < (int)node->keys.size() && node->keys[i] == key) {
            return true;
        }

        // If the key is not found here and this is a leaf node
        if (node->is_leaf) {
            return false;
        }

        // Go to the appropriate child
        return search_internal(node->children[i], key);
    }

    // Splits the full child y of node x at given index i.
    void split_child(Node* x, int i) {
        Node* y = x->children[i];
        Node* z = new Node(y->is_leaf);
        
        // Move the last (T-1) keys of y to z
        for (int j = 0; j < T-1; j++) {
            z->keys.push_back(y->keys[j+T]);
        }

        // If y is not leaf, transfer its children
        if (!y->is_leaf) {
            for (int j = 0; j < T; j++) {
                z->children.push_back(y->children[j+T]);
            }
            y->children.resize(T);
        }

        y->keys.resize(T-1);

        // Insert z into x's children
        x->children.insert(x->children.begin() + i + 1, z);
        // Move the median key of y to x
        x->keys.insert(x->keys.begin() + i, y->keys[T-1]);
        y->keys.resize(T-1);
    }

    // Insert a key into a non-full node
    void insert_non_full(Node* x, const Key& k) {
        int i = (int)x->keys.size()-1;

        if (x->is_leaf) {
            // Insert the new key at the correct position in the leaf node
            x->keys.push_back(k);
            // Sort or simply move keys (since keys are sorted)
            while (i >= 0 && x->keys[i] > k) {
                x->keys[i+1] = x->keys[i];
                i--;
            }
            x->keys[i+1] = k;
        } else {
            // Find the child which will have the new key
            while (i >= 0 && x->keys[i] > k) {
                i--;
            }
            i++;
            // If the child is full
            if ((int)x->children[i]->keys.size() == 2*T-1) {
                split_child(x, i);
                // After splitting, the median of x->children[i] moves up and 
                // x->children[i] is split into two. 
                // Check which of the two children is now the correct one for k.
                if (k > x->keys[i]) {
                    i++;
                }
            }
            insert_non_full(x->children[i], k);
        }
    }

    // A utility function to traverse the tree for printing.
    void traverse_internal(Node* node, int depth=0) const {
        if (!node) return;

        int i;
        for (i = 0; i < (int)node->keys.size(); i++) {
            // If this is not a leaf, then before printing key[i],
            // traverse the subtree rooted with child[i].
            if (!node->is_leaf) {
                traverse_internal(node->children[i], depth+1);
            }
            // Print keys in this node
            std::cout << std::string(depth*2, ' ') << node->keys[i] << "\n";
        }

        // Print the subtree rooted with last child
        if (!node->is_leaf) {
            traverse_internal(node->children[i], depth+1);
        }
    }

    // A helper function to get the index of the key in node's keys.
    // If the key is present, returns the index, otherwise returns the index 
    // where the key would be inserted if it were present.
    int find_key(Node* x, const Key& k) const {
        int idx=0;
        while (idx<(int)x->keys.size() && x->keys[idx]<k) {
            ++idx;
        }
        return idx;
    }

    // Remove a key from a subtree rooted with this node
    void remove_internal(Node* x, const Key& k) {
        int idx = find_key(x, k);

        if (idx < (int)x->keys.size() && x->keys[idx] == k) {
            // The key to be removed is present in this node
            if (x->is_leaf) {
                // If the node is a leaf node - remove the key.
                x->keys.erase(x->keys.begin()+idx);
            } else {
                // The node is an internal node
                remove_internal_internal_node(x, k, idx);
            }
        } else {
            // The key is not in this node
            if (x->is_leaf) {
                // The key is not in the tree
                return;
            }

            // The key to be removed is in the subtree rooted with x->children[idx]
            bool flag = (idx == (int)x->keys.size());

            // Ensure that the child from where the key is supposed to be removed has at least T keys
            if ((int)x->children[idx]->keys.size() < T) {
                fill(x, idx);
            }

            // If the last child has been merged, it must have merged with the previous child
            // so we remove the key from the (idx-1)th child otherwise we remove from the (idx)th child
            if (flag && idx > (int)x->keys.size()) {
                remove_internal(x->children[idx-1], k);
            } else {
                remove_internal(x->children[idx], k);
            }
        }
    }

    // Remove the k-th key from an internal node x
    // The node x must contain the key at keys[idx].
    void remove_internal_internal_node(Node* x, const Key& k, int idx) {
        Key key = x->keys[idx];

        // If the child that precedes k has at least T keys, find the predecessor 'pred' 
        // and replace k by pred, remove pred from that subtree
        if ((int)x->children[idx]->keys.size() >= T) {
            Key pred = get_predecessor(x, idx);
            x->keys[idx] = pred;
            remove_internal(x->children[idx], pred);
        }
        // If the child that comes after k has at least T keys, find the successor 'succ' 
        // and replace k by succ, remove succ from that subtree
        else if ((int)x->children[idx+1]->keys.size() >= T) {
            Key succ = get_successor(x, idx);
            x->keys[idx] = succ;
            remove_internal(x->children[idx+1], succ);
        } else {
            // Both children have less than T keys. Merge them.
            merge(x, idx);
            remove_internal(x->children[idx], k);
        }
    }

    // Get predecessor of keys[idx] in x
    Key get_predecessor(Node* x, int idx) {
        // Move to the rightmost node in left subtree
        Node* cur = x->children[idx];
        while (!cur->is_leaf) {
            cur = cur->children.back();
        }
        return cur->keys.back();
    }

    // Get successor of keys[idx] in x
    Key get_successor(Node* x, int idx) {
        // Move to the leftmost node in right subtree
        Node* cur = x->children[idx+1];
        while (!cur->is_leaf) {
            cur = cur->children.front();
        }
        return cur->keys.front();
    }

    // A function to fill child children[idx] which has less than T-1 keys
    void fill(Node* x, int idx) {
        // If the previous child has more than T-1 keys, borrow from it
        if (idx != 0 && (int)x->children[idx-1]->keys.size() >= T) {
            borrow_from_prev(x, idx);
        }

        // If the next child has more than T-1 keys, borrow from it
        else if (idx != (int)x->keys.size() && (int)x->children[idx+1]->keys.size() >= T) {
            borrow_from_next(x, idx);
        }

        // Merge children[idx] with its sibling
        // If idx is the last child merge it with the previous child, otherwise merge it with the next child
        else {
            if (idx != (int)x->keys.size()) {
                merge(x, idx);
            } else {
                merge(x, idx-1);
            }
        }
    }

    // Borrow a key from children[idx-1] and insert it into children[idx]
    void borrow_from_prev(Node* x, int idx) {
        Node* child = x->children[idx];
        Node* sibling = x->children[idx-1];

        // The last key from sibling goes up to the parent
        // The key at idx-1 from x moves down to child as the first key
        // sibling loses one key and child gains one key

        child->keys.insert(child->keys.begin(), x->keys[idx-1]);
        if (!child->is_leaf) {
            child->children.insert(child->children.begin(), sibling->children.back());
            sibling->children.pop_back();
        }

        x->keys[idx-1] = sibling->keys.back();
        sibling->keys.pop_back();
    }

    // Borrow a key from children[idx+1] and insert it into children[idx]
    void borrow_from_next(Node* x, int idx) {
        Node* child = x->children[idx];
        Node* sibling = x->children[idx+1];

        // The first key from sibling goes up to the parent
        // The key at idx from x moves down to child as the last key
        // sibling loses one key and child gains one key

        child->keys.push_back(x->keys[idx]);
        if (!child->is_leaf) {
            child->children.push_back(sibling->children.front());
            sibling->children.erase(sibling->children.begin());
        }

        x->keys[idx] = sibling->keys.front();
        sibling->keys.erase(sibling->keys.begin());
    }

    // Merge children[idx] and children[idx+1]
    void merge(Node* x, int idx) {
        Node* child = x->children[idx];
        Node* sibling = x->children[idx+1];

        // Insert the key from x into child
        child->keys.push_back(x->keys[idx]);

        // Insert keys of sibling into child
        for (auto& k : sibling->keys) {
            child->keys.push_back(k);
        }

        // Insert children of sibling into child
        if (!child->is_leaf) {
            for (auto& c : sibling->children) {
                child->children.push_back(c);
            }
        }

        // Remove the key and the pointer from x
        x->keys.erase(x->keys.begin()+idx);
        x->children.erase(x->children.begin()+idx+1);

        delete sibling;
    }

public:
    // Constructor
    BTree() : root(nullptr) {}

    // Search the given key in the B-Tree
    bool search(const Key& key) const {
        return search_internal(root, key);
    }

    // Insert a key to the B-Tree
    void insert(const Key& k) {
        // If tree is empty
        if (!root) {
            root = new Node(true);
            root->keys.push_back(k);  // Insert key
        } else {
            // If root is full, then tree grows in height
            if ((int)root->keys.size() == 2*T-1) {
                Node* s = new Node(false);
                s->children.push_back(root);

                // Split the old root
                split_child(s, 0);

                // Decide which of the two children will have the new key
                int i = 0;
                if (k > s->keys[0])
                    i++;
                insert_non_full(s->children[i], k);
                root = s;
            } else {
                // If root is not full, insert the key in it
                insert_non_full(root, k);
            }
        }
    }

    // Remove a key from the B-Tree
    void remove(const Key& k) {
        if (!root) {
            throw std::runtime_error("The tree is empty");
        }

        remove_internal(root, k);

        // If the root node has 0 keys, make its first child the new root
        if ((int)root->keys.size() == 0) {
            Node* tmp = root;
            if (!root->is_leaf) {
                root = root->children[0];
            } else {
                root = nullptr;
            }
            delete tmp;
        }
    }

    // Print the B-Tree (in a structured manner)
    void print() const {
        if (!root) {
            std::cout << "Tree is empty.\n";
            return;
        }
        std::cout << "B-Tree contents:\n";
        traverse_internal(root);
        std::cout << std::endl;
    }

    // Destructor to clean up memory
    ~BTree() {
        delete_subtree(root);
    }

private:
    // Recursively delete nodes
    void delete_subtree(Node* node) {
        if (!node) return;
        if (!node->is_leaf) {
            for (auto c : node->children) {
                delete_subtree(c);
            }
        }
        delete node;
    }
};

// Example usage
int main() {
    BTree<int, 3> btree;  // B-Tree with minimum degree T=3

    // Insert some keys
    btree.insert(10);
    btree.insert(20);
    btree.insert(5);
    btree.insert(6);
    btree.insert(12);
    btree.insert(30);
    btree.insert(7);
    btree.insert(17);

    std::cout << "Initial B-Tree:\n";
    btree.print();

    std::cout << "Removing 6...\n";
    btree.remove(6);
    btree.print();

    std::cout << "Removing 13 (not present, no change)...\n";
    try {
        btree.remove(13);
    } catch (const std::runtime_error& e) {
        std::cout << e.what() << "\n";
    }
    btree.print();

    std::cout << "Removing 7...\n";
    btree.remove(7);
    btree.print();

    std::cout << "Removing 10...\n";
    btree.remove(10);
    btree.print();

    std::cout << "Removing 20...\n";
    btree.remove(20);
    btree.print();

    // Search tests
    std::cout << "Searching for 12: " << (btree.search(12) ? "Found\n" : "Not found\n");
    std::cout << "Searching for 15: " << (btree.search(15) ? "Found\n" : "Not found\n");

    return 0;
}