// Copyright © 2016  Paulo Urio and Filipe Verri
// Copyright © 1997–2010  University of Maryland and Sunil Arya and David Mount.
// All Rights Reserved.
#ifndef MANN_H_
#define MANN_H_

#include <array>
#include <istream>
#include <memory>
#include <numeric>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

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
  static const unsigned dimension = Dim;

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

  const ValueT& at(std::size_t index) const { return elements_[index]; }

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

template<typename T>
class Array {
 public:

};

namespace detail {

class OrthogonalRectangle {
 public:
  void SmallestEnclosingRectangle();
};

class KDTreeNode {};

}  // namespace detail

template <typename PointsArrayT>
class KDTree {
 private:
  using Node = detail::KDTreeNode;

 public:
  KDTree(const PointsArrayT& points, unsigned bucket_size = 1)
      : points_(points), bucket_size_(bucket_size) {
    if (points_.empty()) return;
  }

 private:
  void SkeletonTree() {
  }

  unsigned dim() const { return points_.dimension; }
  unsigned bucket_size() const { return bucket_size_; }

  const PointsArrayT& points_;
  const unsigned bucket_size_;
  std::unique_ptr<Node> root_;
  std::weak_ptr<Node> bounding_box_lower_left_;
  std::weak_ptr<Node> bounding_box_upper_right_;
};

}  // namespace mann

#endif  // MANN_H_
