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
        if (this->priority < _task_record.priority) {
            return true;
        } else {
            return false;
        }
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
        while (i <= this->last_index) {
            int swap = i;

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
    while (i <= this->last_index) {
        int swap = i;

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
    while (i <= this->last_index) {
        int swap = i;

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

    std::vector<TaskRecord> tasks = {
        TaskRecord(3, "Task A"),
        TaskRecord(2, "Task B"),
        TaskRecord(1, "Task C"),
        TaskRecord(5, "Task D"),
        TaskRecord(7, "Task E"),
    };

    for (auto& task : tasks) {
        heap.heapInsert(task);
    }

    // move all data from heap to results
    std::vector<std::unique_ptr<TaskRecord>> results;
    for (int i = 0; i < tasks.size(); ++i) {
        results.emplace_back(heap.heapRemoveMax());
    }

    for (auto& task : results) {
        std::cout << task->priority << " " << task->task_name << "\n";
    }

    return 0;
}