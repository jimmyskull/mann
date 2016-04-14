#include <catch.hpp>

#include <vector>

#include <mann.h>

static std::istringstream input_points(
    R"(-0.297462	0.176102
0.565538	-0.361496
0.909313	-0.182785
0.920712	0.478408
0.167682	0.0499836
0.305223	-0.0805835
0.114973	0.882453
0.742916	0.16376
0.0724605	-0.826775
0.690960	-0.559284
0.188485	-0.643934
0.749427	-0.942415
-0.970662	-0.223466
0.916110	0.879597
0.927417	-0.382593
-0.711327	0.278713
-0.519172	0.986146
0.135338	0.924588
-0.0837537	0.61687
0.0520465	0.896306
)");

TEST_CASE("Rectangle", "[Rectangle]") {
  using Point2d = mann::Point<double, 2>;
  using Rectangle = mann::detail::Rectangle<Point2d>;

  std::vector<Point2d> points;

  while (!input_points.eof())
    if (input_points.peek() != EOF) points.emplace_back(input_points);

  Rectangle rectangle;
  rectangle.SmallestEnclosingRect(points);

  CHECK(rectangle.lower_left() == (Point2d{{-0.970662, -0.942415}}));
  CHECK(rectangle.upper_right() == (Point2d{{0.927417, 0.986146}}));

  unsigned dim;
  double size;
  std::tie(dim, size) = rectangle.longest_side();
  CHECK(dim == 1);
  CHECK(size == Approx(1.928561));
}
