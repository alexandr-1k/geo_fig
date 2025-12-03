#include "triangulation.hpp"
#include <gtest/gtest.h>

TEST(TriangulationTest, SimpleCheck) {
    std::vector<geometry::Point2D> points = {{0, 0}, {10, 0}, {5, 8}, {15, 5}, {2, 12}};
    auto result = geometry::triangulation::DelaunayTriangulation(points);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->size(), 4);

    // vector<vector> is not efficient of course, should've used mdspan or similar,
    // but for test purposes it's fine

    // clang-format off
    std::vector<std::vector<geometry::Point2D>> expected_triangles = {
        {{0, 0}, {10, 0}, {5, 8}},
        {{10, 0}, {15, 5}, {5, 8}},
        {{5, 8}, {15, 5}, {2, 12}},
        {{0, 0}, {5, 8}, {2, 12}}
    };
    // clang-format on

    for (const auto &triangle : *result) {
        std::vector<geometry::Point2D> vertices = triangle.vertices();
        EXPECT_EQ(vertices.size(), 3);  // Each triangle should have 3 vertices
        bool found = false;
        for (const auto &expected : expected_triangles) {
            std::set<geometry::Point2D> vertex_set(vertices.begin(), vertices.end());
            std::set<geometry::Point2D> expected_set(expected.begin(), expected.end());
            if (vertex_set == expected_set) {
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found) << "Triangle not found in expected set";
    }
}
