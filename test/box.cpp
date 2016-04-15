#include <catch.hpp>

#include <algorithm>

#include <mann.h>

#include "./test_input.h"

TEST_CASE("Box", "[Box]") {

   auto points = test::Simple::points();

   auto box = mann::SmallestEnclosingBounds(points.begin(), points.end());

//    CHECK(box.lower_left() == (Point2d{{-0.970662, -0.942415}}));
//    CHECK(box.upper_right() == (Point2d{{0.927417, 0.986146}}));
//
//    auto sides = box.DimensionLengths();
//    auto max = std::max_element(sides.begin(), sides.end());
//    CHECK((max - sides.begin()) == 1);
//    CHECK(*max == Approx(1.928561));
}
