#include "poolallocator.h"

struct A {
  int a;
  double b;
};

int main() {
  // initialization
  PoolStorage storage(100'000);
  PoolAllocator<A> alloc(storage);
  using allocator_traits = std::allocator_traits<PoolAllocator<A>>;
  // use
  A* ptr = allocator_traits::allocate(alloc, 1);
  allocator_traits::construct(alloc, ptr, 3, 4.7);
  allocator_traits::destroy(alloc, ptr);
  allocator_traits::deallocate(alloc, ptr, 1);
}
