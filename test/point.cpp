
#include <catch.hpp>

#include <iostream>
#include <sstream>
#include <vector>

#include <mann.h>

TEST_CASE("Point init", "[Point]") {
  using mann::Point;

  auto p = Point<double, 2>{1.0, 2.0};
  CHECK(p[0] == Approx(1.0));
  CHECK(p[1] == Approx(2.0));
}

TEST_CASE("Point from stream", "[Point]") {
  using mann::Point;

  std::istringstream input{"1.5 3.2\n4 2"};

  Point<double, 2> p;
  input >> p;
  CHECK(p[0] == Approx(1.5));
  CHECK(p[1] == Approx(3.2));

  Point<double, 2> p2;
  input >> p2;
  CHECK(p2[0] == Approx(4.0));
  CHECK(p2[1] == Approx(2.0));
}

TEST_CASE("Array of Points", "[Point]") {
  using mann::Point;

  std::vector<Point<double, 2>> points;

  std::istringstream input{"1.5 3.2\n4 2\n0 1\n1 0\n"};

  while (!input.eof())
    if (input.peek() != EOF) points.emplace_back(input);

  CHECK(points.size() == 4);
}
