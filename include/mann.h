// Copyright © 2016  Paulo Urio and Filipe Verri
// Copyright © 1997–2010  University of Maryland and Sunil Arya and David Mount.
// All Rights Reserved.
#ifndef MANN_H_
#define MANN_H_

#include <algorithm>
#include <array>
#include <istream>
#include <memory>
#include <numeric>
#include <sstream>
#include <string>
#include <type_traits>
#include <tuple>
#include <utility>
#include <vector>

namespace mann {

template <typename R, typename...>
struct fst {
  typedef R type;
};

template <typename ValueT = double, unsigned Dim = 2>
class Point {
 private:
  using point_type = std::array<ValueT, Dim>;
  std::unique_ptr<point_type> elements_ = std::make_unique<point_type>();

 public:
  using const_iterator = typename point_type::const_iterator;
  using value_type = ValueT;

  static const unsigned dimension = Dim;

  ~Point() = default;

  Point(){};
  Point(const Point&) = default;
  Point(Point&&) = default;
  auto operator=(Point&) -> Point& = default;
  auto operator=(Point && ) -> Point& = default;

  explicit Point(std::istream& is) { is >> *this; }

  explicit Point(std::initializer_list<ValueT> args) {
    if (args.size() != Dim)
      throw std::runtime_error("Number of elements and dimensions must match.");
    std::copy(args.begin(), args.end(), elements_->begin());
  }

  ValueT& operator[](std::size_t index) { return elements_->operator[](index); }

  const ValueT& operator[](std::size_t index) const {
    return elements_->operator[](index);
  }

  const ValueT& at(std::size_t index) const { return elements_->at(index); }

  typename point_type::size_type size() const { return elements_->size(); }

  bool operator==(const Point& p) const {
    return std::equal(begin(), end(), p.begin());
  }

  const_iterator begin() const { return elements_->begin(); }
  const_iterator end() const { return elements_->end(); }

  friend std::istream& operator>>(std::istream& is, Point& p) {
    std::string line;
    if (!getline(is, line)) {
      throw std::runtime_error("Empty stream");
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
class Rectangle {
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

  std::tuple<unsigned, typename PointT::value_type>
  longest_side() const {
    using ValueT = typename PointT::value_type;
    auto diff = [&](unsigned dim) -> ValueT {
      return std::abs(lower_left()[dim] - upper_right()[dim]);
    };
    ValueT largest = diff(0);
    unsigned largest_dim = 0;

    for (unsigned d = 0; d < lower_left().dimension; ++d) {
      ValueT candidate = diff(d);
      if (candidate > largest) {
        largest = candidate;
        largest_dim = d;
      }
    }
    return std::make_tuple(largest_dim, largest);
  }

  const PointT& lower_left() const { return lower_left_; }
  const PointT& upper_right() const { return upper_right_; }

 private:
  PointT lower_left_;
  PointT upper_right_;
};

class KDTreeNode {};

class KDTreeNodeLeaf : public KDTreeNode {};

class KDTreeNodeInternal : public KDTreeNode {};

}  // namespace detail

// Space split policies
namespace split {

template <typename ValueT>
struct Split {
  unsigned dimension_index;
  ValueT threshold_value;
  unsigned number_of_points_in_left_side;
};

template <typename PointsT, typename BoundsT, typename ValueT>
void SlidingMidpoint(const PointsT& points, const BoundsT& bounds,
                     ValueT* result, double epsilon = 1e-3) {
//  std::minmax_element(
}

}  // namespace split

template <typename PointsArrayT>
class KDTree {
 private:
  using Node = detail::KDTreeNode;
  using Rectangle = detail::Rectangle<typename PointsArrayT::value_type>;

 public:
  KDTree(const PointsArrayT& points, unsigned bucket_size = 1)
      : points_(points), bucket_size_(bucket_size) {
    BuildTree();
  }

 private:
  void BuildTree() {
    Rectangle bounds;
    bounds.SmallestEnclosingRect(points_);
    root_ = BuildNode();
  }

  std::unique_ptr<Node> BuildNode() {}

  unsigned dim() const { return points_.dimension; }
  unsigned bucket_size() const { return bucket_size_; }

  const PointsArrayT& points_;
  const unsigned bucket_size_;
  std::unique_ptr<Node> root_;
};

}  // namespace mann

#endif  // MANN_H_
