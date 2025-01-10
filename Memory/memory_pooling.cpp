#include <iostream>
#include <memory>
#include <vector>

template <typename T>
class PoolAllocator {
private:
    T* pool;              // Raw pointer to the beginning of the pool
    std::size_t pool_size; // Total number of T elements in the pool
    std::size_t used;     // How many T elements are currently 'in use'

public:
    using value_type = T;

    // Constructor: allocate a pool for `pool_size` Ts
    explicit PoolAllocator(std::size_t pool_size)
        : pool(nullptr), pool_size(pool_size), used(0)
    {
        // Allocate raw memory for the pool
        pool = static_cast<T*>(::operator new(pool_size * sizeof(T)));
    }

    // Prevent copying (simple pool allocator typically doesn't support copy semantics)
    PoolAllocator(const PoolAllocator&) = delete;
    PoolAllocator& operator=(const PoolAllocator&) = delete;

    // Allocate n Ts
    T* allocate(std::size_t n) {
        // If we have enough space left in the pool
        if (used + n <= pool_size) {
            T* result = pool + used;
            used += n;
            return result;
        }
        // Otherwise, fall back to the global operator new
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    // Deallocate n Ts
    void deallocate(T* p, std::size_t n) {
        // Check if p is within the pool range
        if (p >= pool && p < pool + pool_size) {
            // If this pointer is exactly the last allocated block,
            // then we can reclaim it by adjusting `used`.
            // In pointer arithmetic: p + n must be the current top (pool + used).
            if (p + n == pool + used) {
                used -= n;
            }
            // Otherwise, we do nothing. (No partial "freeing" in a naive stack approach.)
        } else {
            // If not in the pool, it was allocated on the heap -> free it
            ::operator delete(p);
        }
    }

    // Destructor frees the entire pool
    ~PoolAllocator() {
        ::operator delete(pool);
    }
};

int main() {
    PoolAllocator<int> alloc{10};

    // 1. Allocate 5 ints (within the pool)
    int* p1 = alloc.allocate(5);
    for (int i = 0; i < 5; ++i) {
        p1[i] = i + 100;
    }

    // 2. Allocate another 5 ints (fills the pool completely)
    int* p2 = alloc.allocate(5);
    for (int i = 0; i < 5; ++i) {
        p2[i] = i + 200;
    }

    // Print out both blocks
    std::cout << "Block p1 (5 ints): ";
    for (int i = 0; i < 5; ++i) {
        std::cout << p1[i] << " ";
    }
    std::cout << "\nBlock p2 (5 ints): ";
    for (int i = 0; i < 5; ++i) {
        std::cout << p2[i] << " ";
    }
    std::cout << "\n";

    // 3. Deallocate p2 first (the last allocated block)
    //    This should free up space for another 5 ints in the pool
    alloc.deallocate(p2, 5);

    // 4. Now allocate 5 more ints again from the pool
    int* p3 = alloc.allocate(5);
    for (int i = 0; i < 5; ++i) {
        p3[i] = i + 300;
    }

    std::cout << "Block p3 (5 ints after reuse): ";
    for (int i = 0; i < 5; ++i) {
        std::cout << p3[i] << " ";
    }
    std::cout << "\n";

    // 5. Deallocate p3, then p1
    //    p3 is still the last allocated block, so it will be freed. 
    //    But p1 won't be freed because it's not the "top" of the stack anymore.
    alloc.deallocate(p3, 5); // Reclaims memory in the pool
    alloc.deallocate(p1, 5); // Does nothing for p1 in a strict LIFO approach

    return 0;
}