#include "intersections.hpp"
#include <gtest/gtest.h>

TEST(IntersectionsTest, IntersectLines) {
    geometry::Line line1{{0, 0}, {2, 2}};
    geometry::Line line2{{0, 2}, {2, 0}};

    geometry::intersections::IntersectionVisitor visitor;
    auto result = visitor(line1, line2);

    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(result->x, 1.0);
    EXPECT_DOUBLE_EQ(result->y, 1.0);
}

TEST(IntersectionsTest, NoIntersectLines) {
    geometry::Line line1{{0, 0}, {1, 1}};
    geometry::Line line2{{2, 2}, {3, 3}};

    geometry::intersections::IntersectionVisitor visitor;
    auto result = visitor(line1, line2);

    ASSERT_FALSE(result.has_value());
}

TEST(IntersectionsTest, IntersectLineCircle) {
    geometry::Line line{{0, 0}, {4, 0}};
    geometry::Circle circle{{2, 0}, 1};

    geometry::intersections::IntersectionVisitor visitor;
    auto result = visitor(line, circle);

    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(result->x, 1.0);
    EXPECT_DOUBLE_EQ(result->y, 0.0);
}

TEST(IntersectionsTest, NoIntersectLineCircle) {
    geometry::Line line{{0, 0}, {1, 1}};
    geometry::Circle circle{{3, 3}, 1};

    geometry::intersections::IntersectionVisitor visitor;
    auto result = visitor(line, circle);

    ASSERT_FALSE(result.has_value());
}

TEST(IntersectionsTest, IntersectCircles) {
    geometry::Circle circle1{{0, 0}, 2};
    geometry::Circle circle2{{3, 0}, 2};

    geometry::intersections::IntersectionVisitor visitor;
    auto result = visitor(circle1, circle2);

    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(result->x, 1.5);
    EXPECT_DOUBLE_EQ(result->y, std::sqrt(1.75));
}

TEST(IntersectionsTest, NoIntersectCircles) {
    geometry::Circle circle1{{0, 0}, 1};
    geometry::Circle circle2{{3, 0}, 1};

    geometry::intersections::IntersectionVisitor visitor;
    auto result = visitor(circle1, circle2);

    ASSERT_FALSE(result.has_value());
}

TEST(IntersectionsTest, IntersectPolygonsNotAllowed) {
    geometry::Polygon poly1({{0, 0}, {4, 0}, {4, 4}, {0, 4}});
    geometry::Polygon poly2({{2, 2}, {6, 2}, {6, 6}, {2, 6}});

    geometry::intersections::IntersectionVisitor visitor;
    auto result = visitor(poly1, poly2);

    ASSERT_FALSE(result.has_value());
}

TEST(IntersectionsTest, IntersectLinePolygonNotAllowed) {
    geometry::Line line{{0, 0}, {4, 4}};
    geometry::Polygon poly({{1, 1}, {3, 1}, {3, 3}, {1, 3}});

    geometry::intersections::IntersectionVisitor visitor;
    auto result = visitor(line, poly);

    ASSERT_FALSE(result.has_value());
}

TEST(IntersectionsTest, IntersectCirclePolygonNotAllowed) {
    geometry::Circle circle{{2, 2}, 2};
    geometry::Polygon poly({{1, 1}, {3, 1}, {3, 3}, {1, 3}});

    geometry::intersections::IntersectionVisitor visitor;
    auto result = visitor(circle, poly);

    ASSERT_FALSE(result.has_value());
}

TEST(IntersectionsTest, IntersectLineTriangleNotAllowed) {
    geometry::Line line{{0, 0}, {4, 4}};
    geometry::Triangle triangle{{1, 1}, {3, 1}, {2, 3}};

    geometry::intersections::IntersectionVisitor visitor;
    auto result = visitor(line, triangle);

    ASSERT_FALSE(result.has_value());
}

TEST(IntersectionsTest, IntersectCircleTriangleNotAllowed) {
    geometry::Circle circle{{2, 2}, 2};
    geometry::Triangle triangle{{1, 1}, {3, 1}, {2, 3}};

    geometry::intersections::IntersectionVisitor visitor;
    auto result = visitor(circle, triangle);

    ASSERT_FALSE(result.has_value());
}

TEST(IntersectionsTest, IntersectTriangleTriangleNotAllowed) {
    geometry::Triangle triangle1{{0, 0}, {4, 0}, {2, 4}};
    geometry::Triangle triangle2{{1, 1}, {5, 1}, {3, 5}};

    geometry::intersections::IntersectionVisitor visitor;
    auto result = visitor(triangle1, triangle2);

    ASSERT_FALSE(result.has_value());
}