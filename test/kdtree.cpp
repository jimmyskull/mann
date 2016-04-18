
#include <catch.hpp>

#include <mann.h>

#include "./test_input.h"

TEST_CASE("KDTree", "[KDTree]") {

  auto points = test::Repeated::points();
  using KDTree = mann::KDTree<decltype(points)::iterator>;
  auto tree = KDTree(points.begin(), points.end());
}
