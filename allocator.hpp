#ifndef OOP_EXERCISE_06_ALLOCATOR_HPP
#define OOP_EXERCISE_06_ALLOCATOR_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

namespace lab {

  template<class T, size_t BLOCK_COUNT = 1024>
  class allocator {
  private:
    const size_t BLOCK_SIZE = sizeof(T);
    std::unique_ptr<uint8_t[]> _buffer;
    std::vector<void*> _blocks;
    uint64_t _free_count;

  public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using size_type = std::size_t;

    allocator() {
      static_assert(BLOCK_COUNT > 0, "BLOCK_COUNT must be positive!\n");
      std::cout << "allocator_ctor" << std::endl;
      _free_count = BLOCK_COUNT;
      _buffer = std::make_unique<uint8_t[]>(BLOCK_SIZE * BLOCK_COUNT);
      _blocks = std::vector<void*>(BLOCK_COUNT);
      std::generate(_blocks.begin(), _blocks.end(), [=, addr = _buffer.get()]() mutable {
        auto offset = addr;
        addr += BLOCK_SIZE;
        return static_cast<void*>(offset);
      });
    }

    ~allocator() {
      std::cout << "allocator_dtor" << std::endl;
    }

    template<typename U>
    struct rebind {
      using other = allocator<U, BLOCK_COUNT>;
    };

    T* allocate(size_t n) {
      std::cout << "allocator_allocate" << std::endl;
      T* result = nullptr;
      if (n && _free_count > 0) {
        while (n && _free_count) {
          --_free_count;
          --n;
        }
        if (n) {
          std::cout << "bad alloc" << std::endl;
          std::terminate();
        }
      }
      else {
        std::cout << "bad alloc" << std::endl;
        std::terminate();
      }
      result = static_cast<pointer>(_blocks[--_free_count]);
      return result;
    }

    void deallocate(pointer p, size_t n) {
      std::cout << "allocator_deallocate" << std::endl;
      while (n && _free_count < BLOCK_COUNT) {
        --n;
        _blocks[_free_count++] = p;
      }

    }

    template<typename U, typename ...Args>
    void construct(U* p, Args &&...args) {
      new (p) U(std::forward<Args>(args)...);
    }

    void destroy(pointer p) {
      p->~T();
    }

  };
}

#endif