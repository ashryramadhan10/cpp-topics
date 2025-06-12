#include <iostream>
#include <vector>
#include <memory>
#include <memory_resource>

// A custom resource that spies on its upstream allocator
class SpyResource : public std::pmr::memory_resource {
public:
    SpyResource(std::pmr::memory_resource* upstream) : upstream_resource(upstream) {}

private:
    // This gets called when our resource needs to allocate memory
    void* do_allocate(size_t bytes, size_t alignment) override {
        std::cout << "--> Spy: Allocating " << bytes << " bytes from upstream! <--" << std::endl;
        return upstream_resource->allocate(bytes, alignment);
    }

    void do_deallocate(void* p, size_t bytes, size_t alignment) override {
        upstream_resource->deallocate(p, bytes, alignment);
    }

    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override {
        return this == &other;
    }

    std::pmr::memory_resource* upstream_resource;
};

int main() {

    // Print the size of a unique_ptr
    std::cout << "sizeof(std::unique_ptr<int>): "
              << sizeof(std::unique_ptr<int>) << " bytes" << std::endl;

    // Print the size of a shared_ptr
    std::cout << "sizeof(std::shared_ptr<int>): "
              << sizeof(std::shared_ptr<int>) << " bytes" << std::endl;

    // Print the size of a weak_ptr
    std::cout << "sizeof(std::weak_ptr<int>):   "
              << sizeof(std::weak_ptr<int>) << " bytes" << std::endl;

    // For comparison, the size of a raw pointer
    std::cout << "sizeof(int*):                 "
              << sizeof(int*) << " bytes" << std::endl;

    // Our spy will watch the default heap allocator
    SpyResource spy{std::pmr::get_default_resource()};

    // We tell our pool to use the spy as its upstream fallback
    std::pmr::monotonic_buffer_resource pool_resource{1024, &spy};

    std::pmr::vector<std::shared_ptr<int>> vec{&pool_resource};

    std::cout << "Starting to fill the vector. First 64 elements should be silent..." << std::endl;

    // This loop will trigger the fallback
    for (int i = 0; i < 128; i++) {
        if (i == 64) {
            std::cout << "\nBuffer is likely full. The next allocation should trigger the spy...\n" << std::endl;
        }
        vec.emplace_back(std::make_shared<int>(i));
    }

    std::cout << "\nVector successfully holds " << vec.size() << " elements." << std::endl;
}