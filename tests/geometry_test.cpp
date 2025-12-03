#include "geometry.hpp"
#include <gtest/gtest.h>

class TestShapeMethods : public ::testing::Test {
protected:
    geometry::Point2D p1{0.0, 0.0};
    geometry::Point2D p2{3.0, 4.0};
    geometry::Line line{{0.0, 0.0}, {3.0, 4.0}};
    geometry::Triangle triangle{{0.0, 0.0}, {4.0, 0.0}, {0.0, 3.0}};
    geometry::Rectangle rectangle{{1.0, 1.0}, 4.0, 3.0};
    geometry::RegularPolygon regular_polygon{{0.0, 0.0}, 5.0, 6};
    geometry::Circle circle{{2.0, 2.0}, 3.0};
    geometry::Polygon polygon{{{0.0, 0.0}, {4.0, 0.0}, {4.0, 3.0}, {0.0, 3.0}}};
};

TEST_F(TestShapeMethods, Point2DMethods) {
    EXPECT_DOUBLE_EQ(p1.DistanceTo(p2), 5.0);
    EXPECT_DOUBLE_EQ(p1.Dot(p2), 0.0);
    EXPECT_DOUBLE_EQ(p2.Cross(p1), 0.0);
    EXPECT_DOUBLE_EQ(p1.Length(), 0.0);
    EXPECT_DOUBLE_EQ(p2.Length(), 5.0);
}

TEST_F(TestShapeMethods, LineMethods) {
    EXPECT_DOUBLE_EQ(line.Length(), 5.0);
    EXPECT_DOUBLE_EQ(line.Height(), 4.0);
    auto center = line.Center();
    EXPECT_DOUBLE_EQ(center.x, 1.5);
    EXPECT_DOUBLE_EQ(center.y, 2.0);
    EXPECT_DOUBLE_EQ(line.BoundBox().min_x, 0.0);
    EXPECT_DOUBLE_EQ(line.BoundBox().max_x, 3.0);
    EXPECT_DOUBLE_EQ(line.BoundBox().min_y, 0.0);
    EXPECT_DOUBLE_EQ(line.BoundBox().max_y, 4.0);
    EXPECT_DOUBLE_EQ(line.BoundBox().Width(), 3.0);
    EXPECT_DOUBLE_EQ(line.BoundBox().Height(), 4.0);
    EXPECT_EQ(line.Direction().x, 0.6);
    EXPECT_EQ(line.Direction().y, 0.8);
}

TEST_F(TestShapeMethods, TriangleMethods) {
    EXPECT_DOUBLE_EQ(triangle.Area(), 6.0);
    EXPECT_DOUBLE_EQ(triangle.Height(), 3.0);
    auto center = triangle.Center();
    EXPECT_DOUBLE_EQ(center.x, 4.0 / 3.0);
    EXPECT_DOUBLE_EQ(center.y, 1.0);
    EXPECT_DOUBLE_EQ(triangle.BoundBox().min_x, 0.0);
    EXPECT_DOUBLE_EQ(triangle.BoundBox().max_x, 4.0);
    EXPECT_DOUBLE_EQ(triangle.BoundBox().min_y, 0.0);
    EXPECT_DOUBLE_EQ(triangle.BoundBox().max_y, 3.0);
}

TEST_F(TestShapeMethods, RectangleMethods) {
    EXPECT_DOUBLE_EQ(rectangle.Height(), 4.0);
    auto center = rectangle.Center();
    EXPECT_DOUBLE_EQ(center.x, 3.0);
    EXPECT_DOUBLE_EQ(center.y, 2.5);
    EXPECT_DOUBLE_EQ(rectangle.BoundBox().min_x, 1.0);
    EXPECT_DOUBLE_EQ(rectangle.BoundBox().max_x, 5.0);
    EXPECT_DOUBLE_EQ(rectangle.BoundBox().min_y, 1.0);
    EXPECT_DOUBLE_EQ(rectangle.BoundBox().max_y, 4.0);
    EXPECT_DOUBLE_EQ(rectangle.TopRight().x, 5.0);
    EXPECT_DOUBLE_EQ(rectangle.TopRight().y, 4.0);
}

TEST_F(TestShapeMethods, RegularPolygonMethods) {
    EXPECT_DOUBLE_EQ(regular_polygon.Height(), 5.0);
    auto center = regular_polygon.Center();
    EXPECT_DOUBLE_EQ(center.x, 0.0);
    EXPECT_DOUBLE_EQ(center.y, 0.0);
    EXPECT_DOUBLE_EQ(regular_polygon.BoundBox().min_x, -5.0);
    EXPECT_DOUBLE_EQ(regular_polygon.BoundBox().max_x, 5.0);
    EXPECT_DOUBLE_EQ(regular_polygon.BoundBox().min_y, -5.0);
    EXPECT_DOUBLE_EQ(regular_polygon.BoundBox().max_y, 5.0);
}

TEST_F(TestShapeMethods, CircleMethods) {
    EXPECT_DOUBLE_EQ(circle.Height(), 5.0);
    auto center = circle.Center();
    EXPECT_DOUBLE_EQ(center.x, 2.0);
    EXPECT_DOUBLE_EQ(center.y, 2.0);
    EXPECT_DOUBLE_EQ(circle.BoundBox().min_x, -1.0);
    EXPECT_DOUBLE_EQ(circle.BoundBox().max_x, 5.0);
    EXPECT_DOUBLE_EQ(circle.BoundBox().min_y, -1.0);
    EXPECT_DOUBLE_EQ(circle.BoundBox().max_y, 5.0);
}

TEST_F(TestShapeMethods, PolygonMethods) {
    EXPECT_DOUBLE_EQ(polygon.Height(), 3.0);
    auto center = polygon.Center();
    EXPECT_DOUBLE_EQ(center.x, 2.0);
    EXPECT_DOUBLE_EQ(center.y, 1.5);
    EXPECT_DOUBLE_EQ(polygon.BoundBox().min_x, 0.0);
    EXPECT_DOUBLE_EQ(polygon.BoundBox().max_x, 4.0);
    EXPECT_DOUBLE_EQ(polygon.BoundBox().min_y, 0.0);
    EXPECT_DOUBLE_EQ(polygon.BoundBox().max_y, 3.0);
}
