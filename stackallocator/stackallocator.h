#pragma once
#include "stackstorage.h"

template<typename T, size_t N>
class StackAllocator {
  public:
    using value_type = T;
    static auto constexpr size = N;
    static auto constexpr alignment = alignof(T);
    using StorageType = StackStorage<size>;

  private:
    StorageType* storage = nullptr;

  public:
    StackAllocator() = delete;
    // constructors
    StackAllocator(StorageType& storage) noexcept :
        storage(&storage) {}

    template<typename U>
    StackAllocator(const StackAllocator<U, size>& alloc) noexcept : 
        storage(alloc.storage) {};

    // assignment operators
    template<typename U>
    StackAllocator& operator=(const StackAllocator<U, size>& alloc) noexcept {
        storage = alloc.storage;
    }

    // mandatory functions
    T* allocate(size_t n) {
        return reinterpret_cast<T*>(storage->allocate(n * sizeof(T), alignment));
    }

    void deallocate(T* ptr, size_t n) noexcept {
        storage->deallocate(reinterpret_cast<char*>(ptr), n * sizeof(T));
    }

    template<typename U>
    struct rebind {
        using other = StackAllocator<U, size>;
    };

    // comparison operators declaration
    template<typename T1, size_t N1, typename T2, size_t N2>
    friend
    inline bool operator==(
    const StackAllocator<T1, N1>& alloc1,
    const StackAllocator<T2, N2>& alloc2) noexcept;

    template<typename U, size_t M> friend class StackAllocator;
};

// comparison operators definition
template<typename T1, size_t N1, typename T2, size_t N2>
inline bool operator==(
    const StackAllocator<T1, N1>& alloc1,
    const StackAllocator<T2, N2>& alloc2) noexcept {
        return alloc1.storage == alloc2.storage && N1 == N2;
}

template<typename T1, size_t N1, typename T2, size_t N2>
inline bool operator!=(
    const StackAllocator<T1, N1>& alloc1,
    const StackAllocator<T2, N2>& alloc2) noexcept {
        return !(alloc1 == alloc2);
}
