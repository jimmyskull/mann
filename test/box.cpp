#include <catch.hpp>

#include <algorithm>

#include <mann.h>

#include "./test_input.h"

TEST_CASE("Box", "[Box]") {
  using Point2d = std::array<double, 2>;
  using PointVec = std::vector<Point2d>;
  using Box = mann::Box<PointVec>;

  auto points = test::Simple::points();

  Box box(points);

  CHECK(box.lower_left() == (Point2d{{-0.970662, -0.942415}}));
  CHECK(box.upper_right() == (Point2d{{0.927417, 0.986146}}));

  auto sides = box.DimensionLengths();
  auto max = std::max_element(sides.begin(), sides.end());
  CHECK((max - sides.begin()) == 1);
  CHECK(*max == Approx(1.928561));
}
