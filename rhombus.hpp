#ifndef OOP_EXERCISE_06_RHOMBUS_HPP
#define OOP_EXERCISE_06_RHOMBUS_HPP

#include <cmath>
#include <iostream>

namespace lab {

  template <typename T>
  struct vertex {
    T x;
    T y;
    vertex(T _x = T(), T _y = T()) : x(_x), y(_y) {};
    vertex& operator=(const vertex& v) {
      x = v.x;
      y = v.y;
      return *this;
    }
    friend vertex getVector(const vertex& a, const vertex& b) {
      return { b.x - a.x, b.y - a.y };
    }
    friend double dot(const vertex& a, const vertex& b) {
      return a.x * b.x + a.y * b.y;
    }
    friend double dist(const vertex& a, const vertex& b) {
      return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
    }
    friend std::ostream& operator<<(std::ostream& os, const vertex& v) {
      os << '[' << v.x << ',' << v.y << ']';
      return os;
    }
    friend std::istream& operator>>(std::istream& is, vertex& v) {
      is >> v.x >> v.y;
      return is;
    }
  };
  template <typename T>
  class rhombus {
  public:
    friend std::ostream& operator<<(std::ostream& os, const rhombus& r) {
      os << "Rhombus: " << r.a << ' ' << r.b << ' ' << r.c << ' ' << r.c;
      return os;
    }
    friend std::istream& operator>>(std::istream& is, rhombus& r) {
      vertex<T> x, i, j;
      is >> x >> i >> j;
      if (!is.good()) {
        std::cerr << "Read error!\n";
        return is;
      }
      vertex<T> u(getVector(x, i));
      vertex<T> v(getVector(x, j));
      vertex<T> k(getVector(u, x));
      vertex<T> l(getVector(v, x));
      if (dot(getVector(i, k), getVector(j, l)) < eps) {
        std::cerr << "Not a rhombus!\n";
        return is;
      }
      r.a = i;
      r.b = j;
      r.c = k;
      r.d = l;
      return is;
    }
    double getArea() {
      return dist(a, c) * dist(b, d) / 2;
    }
  private:
    T eps = 1e-6;
    vertex<T> a, b, c, d;
  };
}

#endif //OOP_EXERCISE_05_RHOMBUS_HPP
