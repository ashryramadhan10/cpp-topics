#include <vector>
#include <algorithm>
#include <memory>
#include <limits>
#include <queue>
#include <cmath>

template<typename T, size_t K>
struct Point {
    std::array<T, K> coords;
    
    Point() = default;
    Point(const std::array<T, K>& coordinates) : coords(coordinates) {}
    
    T& operator[](size_t i) { return coords[i]; }
    const T& operator[](size_t i) const { return coords[i]; }
    
    // Calculate Euclidean distance between two points
    T distance(const Point& other) const {
        T sum = 0;
        for (size_t i = 0; i < K; ++i) {
            T diff = coords[i] - other.coords[i];
            sum += diff * diff;
        }
        return std::sqrt(sum);
    }
};

template<typename T, size_t K>
class KDTree {
private:
    struct Node {
        Point<T, K> point;
        size_t axis;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
        
        Node(const Point<T, K>& p, size_t ax) 
            : point(p), axis(ax), left(nullptr), right(nullptr) {}
    };
    
    std::unique_ptr<Node> root;
    
    // Helper function to build the tree recursively
    std::unique_ptr<Node> buildTree(std::vector<Point<T, K>>& points, 
                                  size_t start, size_t end, 
                                  size_t depth) {
        if (start >= end) return nullptr;
        
        size_t axis = depth % K;
        size_t mid = (start + end) / 2;
        
        // Sort points based on the current axis
        std::nth_element(
            points.begin() + start,
            points.begin() + mid,
            points.begin() + end,
            [axis](const Point<T, K>& a, const Point<T, K>& b) {
                return a[axis] < b[axis];
            }
        );
        
        auto node = std::make_unique<Node>(points[mid], axis);
        node->left = buildTree(points, start, mid, depth + 1);
        node->right = buildTree(points, mid + 1, end, depth + 1);
        
        return node;
    }
    
    // Helper function for k nearest neighbors search
    void kNearestNeighborsHelper(const Node* node,
                                const Point<T, K>& target,
                                std::priority_queue<std::pair<T, Point<T, K>>>& pq,
                                size_t k) const {
        if (!node) return;
        
        // Calculate distance to current point
        T distance = node->point.distance(target);
        
        // Add current point to priority queue if it's closer than the kth neighbor
        if (pq.size() < k) {
            pq.push({distance, node->point});
        } else if (distance < pq.top().first) {
            pq.pop();
            pq.push({distance, node->point});
        }
        
        // Calculate distance to splitting plane
        T axisDist = target[node->axis] - node->point[node->axis];
        
        // Recursively search the closer subtree
        if (axisDist < 0) {
            kNearestNeighborsHelper(node->left.get(), target, pq, k);
            // Check if we need to search the other subtree
            if (pq.size() < k || std::abs(axisDist) < pq.top().first) {
                kNearestNeighborsHelper(node->right.get(), target, pq, k);
            }
        } else {
            kNearestNeighborsHelper(node->right.get(), target, pq, k);
            // Check if we need to search the other subtree
            if (pq.size() < k || std::abs(axisDist) < pq.top().first) {
                kNearestNeighborsHelper(node->left.get(), target, pq, k);
            }
        }
    }
    
    // Helper function for range search
    void rangeSearchHelper(const Node* node,
                          const Point<T, K>& min,
                          const Point<T, K>& max,
                          std::vector<Point<T, K>>& result) const {
        if (!node) return;
        
        // Check if current point is within range
        bool inRange = true;
        for (size_t i = 0; i < K; ++i) {
            if (node->point[i] < min[i] || node->point[i] > max[i]) {
                inRange = false;
                break;
            }
        }
        
        if (inRange) {
            result.push_back(node->point);
        }
        
        // Check if we need to search left subtree
        if (node->left && min[node->axis] <= node->point[node->axis]) {
            rangeSearchHelper(node->left.get(), min, max, result);
        }
        
        // Check if we need to search right subtree
        if (node->right && max[node->axis] >= node->point[node->axis]) {
            rangeSearchHelper(node->right.get(), min, max, result);
        }
    }

public:
    KDTree() = default;
    
    // Build tree from vector of points
    void build(std::vector<Point<T, K>> points) {
        root = buildTree(points, 0, points.size(), 0);
    }
    
    // Insert a single point
    void insert(const Point<T, K>& point) {
        if (!root) {
            root = std::make_unique<Node>(point, 0);
            return;
        }
        
        Node* current = root.get();
        size_t depth = 0;
        
        while (true) {
            size_t axis = depth % K;
            
            if (point[axis] < current->point[axis]) {
                if (!current->left) {
                    current->left = std::make_unique<Node>(point, axis);
                    break;
                }
                current = current->left.get();
            } else {
                if (!current->right) {
                    current->right = std::make_unique<Node>(point, axis);
                    break;
                }
                current = current->right.get();
            }
            
            depth++;
        }
    }
    
    // Find k nearest neighbors
    std::vector<Point<T, K>> kNearestNeighbors(const Point<T, K>& target, 
                                              size_t k) const {
        std::priority_queue<std::pair<T, Point<T, K>>> pq;
        kNearestNeighborsHelper(root.get(), target, pq, k);
        
        std::vector<Point<T, K>> result;
        while (!pq.empty()) {
            result.push_back(pq.top().second);
            pq.pop();
        }
        
        std::reverse(result.begin(), result.end());
        return result;
    }
    
    // Range search
    std::vector<Point<T, K>> rangeSearch(const Point<T, K>& min, 
                                        const Point<T, K>& max) const {
        std::vector<Point<T, K>> result;
        rangeSearchHelper(root.get(), min, max, result);
        return result;
    }
};