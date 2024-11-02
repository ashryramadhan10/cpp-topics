#include <vector>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <cassert>

template <typename T, size_t t = 3>
class BTree;  // Forward declaration

template <typename T, size_t t = 3>
class BTreeNode {
private:
    bool leaf;
    std::vector<T> keys;
    std::vector<BTreeNode<T, t>*> children;

    friend class BTree<T, t>;  // Only specific BTree instantiation is friend

    // Private constructor to ensure nodes are only created through BTree
    explicit BTreeNode(bool isLeaf = true) : leaf(isLeaf) {
        // Reserve space for maximum possible keys and children
        keys.reserve(2 * t - 1);
        if (!isLeaf) {
            children.reserve(2 * t);
        }
    }

    // Private destructor ensures cleanup only through BTree
    ~BTreeNode() {
        for (auto* child : children) {
            delete child;
        }
    }

    // Prevent copying of nodes
    BTreeNode(const BTreeNode&) = delete;
    BTreeNode& operator=(const BTreeNode&) = delete;

    // Helper functions for node operations
    void insertKey(const T& key, size_t pos) {
        keys.insert(keys.begin() + pos, key);
    }

    void insertChild(BTreeNode* child, size_t pos) {
        children.insert(children.begin() + pos, child);
    }

    void removeKeysAfter(size_t pos) {
        keys.resize(pos);
    }

    void removeChildrenAfter(size_t pos) {
        children.resize(pos);
    }

public:
    // Public methods for querying node state
    bool isFull() const { 
        return keys.size() >= 2 * t - 1; 
    }

    bool hasMinKeys() const { 
        return keys.size() >= t - 1; 
    }

    size_t getKeyCount() const { 
        return keys.size(); 
    }
    
    T getKey(size_t index) const { 
        assert(index < keys.size());
        return keys[index]; 
    }
    
    bool isLeaf() const { 
        return leaf; 
    }

    // Debug method to verify node properties
    bool verifyProperties() const {
        // Verify key count constraints
        if (keys.size() > 2 * t - 1) return false;
        
        // Verify keys are in ascending order
        for (size_t i = 1; i < keys.size(); i++) {
            if (keys[i] <= keys[i-1]) return false;
        }
        
        // Verify children count
        if (!leaf && children.size() != keys.size() + 1) return false;
        
        return true;
    }
};

template <typename T, size_t t = 3>
class BTree {
private:
    BTreeNode<T, t>* root;

    // Helper method to find insertion position in a node
    static size_t findPosition(const BTreeNode<T, t>* node, const T& key) {
        return std::lower_bound(node->keys.begin(), node->keys.end(), key) - node->keys.begin();
    }

    void splitChild(BTreeNode<T, t>* parent, size_t childIndex) {
        BTreeNode<T, t>* child = parent->children[childIndex];
        BTreeNode<T, t>* newNode = new BTreeNode<T, t>(child->leaf);
        
        // Move the right half of keys to the new node
        for (size_t j = 0; j < t - 1; j++) {
            newNode->keys.push_back(child->keys[j + t]);
        }
        
        // Move the right half of children if not leaf
        if (!child->leaf) {
            for (size_t j = 0; j < t; j++) {
                newNode->children.push_back(child->children[j + t]);
            }
            child->removeChildrenAfter(t);
        }
        
        // Move median key to parent
        T medianKey = child->keys[t - 1];
        child->removeKeysAfter(t - 1);
        
        // Insert new node into parent
        parent->insertChild(newNode, childIndex + 1);
        parent->insertKey(medianKey, childIndex);
        
        assert(parent->verifyProperties());
        assert(child->verifyProperties());
        assert(newNode->verifyProperties());
    }

    void insertNonFull(BTreeNode<T, t>* node, const T& key) {
        size_t i = node->getKeyCount() - 1;
        
        if (node->isLeaf()) {
            // Find position and insert key
            while (i != size_t(-1) && key < node->getKey(i)) {
                i--;
            }
            node->insertKey(key, i + 1);
        } else {
            // Find appropriate child
            while (i != size_t(-1) && key < node->getKey(i)) {
                i--;
            }
            i++;
            
            // Split child if full
            if (node->children[i]->isFull()) {
                splitChild(node, i);
                if (key > node->getKey(i)) {
                    i++;
                }
            }
            insertNonFull(node->children[i], key);
        }
    }

    void printNode(const BTreeNode<T, t>* node, int level, std::ostream& out) const {
        if (!node) return;
        
        out << "Level " << level << ": ";
        for (size_t i = 0; i < node->getKeyCount(); ++i) {
            out << node->getKey(i) << " ";
        }
        out << '\n';
        
        if (!node->isLeaf()) {
            for (auto* child : node->children) {
                printNode(child, level + 1, out);
            }
        }
    }

    const BTreeNode<T, t>* searchNode(const BTreeNode<T, t>* node, const T& key) const {
        size_t i = 0;
        while (i < node->getKeyCount() && key > node->getKey(i)) {
            i++;
        }
        
        if (i < node->getKeyCount() && key == node->getKey(i)) {
            return node;
        }
        
        if (node->isLeaf()) {
            return nullptr;
        }
        
        return searchNode(node->children[i], key);
    }

public:
    BTree() : root(new BTreeNode<T, t>()) {
        static_assert(t >= 2, "B-Tree minimum degree must be at least 2");
    }
    
    ~BTree() {
        delete root;
    }
    
    // Prevent copying
    BTree(const BTree&) = delete;
    BTree& operator=(const BTree&) = delete;
    
    void insert(const T& key) {
        if (!root) {
            throw std::runtime_error("Tree is not initialized");
        }
        
        BTreeNode<T, t>* r = root;
        
        if (r->isFull()) {
            BTreeNode<T, t>* newRoot = new BTreeNode<T, t>(false);
            newRoot->children.push_back(r);
            root = newRoot;
            splitChild(newRoot, 0);
            insertNonFull(newRoot, key);
        } else {
            insertNonFull(r, key);
        }
    }
    
    bool search(const T& key) const {
        return searchNode(root, key) != nullptr;
    }
    
    void print(std::ostream& out = std::cout) const {
        out << "B-Tree Structure (t=" << t << "):\n";
        printNode(root, 0, out);
    }

    // Verify the entire tree maintains B-tree properties
    bool verifyTree() const {
        if (!root) return false;
        
        // Helper function to verify a subtree
        std::function<bool(const BTreeNode<T,t>*, bool)> verifySubtree = 
            [&](const BTreeNode<T,t>* node, bool isRoot) -> bool {
            if (!node->verifyProperties()) return false;
            
            // Root can have fewer keys, other nodes must have at least t-1 keys
            if (!isRoot && !node->hasMinKeys()) return false;
            
            // Recursively verify children
            if (!node->isLeaf()) {
                for (auto* child : node->children) {
                    if (!verifySubtree(child, false)) return false;
                }
            }
            return true;
        };
        
        return verifySubtree(root, true);
    }
};