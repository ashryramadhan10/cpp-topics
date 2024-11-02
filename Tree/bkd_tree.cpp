#include <vector>
#include <algorithm>
#include <memory>
#include <limits>
#include <queue>

// Point class to represent k-dimensional points
template<size_t K>
struct Point {
    std::array<double, K> coords;
    
    Point() = default;
    Point(const std::array<double, K>& coordinates) : coords(coordinates) {}
    
    double& operator[](size_t i) { return coords[i]; }
    const double& operator[](size_t i) const { return coords[i]; }
};

template<size_t K, size_t B>
class BKDTree {
private:
    struct Node {
        bool isLeaf;
        std::vector<Point<K>> points;  // Used in leaf nodes
        std::vector<Node*> children;    // Used in internal nodes
        std::vector<double> splitValues; // Split values for internal nodes
        size_t splitDimension;          // Current split dimension
        
        Node(bool leaf = true) : isLeaf(leaf), splitDimension(0) {}
        
        ~Node() {
            for (auto* child : children) {
                delete child;
            }
        }
    };
    
    Node* root;
    static constexpr size_t MIN_ENTRIES = B / 2;
    static constexpr size_t MAX_ENTRIES = B;
    
    // Helper function to determine split dimension
    size_t chooseSplitDimension(const std::vector<Point<K>>& points) {
        std::array<double, K> variances{};
        
        // Calculate variance for each dimension
        for (size_t dim = 0; dim < K; ++dim) {
            double sum = 0, sumSq = 0;
            for (const auto& point : points) {
                sum += point[dim];
                sumSq += point[dim] * point[dim];
            }
            double mean = sum / points.size();
            variances[dim] = (sumSq / points.size()) - (mean * mean);
        }
        
        // Return dimension with maximum variance
        return std::max_element(variances.begin(), variances.end()) - variances.begin();
    }
    
    // Split node into multiple nodes based on B-tree properties
    std::vector<Node*> splitNode(const std::vector<Point<K>>& points, size_t dimension) {
        std::vector<Point<K>> sortedPoints = points;
        std::sort(sortedPoints.begin(), sortedPoints.end(),
                 [dimension](const Point<K>& a, const Point<K>& b) {
                     return a[dimension] < b[dimension];
                 });
        
        std::vector<Node*> nodes;
        size_t pointsPerNode = (sortedPoints.size() + MAX_ENTRIES - 1) / MAX_ENTRIES;
        
        for (size_t i = 0; i < sortedPoints.size(); i += pointsPerNode) {
            Node* node = new Node(true);
            size_t end = std::min(i + pointsPerNode, sortedPoints.size());
            node->points.insert(node->points.end(), 
                              sortedPoints.begin() + i, 
                              sortedPoints.begin() + end);
            nodes.push_back(node);
        }
        
        return nodes;
    }
    
    // Insert a point into a leaf node
    void insertIntoLeaf(Node* node, const Point<K>& point) {
        node->points.push_back(point);
        
        if (node->points.size() > MAX_ENTRIES) {
            // Split leaf node
            size_t dim = chooseSplitDimension(node->points);
            std::vector<Node*> newNodes = splitNode(node->points, dim);
            
            // Create new internal node
            Node* newInternal = new Node(false);
            newInternal->children = newNodes;
            newInternal->splitDimension = dim;
            
            // Calculate split values
            for (size_t i = 0; i < newNodes.size() - 1; ++i) {
                double splitValue = newNodes[i]->points.back()[dim];
                newInternal->splitValues.push_back(splitValue);
            }
            
            // Replace current node with new internal node
            *node = *newInternal;
            delete newInternal;
        }
    }
    
    // Find the appropriate child node for a point
    Node* findChild(Node* node, const Point<K>& point) {
        if (node->isLeaf) return node;
        
        size_t dim = node->splitDimension;
        double value = point[dim];
        
        for (size_t i = 0; i < node->splitValues.size(); ++i) {
            if (value <= node->splitValues[i]) {
                return node->children[i];
            }
        }
        
        return node->children.back();
    }
    
    // Range search helper function
    void rangeSearchHelper(Node* node, const Point<K>& min, const Point<K>& max,
                          std::vector<Point<K>>& result) {
        if (!node) return;
        
        if (node->isLeaf) {
            for (const auto& point : node->points) {
                bool inRange = true;
                for (size_t dim = 0; dim < K; ++dim) {
                    if (point[dim] < min[dim] || point[dim] > max[dim]) {
                        inRange = false;
                        break;
                    }
                }
                if (inRange) {
                    result.push_back(point);
                }
            }
        } else {
            size_t dim = node->splitDimension;
            for (size_t i = 0; i < node->children.size(); ++i) {
                if (i < node->splitValues.size()) {
                    if (max[dim] >= node->splitValues[i]) {
                        rangeSearchHelper(node->children[i], min, max, result);
                    }
                } else {
                    if (min[dim] <= node->splitValues.back()) {
                        rangeSearchHelper(node->children[i], min, max, result);
                    }
                }
            }
        }
    }

public:
    BKDTree() : root(new Node(true)) {}
    
    ~BKDTree() {
        delete root;
    }
    
    // Insert a point into the tree
    void insert(const Point<K>& point) {
        Node* node = root;
        while (!node->isLeaf) {
            node = findChild(node, point);
        }
        insertIntoLeaf(node, point);
    }
    
    // Range search
    std::vector<Point<K>> rangeSearch(const Point<K>& min, const Point<K>& max) {
        std::vector<Point<K>> result;
        rangeSearchHelper(root, min, max, result);
        return result;
    }
    
    // Nearest neighbor search
    Point<K> nearestNeighbor(const Point<K>& query) {
        double bestDist = std::numeric_limits<double>::max();
        Point<K> nearest;
        
        std::function<void(Node*)> search = [&](Node* node) {
            if (!node) return;
            
            if (node->isLeaf) {
                for (const auto& point : node->points) {
                    double dist = 0;
                    for (size_t dim = 0; dim < K; ++dim) {
                        double diff = point[dim] - query[dim];
                        dist += diff * diff;
                    }
                    if (dist < bestDist) {
                        bestDist = dist;
                        nearest = point;
                    }
                }
            } else {
                Node* bestChild = findChild(node, query);
                search(bestChild);
                
                // Check other children if they might contain a closer point
                size_t dim = node->splitDimension;
                for (size_t i = 0; i < node->children.size(); ++i) {
                    if (node->children[i] != bestChild) {
                        double splitDist = 0;
                        if (i < node->splitValues.size()) {
                            splitDist = std::abs(query[dim] - node->splitValues[i]);
                        }
                        if (splitDist * splitDist < bestDist) {
                            search(node->children[i]);
                        }
                    }
                }
            }
        };
        
        search(root);
        return nearest;
    }
};