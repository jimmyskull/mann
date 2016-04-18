#include <catch.hpp>

#include <array>
#include <iostream>

#include <mann.h>

#include "./test_input.h"

TEST_CASE("Fit", "[Box]") {
  auto points = test::Simple::points();

  auto box = mann::Box::Fit(points.begin(), points.end());

  using Point = decltype(points)::value_type;
  CHECK(box.first == Point({{-0.970662, -0.942415}}));  // lower left bound
  CHECK(box.second == Point({{0.927417, 0.986146}}));   // upper right bound
}

TEST_CASE("SlidingMidpoint", "[Box]") {
  auto points = test::SixDimensions::points();
  auto box = mann::Box::Fit(points.begin(), points.end());

  using Point = typename decltype(points)::value_type;
  using T = typename Point::value_type;

  auto splitter = mann::SlidingMidpoint<T>();
  auto plane = mann::Hyperplane<T>();

  splitter(points.begin(), points.end(), box, plane);


}


