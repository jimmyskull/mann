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
#include <utility>
#include <vector>

#include <iostream>

namespace mann {

template <typename R, typename...>
struct fst {
  typedef R type;
};

template <typename ValueT = double, unsigned Dim = 2>
class Point {
 private:
  using PointType = std::array<ValueT, Dim>;
  std::unique_ptr<PointType> elements_ = std::make_unique<PointType>();

 public:
  static const unsigned dimension = Dim;

  ~Point() = default;

  Point(){};
  Point(const Point&) = default;
  Point(Point&&) = default;
  auto operator=(Point&) -> Point& = default;
  auto operator=(Point && ) -> Point& = default;

  explicit Point(std::istream& is) { is >> *this; }

  ValueT& operator[](std::size_t index) { return elements_->operator[](index); }

  const ValueT& operator[](std::size_t index) const {
    return elements_->operator[](index);
  }

  const ValueT& at(std::size_t index) const { return elements_->at(index); }

  typename PointType::size_type size() const { return elements_->size(); }

  friend std::istream& operator>>(std::istream& is, Point& p) {
    std::string line;
    if (!getline(is, line)) {
      std::runtime_error("Empty stream");
    }
    std::istringstream iss(line);
    for (auto& e : *p.elements_) {
      iss >> e;
    }
    return is;
  }

  friend std::ostream& operator<<(std::ostream& os, Point& p) {
    bool beginning = true;
    for (auto& e : *p.elements_) {
      os << (beginning ? '(' : ' ');
      os << e;
      beginning = false;
    }
    os << ")";
    return os;
  }
};

namespace detail {

template <typename PointT>
class OrthoRect {
 public:
  template <typename ArrayT>
  void SmallestEnclosingRect(const ArrayT& arr) {
    if (arr.empty()) return;
    auto dim = arr[0].size();
    for (decltype(dim) i = 0; i < dim; ++i) {
      auto r = std::minmax_element(
          arr.begin(), arr.end(),
          [i](const auto& a, const auto& b) -> bool { return a[i] < b[i]; });
      lower_left_[i] = r.first->at(i);
      upper_right_[i] = r.second->at(i);
    }
  }

 private:
  PointT lower_left_;
  PointT upper_right_;
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
    SkeletonTree();
  }

 private:
  void SkeletonTree() {
    bounds_.SmallestEnclosingRect(points_);
  }

  unsigned dim() const { return points_.dimension; }
  unsigned bucket_size() const { return bucket_size_; }

  const PointsArrayT& points_;
  const unsigned bucket_size_;
  std::unique_ptr<Node> root_;
  detail::OrthoRect<typename PointsArrayT::value_type> bounds_;
};

}  // namespace mann

#endif  // MANN_H_
