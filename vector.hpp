#ifndef OOP_EXERCISE_06_VECTOR_HPP
#define OOP_EXERCISE_06_VECTOR_HPP

#include <exception>
#include <stdexcept>
#include <algorithm>
#include <functional>
#include <memory>

#include "allocator.hpp"

namespace lab {

  template <typename T, typename Allocator = lab::allocator<T>>
  class Vector {
  public:
    struct VItem {
      using allocator_type = typename Allocator::template rebind<VItem>::other;

      static allocator_type& get_allocator()
      {
        static allocator_type allocator;
        return allocator;
      }

      T v;

      VItem() = default;
      VItem(T _v) : v(_v) {}
      ~VItem() = default;
      VItem(const VItem& i) {
        v = i.v;
      }
      void* operator new(size_t size) {
        return get_allocator().allocate(1);
      }

      void operator delete(void* p) {
        get_allocator().deallocate((VItem*)p, 1);
      }

      void* operator new[](size_t size) {
        return get_allocator().allocate(size / sizeof(VItem));
      }

        void operator delete[](void* p, size_t size) {
        get_allocator().deallocate((VItem*)p, size / sizeof(VItem));
      }
    };

    using index_type = size_t;
    using value_type = VItem;
    using item_type = T;

    Vector() : _length(0), _capacity(BASE_CAP) {
      //VItem* i = new VItem;
      _data.reset(new VItem[BASE_CAP]);
    }
    Vector(const Vector&) = delete;
    Vector(Vector&&) = delete;
    ~Vector() = default;

    class VectorIterator {
    public:
      using difference_type = Vector::index_type;
      using value_type = Vector::item_type;
      using reference = Vector::item_type&;
      using pointer = Vector::item_type*;
      using iterator_category = std::forward_iterator_tag;

      VectorIterator(Vector& vector, difference_type index) : _vector(vector), _index(index) {}

      VectorIterator& operator++() {
        ++_index;
        return *this;
      }

      reference operator*() {
        return _vector[_index];
      }

      pointer operator->() {
        return &_vector[_index];
      }

      bool operator!=(const VectorIterator& other) {
        if (_index != other._index) {
          return true;
        }
        if (&_vector != &(other._vector)) {
          return true;
        }
        return false;
      }

      bool operator==(const VectorIterator& other) {
        if (_index != other._index) {
          return false;
        }
        if (&_vector != &(other._vector)) {
          return false;
        }
        return true;
      }

    private:
      Vector& _vector;
      difference_type _index;

      friend class Vector;
    };

    index_type& length() {
      return _length;
    };
    [[nodiscard]] const index_type& length() const {
      return _length;
    };

    T& at(index_type index) {
      if (index >= 0 && index < _capacity) {
        return _data[index].v;
      }
      else {
        std::cout << "out of range" << std::endl;
        std::terminate();
        //throw std::out_of_range("Out of bounds");
      }
    };
    const T& at(index_type index) const {
      if (index >= 0 && index < _capacity) {
        return _data[index].v;
      }
      else {
        std::cout << "out of range" << std::endl;
        std::terminate();
        //throw std::out_of_range("Out of bounds");
      }
    };

    T& operator[](index_type index) noexcept {
      return _data[index].v;
    };
    const T& operator[](index_type index) const noexcept {
      return _data[index].v;
    };

    index_type size() {
      return _length;
    }

    VectorIterator begin() {
      return VectorIterator(*this, 0);
    }

    VectorIterator end() {
      return VectorIterator(*this, size());
    }

    void reserve(index_type capacity) {
      if (capacity < _capacity) {
        return;
      }
      else {
        while (capacity < _capacity) {
          index_type new_capacity = capacity * Vector::GROWTH_RATE;
          if (new_capacity > capacity) {
            capacity = new_capacity;
          }
          else {
            break;
          }
        }
        auto old_data = std::move(_data);
        //try {
        _data.reset(new VItem[_capacity]);
        //}
        //catch (std::bad_alloc& ex) {
        //    std::cerr << "ERROR: " << ex.what() << std::endl;
        //}
        for (index_type index = 0; index < _capacity; ++index) {
          _data[index] = old_data[index];
        }
        _capacity = capacity;
      }
    }

    void push_back(T& value) {
      if (_length >= _capacity) {
        reserve(_capacity * 2);
      }
      if (_length >= _capacity) {
        std::cout << "max capacity reached" << std::endl;
        std::terminate();
        //throw std::logic_error("max capacity reached");
      }
      _data[_length].v = value.v;
      ++_length;
    }

    void pop_back(T& value) {
      if (_length > 0) {
        --_length;
        value = _data[_length];
      }
      else {
        std::cout << "pop on empty vector" << std::endl;
        std::terminate();
        //throw std::logic_error("pop on empty vector");
      }
    }

    VectorIterator erase(VectorIterator iter) {
      if (iter._index < 0 || iter._index > size()) {
        std::cout << "bad insert" << std::endl;
        std::terminate();
        //throw std::logic_error("bad insert");
      }
      index_type i = 0;
      for (; i < _length; ++i) {
        if (i == iter._index) {
          break;
        }
      }
      for (; i < _length; ++i) {
        _data[i] = _data[i + 1];
      }
      --_length;
      return iter;
    }

    VectorIterator insert(VectorIterator iter, const T& value) {
      if (iter._index < 0 || iter._index > size()) {
        std::cout << "bad insert" << std::endl;
        std::terminate();
        //throw std::logic_error("bad insert");
      }
      if (_length == _capacity) {
        reserve(_capacity * 2);
      }
      for (index_type i = _length; i > 0; --i) {
        _data[i] = _data[i - 1];
        if (i == iter._index) {
          _data[i].v = value;
          break;
        }
      }
      if (iter._index == 0) {
        _data[0].v = value;
      }
      ++_length;
      return iter;
    }

  private:
    static index_type const BASE_CAP = 4;
    static index_type const GROWTH_RATE = 2;
    index_type _length;
    index_type _capacity;
    std::unique_ptr<value_type[]> _data;
  };

}

#endif //OOP_EXERCISE_05_VECTOR_HPP