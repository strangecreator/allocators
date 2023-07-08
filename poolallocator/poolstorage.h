#pragma once
#include <cstddef>
#include <stdexcept>
#include <memory>

class PoolStorage {
    static const size_t constexpr KDefaultAlignment = alignof(std::max_align_t);
    static const size_t constexpr kDefaultSize = 100'000;

    template <typename U>
    static U* allocate_memory(size_t size, std::align_val_t alignment = std::align_val_t(KDefaultAlignment)) {
        return static_cast<U*>(new (alignment) char[size * sizeof(U)]);
    }

  private:
    size_t size = kDefaultSize;
    size_t index = 0; // index to a free cell
    char* pool = nullptr;

    char* get_pointer() {
        return pool + index;
    }

  public:
    PoolStorage() : pool(allocate_memory<char>(size)) {};
    PoolStorage(size_t size) : size(size), pool(allocate_memory<char>(size)) {}

    PoolStorage(const PoolStorage&) = delete;
    PoolStorage& operator=(const PoolStorage&) = delete;

    ~PoolStorage() {
        delete[] pool;
    }

    void* allocate(size_t n, size_t alignment = KDefaultAlignment) {
        void* ptr = get_pointer();
        size_t size_remained = size - index;
        if (std::align(alignment, n, ptr, size_remained)) {
            index += (reinterpret_cast<char*>(ptr) - get_pointer()) + n;
        } else {
            throw std::runtime_error("Memory in storage is over or it's a problem of std::align.");
        }
        return ptr;
    }

    void deallocate(char*, size_t) const noexcept {}
};
