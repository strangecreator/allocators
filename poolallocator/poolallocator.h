#pragma once
#include "poolstorage.h"

template<typename T>
class PoolAllocator {
  public:
    using value_type = T;
    static auto constexpr alignment = alignof(T);
    using StorageType = PoolStorage;

  private:
    StorageType* storage = nullptr;

  public:
    PoolAllocator() = delete;
    // constructors
    PoolAllocator(StorageType& storage) noexcept :
        storage(&storage) {}

    template<typename U>
    PoolAllocator(const PoolAllocator<U>& alloc) noexcept : 
        storage(alloc.storage) {};

    // assignment operators
    template<typename U>
    PoolAllocator& operator=(const PoolAllocator<U>& alloc) noexcept {
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
        using other = PoolAllocator<U>;
    };

    // comparison operators declaration
    template<typename T1, typename T2>
    friend
    inline bool operator==(
    const PoolAllocator<T1>& alloc1,
    const PoolAllocator<T2>& alloc2) noexcept;

    template<typename U> friend class PoolAllocator;
};

// comparison operators definition
template<typename T1, typename T2>
inline bool operator==(
    const PoolAllocator<T1>& alloc1,
    const PoolAllocator<T2>& alloc2) noexcept {
        return alloc1.storage == alloc2.storage;
}

template<typename T1, typename T2>
inline bool operator!=(
    const PoolAllocator<T1>& alloc1,
    const PoolAllocator<T2>& alloc2) noexcept {
        return !(alloc1 == alloc2);
}
