#include "queries.hpp"
#include <gtest/gtest.h>

class QueriesTest : public ::testing::Test {
public:
    void SetUp() override {
        circle = std::make_unique<geometry::Circle>(geometry::Circle{{0.0, 0.0}, 5.0});
        triangle = std::make_unique<geometry::Triangle>(geometry::Triangle{{3.0, 0.0}, {0.0, 4.0}, {-3.0, 0.0}});
    }

    std::unique_ptr<geometry::Circle> circle;
    std::unique_ptr<geometry::Triangle> triangle;
};

TEST_F(QueriesTest, DistanceToPoint) {
    geometry::Point2D point{10.0, 0.0};
    double distance = geometry::queries::DistanceToPoint(*circle, point);
    EXPECT_DOUBLE_EQ(distance, 5.0);

    distance = geometry::queries::DistanceToPoint(*triangle, point);
    EXPECT_DOUBLE_EQ(distance, 7.0);
}

TEST_F(QueriesTest, GetBoundBox) {
    geometry::BoundingBox bb_circle = geometry::queries::GetBoundBox(*circle);
    EXPECT_DOUBLE_EQ(bb_circle.min_x, -5.0);
    EXPECT_DOUBLE_EQ(bb_circle.max_x, 5.0);
    EXPECT_DOUBLE_EQ(bb_circle.min_y, -5.0);
    EXPECT_DOUBLE_EQ(bb_circle.max_y, 5.0);

    geometry::BoundingBox bb_triangle = geometry::queries::GetBoundBox(*triangle);
    EXPECT_DOUBLE_EQ(bb_triangle.min_x, -3.0);
    EXPECT_DOUBLE_EQ(bb_triangle.max_x, 3.0);
    EXPECT_DOUBLE_EQ(bb_triangle.min_y, 0.0);
    EXPECT_DOUBLE_EQ(bb_triangle.max_y, 4.0);
}

TEST_F(QueriesTest, GetHeight) {
    double height_circle = geometry::queries::GetHeight(*circle);
    EXPECT_DOUBLE_EQ(height_circle, 5.0);

    double height_triangle = geometry::queries::GetHeight(*triangle);
    EXPECT_DOUBLE_EQ(height_triangle, 4.0);
}

TEST_F(QueriesTest, BoundingBoxesOverlap) {
    geometry::Rectangle rect1{{-1.0, -1.0}, 2.0, 2.0};
    geometry::Rectangle rect2{{0.0, 0.0}, 2.0, 2.0};
    geometry::Rectangle rect3{{3.0, 3.0}, 2.0, 2.0};

    EXPECT_TRUE(geometry::queries::BoundingBoxesOverlap(rect1, rect2));
    EXPECT_FALSE(geometry::queries::BoundingBoxesOverlap(rect1, rect3));
}

TEST_F(QueriesTest, DistanceBetweenShapes) {
    auto distance = geometry::queries::DistanceBetweenShapes(*circle, *triangle);
    EXPECT_FALSE(distance.has_value());  // overlap case

    geometry::Circle circle2{{20.0, 0.0}, 3.0};
    distance = geometry::queries::DistanceBetweenShapes(*circle, circle2);
    EXPECT_TRUE(distance.has_value());
    EXPECT_DOUBLE_EQ(distance.value(), 12.0);
}

TEST_F(QueriesTest, GetShapeVertices) {
    auto triangle_vertices = geometry::queries::GetShapeVertices(*triangle);
    EXPECT_EQ(triangle_vertices.size(), 3);
}