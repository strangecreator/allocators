#include "stackallocator.h"

struct A {
  int a;
  double b;
};

int main() {
  // initialization
  static const int size = 100'000;
  StackStorage<size> storage;
  StackAllocator<A, size> alloc(storage);
  using allocator_traits = std::allocator_traits<StackAllocator<A, size>>;
  // use
  A* ptr = allocator_traits::allocate(alloc, 1);
  allocator_traits::construct(alloc, ptr, 3, 4.7);
  allocator_traits::destroy(alloc, ptr);
  allocator_traits::deallocate(alloc, ptr, 1);
}
