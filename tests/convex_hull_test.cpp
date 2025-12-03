#include "convex_hull.hpp"
#include <gtest/gtest.h>

TEST(ConvexHullTest, SimpleCheck) {

    std::vector<geometry::Point2D> points = {{7, 7}, {7, -7}, {-7, -7}, {-7, 7}, {9, 0}, {-9, 0}, {0, 9}, {0, -9}};

    auto hull = geometry::convex_hull::GrahamScan(points);

    std::vector<geometry::Point2D> expected_hull = {{0, -9}, {7, -7}, {9, 0},  {7, 7},
                                                    {0, 9},  {-7, 7}, {-9, 0}, {-7, -7}};
    ASSERT_TRUE(hull.has_value());
    EXPECT_EQ(hull->size(), expected_hull.size());
    for (const auto &point : expected_hull) {
        EXPECT_NE(std::find(hull->begin(), hull->end(), point), hull->end());
    }
}
