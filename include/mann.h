// Copyright © 2016  Paulo Urio and Filipe Verri
#ifndef MANN_H_
#define MANN_H_

#include <array>
#include <istream>
#include <sstream>
#include <initializer_list>
#include <string>

namespace mann {

template <typename ValueT = double, unsigned Dim = 2>
class Point {
 private:
  std::array<ValueT, Dim> elements_;

 public:
  template <typename... T>
  Point(T&&... args)
      : elements_{{std::forward<T>(args)...}} {}

  const ValueT& operator[](std::size_t index) const { return elements_[index]; }

  friend std::istream& operator>>(std::istream& is, Point& p) {
    std::string line;
    if (!getline(is, line)) {
      // error
    }
    std::istringstream iss(line);
    for (auto& e : p.elements_) iss >> e;
    return is;
  }
};

}  // namespace mann

#endif  // MANN_H_
