#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <stdexcept>

// A B-KD Tree: a B-Tree that stores K-dimensional points and
// uses a KD-Tree-like approach of choosing a dimension per level.
// Minimum degree T: every node (except root) must have at least T-1 keys.
// Each node can have at most 2T-1 keys.

template <typename CoordType, int K, int T = 3>
class BKDTree {
    static_assert(T >= 2, "Minimum degree T must be at least 2");
    static_assert(K >= 1, "Dimension K must be at least 1");

public:
    using Point = std::array<CoordType, K>;

private:
    struct Node {
        bool is_leaf;
        std::vector<Point> keys;
        std::vector<Node*> children;

        Node(bool leaf) : is_leaf(leaf) {
            keys.reserve(2*T - 1);
            children.reserve(2*T);
        }
    };

    Node* root;

public:
    BKDTree() : root(nullptr) {}

    ~BKDTree() {
        clear_subtree(root);
    }

    // Insert a new point into the B-KD Tree
    void insert(const Point& p) {
        if (!root) {
            root = new Node(true);
            root->keys.push_back(p);
        } else {
            if ((int)root->keys.size() == 2*T - 1) {
                Node* s = new Node(false);
                s->children.push_back(root);
                split_child(s, 0, 0);
                int i = 0;
                // Determine dimension using root depth = 1 (since s is new root)
                int dimension = 0; // root level dimension
                if (compare_points(p, s->keys[0], dimension) > 0) {
                    i++;
                }
                insert_non_full(s->children[i], p, 1);
                root = s;
            } else {
                insert_non_full(root, p, 0);
            }
        }
    }

    // Remove a point from the B-KD Tree
    void remove(const Point& p) {
        if (!root) {
            throw std::runtime_error("Tree is empty");
        }

        remove_internal(root, p, 0);

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

    // Search for a point
    bool search(const Point& p) const {
        return search_internal(root, p, 0);
    }

    // Print the B-KD Tree with indentation
    void print() const {
        if (!root) {
            std::cout << "Tree is empty.\n";
            return;
        }
        std::cout << "B-KD Tree contents:\n";
        print_internal(root, 0);
        std::cout << std::endl;
    }

private:
    // Recursively delete nodes
    void clear_subtree(Node* node) {
        if (!node) return;
        if (!node->is_leaf) {
            for (auto c : node->children) {
                clear_subtree(c);
            }
        }
        delete node;
    }

    // Compare two points along a given dimension:
    // returns negative if p1 < p2 on that dimension,
    // 0 if they are equal (all coords equal),
    // positive if p1 > p2 on that dimension.
    int compare_points(const Point& p1, const Point& p2, int dimension) const {
        // Primary comparison by chosen dimension
        if (p1[dimension] < p2[dimension]) return -1;
        if (p1[dimension] > p2[dimension]) return 1;

        // If equal on chosen dimension, we can break ties by subsequent dimensions (optional)
        // but for equality check, we must confirm all dimensions match
        for (int d = 0; d < K; d++) {
            if (p1[d] < p2[d]) return -1;
            if (p1[d] > p2[d]) return 1;
        }

        return 0; // completely equal
    }

    bool points_equal(const Point& p1, const Point& p2) const {
        for (int i = 0; i < K; i++) {
            if (p1[i] != p2[i]) return false;
        }
        return true;
    }

    bool search_internal(Node* node, const Point& p, int depth) const {
        if (!node) return false;
        int dimension = depth % K;

        int i = 0;
        while (i < (int)node->keys.size() && compare_points(p, node->keys[i], dimension) > 0) {
            i++;
        }

        // If found
        if (i < (int)node->keys.size() && points_equal(node->keys[i], p)) {
            return true;
        }

        // If leaf node, not found
        if (node->is_leaf) {
            return false;
        }

        return search_internal(node->children[i], p, depth+1);
    }

    void split_child(Node* x, int i, int depth) {
        int dimension = depth % K;
        Node* y = x->children[i];
        Node* z = new Node(y->is_leaf);

        // Move last T-1 keys from y to z
        for (int j = 0; j < T-1; j++) {
            z->keys.push_back(y->keys[j+T]);
        }

        // If not leaf, move children as well
        if (!y->is_leaf) {
            for (int j = 0; j < T; j++) {
                z->children.push_back(y->children[j+T]);
            }
            y->children.resize(T);
        }

        y->keys.resize(T-1);

        x->children.insert(x->children.begin() + i + 1, z);
        x->keys.insert(x->keys.begin() + i, y->keys[T-1]);
        y->keys.resize(T-1);
    }

    void insert_non_full(Node* x, const Point& p, int depth) {
        int dimension = depth % K;
        int i = (int)x->keys.size() - 1;

        if (x->is_leaf) {
            // Insert in sorted order by chosen dimension
            x->keys.push_back(p);
            // Insertion sort by dimension (and tie-break by full comparison)
            while (i >= 0 && compare_points(p, x->keys[i], dimension) < 0) {
                x->keys[i+1] = x->keys[i];
                i--;
            }
            x->keys[i+1] = p;
        } else {
            while (i >= 0 && compare_points(p, x->keys[i], dimension) < 0) {
                i--;
            }
            i++;
            if ((int)x->children[i]->keys.size() == 2*T-1) {
                split_child(x, i, depth+1);
                // After splitting, re-check where to go
                if (compare_points(p, x->keys[i], dimension) > 0) {
                    i++;
                }
            }
            insert_non_full(x->children[i], p, depth+1);
        }
    }

    void print_internal(Node* node, int depth) const {
        if (!node) return;

        int dimension = depth % K;

        for (int i = 0; i < (int)node->keys.size(); i++) {
            if (!node->is_leaf) {
                print_internal(node->children[i], depth+1);
            }
            std::cout << std::string(depth*2, ' ');
            print_point(node->keys[i]);
            std::cout << " (dim:" << dimension << ")\n";
        }

        if (!node->is_leaf) {
            print_internal(node->children[node->keys.size()], depth+1);
        }
    }

    void print_point(const Point& p) const {
        std::cout << "(";
        for (int i = 0; i < K; i++) {
            std::cout << p[i] << (i == K-1 ? "" : ", ");
        }
        std::cout << ")";
    }

    // Removal logic (similar to B-Tree deletion, using dimension at depth)
    void remove_internal(Node* x, const Point& p, int depth) {
        int dimension = depth % K;
        int idx = find_key(x, p, dimension);

        if (idx < (int)x->keys.size() && points_equal(x->keys[idx], p)) {
            // Key found
            if (x->is_leaf) {
                x->keys.erase(x->keys.begin()+idx);
            } else {
                remove_internal_non_leaf(x, idx, depth);
            }
        } else {
            // Key not found in this node
            if (x->is_leaf) {
                // Key doesn't exist
                return;
            }

            bool flag = (idx == (int)x->keys.size());
            if ((int)x->children[idx]->keys.size() < T) {
                fill(x, idx, depth);
            }

            if (flag && idx > (int)x->keys.size()) {
                remove_internal(x->children[idx-1], p, depth+1);
            } else {
                remove_internal(x->children[idx], p, depth+1);
            }
        }
    }

    // Find the index of the key in this node according to the node dimension
    int find_key(Node* x, const Point& p, int dimension) const {
        int idx=0;
        while (idx<(int)x->keys.size() && compare_points(p, x->keys[idx], dimension) > 0) {
            idx++;
        }
        return idx;
    }

    void remove_internal_non_leaf(Node* x, int idx, int depth) {
        Point p = x->keys[idx];
        // If left child has at least T keys
        if ((int)x->children[idx]->keys.size() >= T) {
            Point pred = get_predecessor(x, idx, depth);
            x->keys[idx] = pred;
            remove_internal(x->children[idx], pred, depth+1);
        }
        else if ((int)x->children[idx+1]->keys.size() >= T) {
            Point succ = get_successor(x, idx, depth);
            x->keys[idx] = succ;
            remove_internal(x->children[idx+1], succ, depth+1);
        } else {
            merge(x, idx, depth);
            remove_internal(x->children[idx], p, depth+1);
        }
    }

    Point get_predecessor(Node* x, int idx, int depth) {
        Node* cur = x->children[idx];
        while (!cur->is_leaf) {
            cur = cur->children.back();
            depth++;
        }
        return cur->keys.back();
    }

    Point get_successor(Node* x, int idx, int depth) {
        Node* cur = x->children[idx+1];
        while (!cur->is_leaf) {
            cur = cur->children.front();
            depth++;
        }
        return cur->keys.front();
    }

    void fill(Node* x, int idx, int depth) {
        if (idx != 0 && (int)x->children[idx-1]->keys.size() >= T) {
            borrow_from_prev(x, idx, depth);
        }
        else if (idx != (int)x->keys.size() && (int)x->children[idx+1]->keys.size() >= T) {
            borrow_from_next(x, idx, depth);
        }
        else {
            if (idx != (int)x->keys.size()) {
                merge(x, idx, depth);
            } else {
                merge(x, idx-1, depth);
            }
        }
    }

    void borrow_from_prev(Node* x, int idx, int depth) {
        Node* child = x->children[idx];
        Node* sibling = x->children[idx-1];

        child->keys.insert(child->keys.begin(), x->keys[idx-1]);
        if (!child->is_leaf) {
            child->children.insert(child->children.begin(), sibling->children.back());
            sibling->children.pop_back();
        }
        x->keys[idx-1] = sibling->keys.back();
        sibling->keys.pop_back();
    }

    void borrow_from_next(Node* x, int idx, int depth) {
        Node* child = x->children[idx];
        Node* sibling = x->children[idx+1];

        child->keys.push_back(x->keys[idx]);
        if (!child->is_leaf) {
            child->children.push_back(sibling->children.front());
            sibling->children.erase(sibling->children.begin());
        }

        x->keys[idx] = sibling->keys.front();
        sibling->keys.erase(sibling->keys.begin());
    }

    void merge(Node* x, int idx, int depth) {
        Node* child = x->children[idx];
        Node* sibling = x->children[idx+1];

        child->keys.push_back(x->keys[idx]);
        for (auto& k : sibling->keys) {
            child->keys.push_back(k);
        }
        if (!child->is_leaf) {
            for (auto c : sibling->children) {
                child->children.push_back(c);
            }
        }

        x->keys.erase(x->keys.begin()+idx);
        x->children.erase(x->children.begin()+idx+1);

        delete sibling;
    }
};

// Example usage
int main() {
    using Point2D = std::array<int, 2>;
    BKDTree<int, 2, 3> bkd;

    // Insert some 2D points
    bkd.insert({10, 5});
    bkd.insert({3, 4});
    bkd.insert({5, 6});
    bkd.insert({10, 20});
    bkd.insert({2, 1});
    bkd.insert({3, 9});
    bkd.insert({8, 7});
    bkd.insert({12, 3});

    std::cout << "Initial B-KD Tree:\n";
    bkd.print();

    std::cout << "Removing (3,4)...\n";
    bkd.remove({3,4});
    bkd.print();

    std::cout << "Searching (10,20): " << (bkd.search({10,20}) ? "Found\n" : "Not Found\n");
    std::cout << "Searching (4,4): " << (bkd.search({4,4}) ? "Found\n" : "Not Found\n");

    return 0;
}