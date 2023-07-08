#pragma once
#include <cstddef>
#include <stdexcept>
#include <memory>

template<size_t N>
class StackStorage {
  public:
    static auto constexpr size = N;

  private:
    alignas(alignof(std::max_align_t)) char buffer[size];
    size_t index = 0; // index to a free cell

    char* get_pointer() {
        return &buffer[index];
    }

  public:
    StackStorage() = default;

    StackStorage(const StackStorage&) = delete;
    StackStorage& operator=(const StackStorage&) = delete;

    void* allocate(size_t n, size_t alignment = alignof(std::max_align_t)) {
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
