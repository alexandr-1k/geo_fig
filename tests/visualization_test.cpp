#include "geometry.hpp"
#include "visualization.hpp"
#include <gtest/gtest.h>

template <typename T>
void DrawInTest(std::vector<T> shapes) {
    // mimic pressing Enter to skip the plot display
    std::istringstream fake_in("\n");
    auto *old_buf = std::cin.rdbuf(fake_in.rdbuf());

    geometry::visualization::Draw(shapes);

    std::cin.rdbuf(old_buf);
}

TEST(VisualizationTest, SimpleCheck) {
    std::vector<geometry::Shape> shapes = {
        geometry::Circle({0, 0}, 1.0),
        geometry::Triangle({{0, 0}, {1, 0}, {0, 1}}),
    };
    DrawInTest(std::move(shapes));
    SUCCEED();
}

TEST(VisualizationTest, Triangulation) {
    std::vector<geometry::Point2D> points = {{0, 0}, {10, 0}, {5, 8}, {15, 5}, {2, 12}};
    auto result = geometry::triangulation::DelaunayTriangulation(points);
    ASSERT_TRUE(result.has_value());
    DrawInTest(std::move(*result));
    SUCCEED();
}
