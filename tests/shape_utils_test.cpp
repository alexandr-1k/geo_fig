#include "geometry.hpp"
#include "shape_utils.hpp"
#include <gtest/gtest.h>

TEST(ShapeUtilsTest, ParseShapes) {
    std::vector<geometry::Shape> shapes =
        geometry::utils::ParseShapes("circle 0 0 1.5; line 1 2 3 4; polygon 0 0 2 5; triangle 0 0 1 0 "
                                     "0.5 1; polygon 0 0 1 2; badshape; circle 0 0 -1");

    EXPECT_EQ(shapes.size(), 4);
    EXPECT_TRUE(std::holds_alternative<geometry::Circle>(shapes[0]));
    EXPECT_TRUE(std::holds_alternative<geometry::Line>(shapes[1]));
    EXPECT_TRUE(std::holds_alternative<geometry::RegularPolygon>(shapes[2]));
    EXPECT_TRUE(std::holds_alternative<geometry::Triangle>(shapes[3]));

    const auto &circle = std::get<geometry::Circle>(shapes[0]);
    EXPECT_DOUBLE_EQ(circle.Center().x, 0.0);
    EXPECT_DOUBLE_EQ(circle.Center().y, 0.0);
    EXPECT_DOUBLE_EQ(circle.radius, 1.5);

    const auto &line = std::get<geometry::Line>(shapes[1]);
    EXPECT_DOUBLE_EQ(line.start.x, 1.0);
    EXPECT_DOUBLE_EQ(line.start.y, 2.0);
    EXPECT_DOUBLE_EQ(line.end.x, 3.0);
    EXPECT_DOUBLE_EQ(line.end.y, 4.0);

    const auto &polygon = std::get<geometry::RegularPolygon>(shapes[2]);
    auto vertices = polygon.Vertices();
    EXPECT_EQ(vertices.size(), 5);

    const auto &triangle = std::get<geometry::Triangle>(shapes[3]);
    auto t_vertices = triangle.Vertices();
    EXPECT_EQ(t_vertices.size(), 3);
    EXPECT_DOUBLE_EQ(t_vertices[0].x, 0.0);
    EXPECT_DOUBLE_EQ(t_vertices[0].y, 0.0);
    EXPECT_DOUBLE_EQ(t_vertices[1].x, 1.0);
    EXPECT_DOUBLE_EQ(t_vertices[1].y, 0.0);
    EXPECT_DOUBLE_EQ(t_vertices[2].x, 0.5);
    EXPECT_DOUBLE_EQ(t_vertices[2].y, 1.0);
}

TEST(ShapeUtilsTest, FindAllCollisions) {
    std::vector<geometry::Shape> shapes = {
        geometry::Circle{{0.0, 0.0}, 2.0}, geometry::Triangle{{1.0, 1.0}, {3.0, 1.0}, {2.0, 3.0}},
        geometry::Rectangle{{10.0, 10.0}, 2.0, 2.0}, geometry::Line{{10.0, 10.0}, {15.0, 15.0}}};

    std::vector<std::pair<geometry::Shape, geometry::Shape>> collisions = geometry::utils::FindAllCollisions(shapes);

    EXPECT_EQ(collisions.size(), 2);
    const auto &[shape1, shape2] = collisions[0];
    EXPECT_TRUE(std::holds_alternative<geometry::Circle>(shape1) || std::holds_alternative<geometry::Circle>(shape2));
    EXPECT_TRUE(std::holds_alternative<geometry::Triangle>(shape1) ||
                std::holds_alternative<geometry::Triangle>(shape2));
    const auto &[shape3, shape4] = collisions[1];
    EXPECT_TRUE(std::holds_alternative<geometry::Rectangle>(shape3) ||
                std::holds_alternative<geometry::Rectangle>(shape4));
    EXPECT_TRUE(std::holds_alternative<geometry::Line>(shape3) || std::holds_alternative<geometry::Line>(shape4));
}

TEST(ShapeUtilsTest, FindHighestShape) {
    std::vector<geometry::Shape> shapes = {
        geometry::Circle{{0.0, 0.0}, 2.0}, geometry::Triangle{{1.0, 1.0}, {3.0, 1.0}, {2.0, 3.0}},
        geometry::Rectangle{{5.0, 5.0}, 2.0, 4.0}, geometry::Line{{0.0, 0.0}, {4.0, 4.0}},
        geometry::RegularPolygon{{8.0, 8.0}, 1.0, 5}};

    auto highest_shape_index_opt = geometry::utils::FindHighestShape(shapes);

    ASSERT_TRUE(highest_shape_index_opt.has_value());
    EXPECT_EQ(highest_shape_index_opt.value(), 2);
}
