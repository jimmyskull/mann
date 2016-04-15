// Copyright © 2016  Paulo Urio and Filipe Verri
// Copyright © 1997–2010  University of Maryland and Sunil Arya and David Mount.
// All Rights Reserved.
#ifndef MANN_H_
#define MANN_H_

#include <algorithm>
#include <array>
#include <istream>
#include <iterator>
#include <memory>
#include <numeric>
#include <sstream>
#include <string>
#include <type_traits>
#include <tuple>
#include <utility>
#include <vector>

namespace mann {

// Orthotope or hyperrectangle
template <typename PointsT>
class Box {
 private:
  using points_type = PointsT;
  using point_type = typename PointsT::value_type;
  using value_type = typename point_type::value_type;

 public:
  Box(const points_type& points) : points_(points) {
    SmallestEnclosingBounds();
  }

  auto DimensionLengths() const {
    std::vector<value_type> result;
    result.reserve(lower_left().size());
    std::transform(lower_left().begin(), lower_left().end(),
                   upper_right().begin(), std::back_inserter(result),
                   [&](const auto& a, const auto& b) -> value_type {
                     return std::abs(a - b);
                   });
    return result;
  }

  const point_type& lower_left() const { return lower_left_; }
  const point_type& upper_right() const { return upper_right_; }

 private:
  void SmallestEnclosingBounds() {
    auto kDim = points_[0].size();
    for (decltype(kDim) dim = 0; dim < kDim; ++dim) UpdateDimensionBounds(dim);
  }

  void UpdateDimensionBounds(unsigned dim) {
    auto r = std::minmax_element(points_.begin(), points_.end(),
                                 [dim](const auto& a, const auto& b) -> bool {
                                   return a[dim] < b[dim];
                                 });
    lower_left_[dim] = r.first->at(dim);
    upper_right_[dim] = r.second->at(dim);
  }

 private:
  point_type lower_left_;
  point_type upper_right_;
  const points_type& points_;
};

class KDTreeNode {};

class KDTreeNodeLeaf : public KDTreeNode {};

class KDTreeNodeInternal : public KDTreeNode {};

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
  using Node = KDTreeNode;

 public:
  KDTree(const PointsArrayT& points, unsigned bucket_size = 1)
      : points_(points), bucket_size_(bucket_size) {
    BuildTree();
  }

 private:
  void BuildTree() {
    // Rectangle bounds;
    // bounds.SmallestEnclosingRect(points_);
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
