#include <cmath>
#include <iostream>
#include <utility>
#include <vector>
#include <memory>
#include <unordered_map>
#define ARRAYSIZE 100

// Heap properties:
// Addition: add new node as leaf node, then bubble up based on its prority
// By design, heaps are balanced binary trees
// Removing: swapping the first element (root) with the last element (last index)
// To restore the tree, we have to walkdown the tree, restoring the heap property at every level

class TaskRecord {
public:
    unsigned int priority;
    std::string task_name;

    TaskRecord(unsigned int priority, std::string task_name) : priority(priority), task_name(std::move(task_name)) {}
    ~TaskRecord() = default;

    bool operator < (const TaskRecord& _task_record) const {
        return this->priority < _task_record.priority;
    }
};

template <class T>
class Heap {
public:
    std::vector<std::unique_ptr<T>> arr;
    int last_index;

    std::unordered_map<std::string, int> taskNameToIndex;

    Heap();
    ~Heap();

    void heapInsert(const T& value);
    std::unique_ptr<T> heapRemoveMax();
    std::unique_ptr<T> heapRemoveMin();
    void updateValue(const int& index, const T& value);

private:
    void swapNodes(int i, int j);
};

template <class T>
Heap<T>::Heap() {
    this->arr.resize(ARRAYSIZE);
    this->last_index = 0;
}

template <class T>
Heap<T>::~Heap() {
    this->arr.clear();
    this->last_index = 0;
}

template<class T>
void Heap<T>::swapNodes(int i, int j) {
    std::swap(this->arr[i], this->arr[j]);
    this->taskNameToIndex[this->arr[i]->task_name] = i;
    this->taskNameToIndex[this->arr[j]->task_name] = j;
}

template<class T>
void Heap<T>::heapInsert(const T& value) {
    if (this->last_index == static_cast<int>(this->arr.size()) - 1) {
        this->arr.resize(this->arr.size() * 2);
    }

    this->last_index++;
    this->arr[this->last_index] = std::make_unique<T>(value);
    taskNameToIndex[value.task_name] = this->last_index;

    int current_index = this->last_index;
    int parent_index = std::floor(current_index / 2);

    while (parent_index >= 1 && (*(this->arr[parent_index]) < *(this->arr[current_index]))) {
        swapNodes(parent_index, current_index);
        current_index = parent_index;
        parent_index = std::floor(current_index / 2);
    }
}

template<class T>
void Heap<T>::updateValue(const int &index, const T &value) {
    if (index < 1 || index > last_index) {
        std::cerr << "Invalid index\n";
        return;
    }

    std::unique_ptr<T> old_value = std::move(this->arr[index]);
    this->taskNameToIndex.erase(old_value->task_name);

    this->arr[index] = std::make_unique<T>(value);
    this->taskNameToIndex[value.task_name] = index;

    if (*old_value < *this->arr[index]) {
        // Bubble up
        int current_index = index;
        int parent_index = std::floor(current_index / 2);
        while (parent_index >= 1 && (*(this->arr[parent_index]) < *(this->arr[current_index]))) {
            swapNodes(parent_index, current_index);
            current_index = parent_index;
            parent_index = std::floor(current_index / 2);
        }
    } else {
        // Bubble down
        int i = index;
        int swap = i;

        while (i <= this->last_index) {

            if (2 * i <= this->last_index && (*(this->arr[swap]) < *(this->arr[2 * i]))) {
                swap = 2 * i;
            }

            if (2 * i + 1 <= this->last_index && (*(this->arr[swap]) < *(this->arr[2 * i + 1]))) {
                swap = 2 * i + 1;
            }

            if (i != swap) {
                swapNodes(i, swap);
                i = swap;
            } else {
                break;
            }
        }
    }
}

template<class T>
std::unique_ptr<T> Heap<T>::heapRemoveMax() {
    if (this->last_index == 0) {
        return nullptr;
    }

    if (this->last_index == 1) {
        return std::move(this->arr[1]);
    }

    // previous max priority
    std::unique_ptr<T> removed = std::move(this->arr[1]);
    this->taskNameToIndex.erase(removed->task_name);

    this->arr[1] = std::move(this->arr[this->last_index]);
    this->taskNameToIndex[this->arr[1]->task_name] = 1;

    // decrease last_index
    this->last_index--;

    int i = 1;
    int swap = i;

    while (i <= this->last_index) {

        // search the largest priority between parent with left child
        if (2 * i <= this->last_index && (*(this->arr[swap]) < *(this->arr[2 * i]))) {
            swap = 2 * i;
        }

        // then will be check between left child and right child if swap has changed to 2 * i
        if (2 * i + 1 <= this->last_index && (*(this->arr[swap]) < *(this->arr[2 * i + 1]))) {
            swap = 2 * i + 1;
        }

        // swap it between i and swap
        if (i != swap) {
            swapNodes(i, swap);

            // i now as the swap
            i = swap;
        } else {
            break;
        }
    }

    std::cout << "Deleted: " << removed->task_name << " (Priority: " << removed->priority << ")\n";
    return std::move(removed);
}

template<class T>
std::unique_ptr<T> Heap<T>::heapRemoveMin() {
    if (this->last_index == 0) {
        std::cerr << "Heap is empty.\n";
        return nullptr;
    }

    if (this->last_index == 1) {
        return std::move(this->arr[1]);
    }

    // search the min index
    int min_index = 1;
    for (int i = 2; i <= this->last_index; ++i) {
        if (*(this->arr[i]) < *(this->arr[min_index])) {
            min_index = i;
        }
    }

    std::unique_ptr<T> removed = std::move(this->arr[min_index]);
    this->taskNameToIndex.erase(removed->task_name);

    this->arr[min_index] = std::move(this->arr[this->last_index]);
    this->taskNameToIndex[this->arr[min_index]->task_name] = min_index;
    this->last_index--;

    int i = min_index;
    int swap = i;

    while (i <= this->last_index) {

        if (2 * i <= this->last_index && (*(this->arr[swap]) < *(this->arr[2 * i]))) {
            swap = 2 * i;
        }

        if (2 * i + 1 <= this->last_index && (*(this->arr[swap]) < *(this->arr[2 * i + 1]))) {
            swap = 2 * i + 1;
        }

        if (i != swap) {
            swapNodes(i, swap);
            i = swap;
        } else {
            break;
        }
    }

    std::cout << "Deleted Min: " << removed->task_name << " (Priority: " << removed->priority << ")\n";
    return std::move(removed);
}

int main(int argc, char* argv[]) {
    Heap<TaskRecord> heap;

    // Insert some tasks
    heap.heapInsert(TaskRecord(10, "Task A"));
    heap.heapInsert(TaskRecord(5, "Task B"));
    heap.heapInsert(TaskRecord(20, "Task C"));
    heap.heapInsert(TaskRecord(1, "Task D"));

    std::cout << "\nRemoving Max:\n";
    heap.heapRemoveMax();  // Should delete "Task C" (priority 20)

    std::cout << "\nUpdating Priority:\n";
    // Now, safely get the index of "Task D" from the map
    auto it = heap.taskNameToIndex.find("Task D");
    if (it != heap.taskNameToIndex.end()) {
        int idx = it->second;
        heap.updateValue(idx, TaskRecord(30, "Task D"));  // Boost priority of Task D
    } else {
        std::cerr << "Task D not found in heap!\n";
    }

    std::cout << "\nRemoving Max (After Update):\n";
    heap.heapRemoveMax();  // Should delete "Task D" (priority 30)

    std::cout << "\nInserting More:\n";
    heap.heapInsert(TaskRecord(3, "Task E"));
    heap.heapInsert(TaskRecord(8, "Task F"));
    heap.heapInsert(TaskRecord(15, "Task G"));

    std::cout << "\nRemoving Min:\n";
    heap.heapRemoveMin();  // Should delete "Task E" (priority 3)

    std::cout << "\nRemaining Removals:\n";
    heap.heapRemoveMax();  // Should delete "Task G"
    heap.heapRemoveMax();  // Should delete "Task F"
    heap.heapRemoveMax();  // Should delete "Task B"

    return 0;
}