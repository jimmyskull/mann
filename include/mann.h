// Copyright © 2016  Paulo Urio and Filipe Verri
#ifndef MANN_H_
#define MANN_H_

#include <array>
#include <istream>
#include <sstream>
#include <initializer_list>
#include <string>
#include <type_traits>

namespace mann {

template <typename R, typename...>
struct fst {
  typedef R type;
};

template <typename ValueT = double, unsigned Dim = 2>
class Point {
 private:
  std::array<ValueT, Dim> elements_;

 public:
  ~Point() = default;

  Point() = default;
  Point(const Point&) = default;
  Point(Point&&) = default;
  auto operator=(Point&) -> Point& = default;
  auto operator=(Point && ) -> Point& = default;

  explicit Point(std::istream& is) { is >> *this; }

  template <
      typename... T,
      typename = typename fst<void, typename std::enable_if<std::is_convertible<
                                        T, ValueT>::value>::type...>::type>
  Point(T&&... args)
      : elements_{{std::forward<T>(args)...}} {
    static_assert(sizeof...(T) == Dim, "Invalid number of dimensions.");
  }

  const ValueT& operator[](std::size_t index) const { return elements_[index]; }

  friend std::istream& operator>>(std::istream& is, Point& p) {
    std::string line;
    if (!getline(is, line)) {
      std::runtime_error("Empty stream");
    }
    std::istringstream iss(line);
    for (auto& e : p.elements_) {
      iss >> e;
    }
    return is;
  }

  friend std::ostream& operator<<(std::ostream& os, Point& p) {
    bool beginning = true;
    for (auto& e : p.elements_) {
      os << (beginning ? '(' : ' ');
      os << e;
      beginning = false;
    }
    os << ")";
    return os;
  }
};

}  // namespace mann

#endif  // MANN_H_
