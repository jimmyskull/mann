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

#include <iostream>  // TODO : remove
#include "../lib/util.h"

namespace mann {

namespace detail {

template <typename PointT, typename ValueT = typename PointT::value_type>
struct PointHelper {
  static_assert(sizeof(ValueT) == -1,
                "A Point must be either a vector or an array.");
};

template <typename ValueT, std::size_t Dim>
struct PointHelper<std::array<ValueT, Dim>, ValueT> {
  static std::array<ValueT, Dim> Create(std::size_t M) {
    if (Dim != M)
      std::runtime_error{"Inconsistent size for a fixed-length array."};
    return {};
  }

  using RefVector = std::vector<std::array<ValueT, Dim>>;
  using ObjVector = std::vector<std::array<ValueT, Dim>>;

  static RefVector ToRefVector(typename ObjVector::const_iterator first,
                               typename ObjVector::const_iterator last) {
    return RefVector(first, last);
  }
};

template <typename ValueT>
struct PointHelper<std::vector<ValueT>, ValueT> {
  static std::vector<ValueT> Create(std::size_t N) {
    return std::vector<ValueT>(N);
  }

  using RefVector =
      std::vector<std::reference_wrapper<const std::vector<ValueT>>>;
  using ObjVector = std::vector<std::vector<ValueT>>;

  static RefVector ToRefVector(typename ObjVector::const_iterator first,
                               typename ObjVector::const_iterator last) {
    RefVector result;
    result.reserve(std::distance(first, last));
    std::transform(first, last, std::back_inserter(result),
                   [](const auto& vec) { return std::cref(vec); });
    return result;
  }
};

}  // namespace detail

class Box {
 public:
  // Returns a pair<min, max> of the range of values in |dimension|
  template <typename RandomAccessIterator>
  static auto DimensionRange(RandomAccessIterator first,
                             RandomAccessIterator last, unsigned dimension) {
    auto r = std::minmax_element(
        first, last, [dimension](const auto& a, const auto& b) -> bool {
          return a[dimension] < b[dimension];
        });
    return std::make_pair(r.first->at(dimension), r.second->at(dimension));
  }

  // Returns vector<> with the length of each |box| edge
  template <typename T>
  static auto Lengths(const std::pair<T, T>& box) {
    std::vector<typename T::value_type> lengths;
    lengths.reserve(box.first.size());
    std::transform(box.first.begin(), box.first.end(), box.second.begin(),
                   std::back_inserter(lengths),
                   [](const auto& a, const auto& b) { return a + b; });
    return lengths;
  }

  // Returns a pair<lower left point, upper right point> that determines the
  // smallest box that fits a list of points.
  template <typename RandomAccessIterator>
  static auto Fit(RandomAccessIterator first, RandomAccessIterator last) {
    using Helper =
        detail::PointHelper<typename RandomAccessIterator::value_type>;

    auto kDim = first->size();
    auto lower_left = Helper::Create(kDim);
    auto upper_right = Helper::Create(kDim);

    for (decltype(kDim) dim = 0; dim < kDim; ++dim) {
      auto range = DimensionRange(first, last, dim);
      lower_left[dim] = range.first;
      upper_right[dim] = range.second;
    }
    return std::make_pair(lower_left, upper_right);
  }
};

template <typename PointsIteratorT, typename ValueT>
struct Hyperplane {
  unsigned dimension_index;
  ValueT threshold;
  PointsIteratorT middle;

  friend std::ostream& operator<<(std::ostream& os, const Hyperplane& hp) {
    os << "<dim=" << hp.dimension_index;
    os << ", threshold=" << hp.threshold;
    os << ", middle=" << *hp.middle << ">";
    return os;
  }
};

// Not thread safe
template <typename ValueT>
class SlidingMidpoint {
 public:
  SlidingMidpoint(ValueT epsilon = 1e-3) : kEpsilon(epsilon) {}

  template <typename RandomAccessIterator, typename BoundsT>
  void operator()(RandomAccessIterator first, RandomAccessIterator last,
                  const BoundsT& bounds,
                  Hyperplane<RandomAccessIterator, ValueT>& split) {
    dimensions_.resize(bounds.first.size());
    std::transform(
        bounds.first.begin(), bounds.first.end(), bounds.second.begin(),
        dimensions_.begin(), [&bounds](const auto& a, const auto& b) {
          return std::make_pair(std::abs(a - b),
                                std::distance(bounds.first.begin(), &a));
        });
    std::make_heap(
        dimensions_.begin(), dimensions_.end(),
        [](const auto& a, const auto& b) -> bool { return a.first < b.first; });

    const auto threshold = (1.0 - kEpsilon) * dimensions_.front().first;

    ValueT max_interval = 0.0;
    typename decltype(dimensions_)::value_type dimension;

    while (!dimensions_.empty()) {
      // Process the next largest dimension information
      auto dim = dimensions_.front();
      std::pop_heap(dimensions_.begin(), dimensions_.end());
      dimensions_.pop_back();

      if (dim.first >= threshold) {
        // std::cout << "Top:";
        // std::cout << '<' << dim.first << ", " << dim.second << '>' <<
        // std::endl;
        auto range = Box::DimensionRange(first, last, dim.second);
        const auto interval = std::abs(range.first - range.second);
        // std::cout << "Range: <" << range.first << ", " << range.second
        //          << "> = " << interval << std::endl;
        if (interval > max_interval) {
          max_interval = interval;
          split.dimension_index = dim.second;
          dimension = dim;
        }
      }
    }

    const auto min = bounds.first[split.dimension_index];
    const auto max = bounds.second[split.dimension_index];

    split.threshold = (min + max) / 2.0;
    const bool left_of_min = split.threshold < min;
    const bool right_of_max = split.threshold > max;
    split.threshold = std::max(split.threshold, min);
    split.threshold = std::min(split.threshold, max);

    // True if |p| is at the left-hand side of the hyperplane
    auto is_lhs = [&split](const auto& p) -> bool {
      return p[split.dimension_index] <= split.threshold;
    };

    const auto middle2 = std::partition(first, last, is_lhs);

    auto middle1 = middle2;
    while (first != middle1 && is_lhs(*(middle1 - 1))) {
      --middle1;
      // std::cerr << "Pos: " << std::distance(first, middle1) << std::endl;
    }

    const auto N = std::distance(first, last);
    const auto half = N / 2;

    unsigned middle1_pos = std::distance(first, middle1);
    unsigned middle2_pos = std::distance(first, middle2);
    unsigned points_on_lhs;

    if (left_of_min)
      points_on_lhs = 1;
    else if (right_of_max)
      points_on_lhs = N - 1;
    else if (middle1_pos > half)
      points_on_lhs = middle1_pos;
    else if (middle2_pos < half)
      points_on_lhs = middle2_pos;
    else
      points_on_lhs = half;

    split.middle = first + points_on_lhs;
  }

 private:
  const ValueT kEpsilon;
  // Pair of dimension lenght and the dimension index
  std::vector<std::pair<ValueT, unsigned>> dimensions_;
};

template <typename RandomAccessIterator>
class KDTree {
 public:
  using point_iterator = RandomAccessIterator;
  using point_type = typename point_iterator::value_type;
  using value_type = typename point_type::value_type;

 private:
  using Hyperplane = Hyperplane<point_iterator, value_type>;
  using Box = std::pair<point_type, point_type>;

  class Node {};

  // An internal node contains a hyperplane and two children
  class InternalNode : public Node {
   public:
    InternalNode(const Hyperplane& plane, const Box& box,
                 std::shared_ptr<Node> left, std::shared_ptr<Node> right)
        : Node(), plane_(plane), box_(box), left_(left), right_(right) {}

   private:
    const Hyperplane plane_;
    const Box box_;
    std::shared_ptr<Node> left_;
    std::shared_ptr<Node> right_;
  };

  class LeafNode : public Node {
   public:
    LeafNode(point_iterator begin, point_iterator end)
        : Node(), begin_(begin), end_(end) {}

    static auto Sentinel() {
      static std::shared_ptr<LeafNode> instance =
          std::make_shared<LeafNode>(point_iterator{}, point_iterator{});
      return instance;
    }

   private:
    point_iterator begin_;
    point_iterator end_;
  };

 public:
  KDTree(point_iterator first, point_iterator last, unsigned bucket_size = 1)
      : first_(first), last_(last), bucket_size_(bucket_size) {
    BuildTree();
  }

 private:
  void BuildTree() {
    auto box = mann::Box::Fit(first(), last());

    root_ = BuildNode(first(), last(), box);
  }

  std::shared_ptr<Node> BuildNode(point_iterator left, point_iterator right,
                                  Box& box) {
    const auto n = std::distance(left, right);
    if (n <= bucket_size()) {
      if (n <= 0) {
        std::cerr << "Zee end (leaf).\n";
        return LeafNode::Sentinel();
      } else {
        std::cerr << "Zee end (bucket).\n";
        return std::make_shared<LeafNode>(left, right);
      }
    }
    // Split node
    auto splitter = mann::SlidingMidpoint<value_type>();
    auto plane = Hyperplane();

    splitter(left, right, box, plane);

    std::swap(box.second[plane.dimension_index], plane.threshold);
    auto left_node = BuildNode(left, plane.middle, box);
    std::swap(box.second[plane.dimension_index], plane.threshold);

    std::swap(box.first[plane.dimension_index], plane.threshold);
    auto right_node = BuildNode(plane.middle, right, box);
    std::swap(box.first[plane.dimension_index], plane.threshold);

    return std::make_shared<InternalNode>(plane, box, left_node, right_node);
  }

  point_iterator first() { return first_; }
  point_iterator last() { return last_; }

  unsigned bucket_size() const { return bucket_size_; }

  point_iterator first_;
  point_iterator last_;
  const unsigned bucket_size_;
  std::shared_ptr<Node> root_;
};

}  // namespace mann

#endif  // MANN_H_
