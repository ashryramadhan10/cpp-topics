//
// Created by ashry on 6/9/2025.
//
#include <cmath>
#include <iostream>
#include <vector>

class GridPoint {
public:
    float x;
    float y;
    GridPoint* next;

    GridPoint(): x(0), y(0), next(nullptr) {}
    GridPoint(float x, float y): x(x), y(y), next(nullptr) {}
    ~GridPoint() = default;
};

class Grid {
private:
    int num_x_bins;
    int num_y_bins;
    float x_start;
    float x_end;
    float y_start;
    float y_end;
    float x_bin_width;
    float y_bin_width;
    std::vector<std::vector<GridPoint*>> grid_points;

public:

    Grid() {
        num_x_bins = 10;
        num_y_bins = 10;
        x_start = 0;
        x_end = 10;
        y_start = 0;
        y_end = 10;
        x_bin_width = (x_end - x_start) / num_x_bins;
        y_bin_width = (y_end - y_start) / num_y_bins;

        // CORRECTION: Correctly initialize the 2D vector.
        grid_points.resize(num_x_bins);
        for (int i = 0; i < num_x_bins; ++i) {
            grid_points[i].resize(num_y_bins, nullptr);
        }
    }

    // Destructor: Safely cleans up all dynamically allocated GridPoints.
    ~Grid() {
        // SAFETY FIX: This now iterates through every bin in the grid,
        // walks each linked list, and deletes each node one-by-one.
        // This prevents memory leaks.
        for (int i = 0; i < num_x_bins; ++i) {
            for (int j = 0; j < num_y_bins; ++j) {
                GridPoint* current = grid_points[i][j];
                while (current != nullptr) {
                    GridPoint* to_delete = current;
                    current = current->next;
                    to_delete->next = nullptr;
                    delete to_delete;
                }
            }
        }
    }

    bool approx_equal(float x1, float y1, float x2, float y2, float threshold) {
        if (std::abs(x1 - x2) > threshold) {
            return false;
        }

        if (std::abs(y1 - y2) > threshold) {
            return false;
        }

        return true;
    }

    // used for bins pruning
    float euclid_distance(float x1, float y1, float x2, float y2) {
        return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
    }

    bool gridInsert(Grid* grid, float x, float y) {
        int x_bin = std::floor((x - grid->x_start) / grid->x_bin_width);
        int y_bin = std::floor((y - grid->y_start) / grid->y_bin_width);

        if (x_bin < 0 || x_bin >= grid->num_x_bins || y_bin < 0 || y_bin >= grid->num_y_bins) {
            return false;
        }

        // Correct, leak-free insertion logic:
        // 1. Create the new node first.
        GridPoint* new_point = new GridPoint(x, y);

        // 2. Link the new node to the current head of the list.
        new_point->next = grid->grid_points[x_bin][y_bin];

        // 3. Update the grid's head pointer to be the new node.
        grid->grid_points[x_bin][y_bin] = new_point;

        return true;
    }

    bool gridDelete(Grid* grid, float x, float y) {
        int x_bin = std::floor((x - grid->x_start) / grid->x_bin_width);
        int y_bin = std::floor((y - grid->y_start) / grid->y_bin_width);

        if (x_bin < 0 || x_bin >= grid->num_x_bins) {
            return false;
        } else if (y_bin < 0 || y_bin >= grid->num_y_bins) {
            return false;
        }

        if (grid->grid_points[x_bin][y_bin] == nullptr) {
            return false;
        }

        GridPoint* current_point = grid->grid_points[x_bin][y_bin];
        GridPoint* previous_point = nullptr;
        while (current_point != nullptr) {
            if (approx_equal(x, y, current_point->x, current_point->y, 1.5)) {
                if (previous_point == nullptr) {
                    grid->grid_points[x_bin][y_bin] = current_point->next;
                } else {
                    previous_point->next = current_point->next;
                }
                delete current_point;
                return true;
            }
            previous_point = current_point;
            current_point = current_point->next;
        }
        return false;
    }

    // ================================== Pruning Bins ==================================
    float minDistToBin(Grid* grid, int xbin, int ybin, float x, float y) {

        // If the bin is out of bounds, return infinity so it is never considered.
        if (xbin < 0 || xbin >= grid->num_x_bins) {
            return std::numeric_limits<float>::infinity();
        }

        if (ybin < 0 || ybin >= grid->num_y_bins) {
            return std::numeric_limits<float>::infinity();
        }

        // xbin and ybin are always a floored number
        float x_min = grid->x_start + xbin * grid->x_bin_width;
        float x_max = grid->x_start + (xbin + 1) * grid->x_bin_width;
        float x_dist = 0;

        if (x < x_min) {
            x_dist = x_min - x;
        }

        if (x > x_max) {
            x_dist = x - x_max;
        }

        float y_min = grid->y_start + ybin * grid->y_bin_width;
        float y_max = grid->y_start + (ybin + 1) * grid->y_bin_width;
        float y_dist = 0;

        if (y < y_min) {
            y_dist = y_min - y;
        }

        if (y > y_max) {
            y_dist = y - y_max;
        }

        return std::sqrt((x_dist * x_dist) + (y_dist * y_dist));
    }

    // Linear scan from xbin = 0, ybin = 0, to xbin = num_x_bins, ybin = num_y_bins
    GridPoint* gridLinearScanNN(Grid* grid, float x, float y) {
        float best_dist = 99999.0F;
        GridPoint* best_candidate = nullptr;

        int xbin = 0;
        while (xbin < grid->num_x_bins) {

            // always restart the ybin for new xbin
            int ybin = 0;
            while (ybin < grid->num_y_bins) {

                if (minDistToBin(grid, xbin, ybin, x, y) < best_dist) {

                    // check everypoint in the bin
                    GridPoint* current_point = grid->grid_points[xbin][ybin];
                    while (current_point != nullptr) {

                        float dist = euclid_distance(x, y, current_point->x, current_point->y);

                        if (dist < best_dist) {
                            best_dist = dist;
                            best_candidate = current_point;
                        }

                        current_point = current_point->next;
                    }
                }
                ybin = ybin + 1;
            }
            xbin = xbin + 1;
        }
        return best_candidate;
    }

    GridPoint* gridCheckBin(Grid* g, int xbin, int ybin, float x, float y, float threshold) {
        if (xbin < 0 || xbin >= g->num_x_bins) {
            return nullptr;
        }

        if (ybin < 0 || ybin >= g->num_y_bins) {
            return nullptr;
        }

        GridPoint* best_candidate = nullptr;
        float best_dist = threshold;
        GridPoint* current_point = g->grid_points[xbin][ybin];
        while (current_point != nullptr) {
            float dist = euclid_distance(x, y, current_point->x, current_point->y);
            if (dist < best_dist) {
                best_dist = dist;
                best_candidate = current_point;
            }
            current_point = current_point->next;
        }
        return best_candidate;
    }

    GridPoint* gridSearchExpanding(Grid* g, float x, float y) {
        float best_dist = 99999.0F;
        GridPoint* best_candidate = nullptr;

        // find the starting x and y bins for our search
        // add some safety checking and clip the value of xbin and ybin
        int xbin = std::floor((x - g->x_start) / g->x_bin_width);
        if (xbin < 0) {
            xbin = 0;
        }
        if (xbin >= g->num_x_bins) {
            xbin = g->num_x_bins - 1;
        }

        int ybin = std::floor((y - g->y_start) / g->y_bin_width);
        if (ybin < 0) {
            ybin = 0;
        }
        if (ybin >= g->num_y_bins) {
            ybin = g->num_y_bins - 1;
        }

        int steps = 0;
        bool explore = true;
        while (explore) {
            explore = false;

            int xoff = -steps;
            while (xoff <= steps) {
                int yoff = steps - std::abs(xoff);

                if (minDistToBin(g, xbin + xoff, ybin - yoff, x, y) < best_dist) {
                    GridPoint* point = gridCheckBin(g, xbin + xoff, ybin - yoff, x, y, best_dist);
                    if (point != nullptr) {
                        best_dist = euclid_distance(x, y, point->x, point->y);
                        best_candidate = point;
                    }
                    explore = true;
                }

                if (minDistToBin(g, xbin + xoff, ybin + yoff, x, y) < best_dist && yoff != 0) {
                    GridPoint* point = gridCheckBin(g, xbin + xoff, ybin + yoff, x, y, best_dist);
                    if (point != nullptr) {
                        best_dist = euclid_distance(x, y, point->x, point->y);
                        best_candidate = point;
                    }
                    explore = true;
                }

                xoff = xoff + 1;
            }
            steps = steps + 1;
        }
        return best_candidate;
    }
};

int main() {
    std::cout << "## Grid Nearest Neighbor Demo ##" << std::endl;

    // 1. Create a Grid object on the stack.
    // Its destructor will automatically be called at the end of main,
    // cleaning up any remaining points.
    Grid my_grid;
    std::cout << "\n[INFO] Grid created." << std::endl;

    // --- DEMONSTRATE INSERTION ---
    std::cout << "\n## Testing Insertion ##" << std::endl;
    my_grid.gridInsert(&my_grid, 2.5f, 3.5f);
    std::cout << "[INSERT] Point (2.5, 3.5) added." << std::endl;

    my_grid.gridInsert(&my_grid, 8.1f, 8.2f);
    std::cout << "[INSERT] Point (8.1, 8.2) added." << std::endl;

    my_grid.gridInsert(&my_grid, 2.8f, 3.1f);
    std::cout << "[INSERT] Point (2.8, 3.1) added." << std::endl;

    my_grid.gridInsert(&my_grid, 5.5f, 5.5f);
    std::cout << "[INSERT] Point (5.5, 5.5) added." << std::endl;

    // --- DEMONSTRATE SEARCHING ---
    std::cout << "\n## Testing Search Algorithms ##" << std::endl;
    float search_x = 2.6f;
    float search_y = 3.2f;
    std::cout << "\nSearching for the nearest neighbor to (" << search_x << ", " << search_y << ")..." << std::endl;

    // Test the expanding search method
    GridPoint* found_expanding = my_grid.gridSearchExpanding(&my_grid, search_x, search_y);
    if (found_expanding != nullptr) {
        std::cout << "[Expanding Search] Found nearest point: (" << found_expanding->x << ", " << found_expanding->y << ")" << std::endl;
    } else {
        std::cout << "[Expanding Search] No points found in the grid." << std::endl;
    }

    // Test the linear scan method (should yield the same result)
    GridPoint* found_linear = my_grid.gridLinearScanNN(&my_grid, search_x, search_y);
    if (found_linear != nullptr) {
        std::cout << "[Linear Scan]      Found nearest point: (" << found_linear->x << ", " << found_linear->y << ")" << std::endl;
    } else {
        std::cout << "[Linear Scan]      No points found in the grid." << std::endl;
    }


    // --- DEMONSTRATE DELETION ---
    std::cout << "\n## Testing Deletion ##" << std::endl;
    float delete_x = 8.1f;
    float delete_y = 8.2f;
    std::cout << "\nAttempting to delete point (" << delete_x << ", " << delete_y << ")..." << std::endl;

    if (my_grid.gridDelete(&my_grid, delete_x, delete_y)) {
        std::cout << "[SUCCESS] Point was successfully deleted." << std::endl;
    } else {
        std::cout << "[FAILURE] Point could not be found to delete." << std::endl;
    }

    // Prove deletion by trying to delete the same point again
    std::cout << "Attempting to delete the same point again..." << std::endl;
     if (my_grid.gridDelete(&my_grid, delete_x, delete_y)) {
        std::cout << "[SUCCESS] Point was successfully deleted." << std::endl;
    } else {
        std::cout << "[FAILURE] Point could not be found, as expected." << std::endl;
    }

    // Prove deletion by searching near the deleted point
    std::cout << "Searching near the deleted point (8.0, 8.0)... The result should now be (5.5, 5.5)." << std::endl;
    GridPoint* found_after_delete = my_grid.gridSearchExpanding(&my_grid, 8.0f, 8.0f);
    if (found_after_delete != nullptr) {
        std::cout << "[Search After Delete] Found nearest point: (" << found_after_delete->x << ", " << found_after_delete->y << ")" << std::endl;
    } else {
        std::cout << "[Search After Delete] No points found." << std::endl;
    }

    std::cout << "\n[INFO] Demo finished. Grid destructor will now clean up remaining memory." << std::endl;

    return 0;
}