#include "convex_hull.hpp"
#include "geometry.hpp"
#include "intersections.hpp"
#include "queries.hpp"
#include "shape_utils.hpp"
#include "triangulation.hpp"
#include "visualization.hpp"

#include <algorithm>
#include <format>
#include <print>
#include <ranges>

using namespace geometry;

namespace rng = std::ranges;
namespace views = std::ranges::views;

void PrintAllIntersections(const Shape &shape, std::span<const Shape> others) {
    std::println("\n=== Intersections ===");
    /*
     * Используйте ranges чтобы оставить только фигуры,
     * поддерживающие возможность находить пересечения между собой
     *
     * Затем примените монадический интерфейс для обработки результатов:
     *     - Пересечение найдено в точке A между фигурами B и C
     *     - Фигуры B и C не пересекаются
     */
    rng::for_each(others, [&shape](const Shape &other) {
        auto intersection = geometry::intersections::GetIntersectPoint(shape, other)
                                .transform([](Point2D p) { return std::format("at point ({:.2f}, {:.2f})", p.x, p.y); })
                                .value_or(std::string("no intersection"));
        std::println("Intersection between shapes: {}", intersection);
    });
}

void PrintDistancesFromPointToShapes(Point2D p, std::span<const Shape> shapes) {
    std::println("\n=== Distance from Point Test ===");

    /*
     * Используйте ranges чтобы выбрать любые 5 фигур из списка.
     * Затем найдите расстояния от заданной точки до всех выбранных фигур.
     * Выведите результат в формате "Расстояние от точки P до фигуры S равно D"
     */

    auto selected_shapes = shapes | views::take(5);

    rng::for_each(selected_shapes, [p](const Shape &shape) {
        double distance = geometry::queries::DistanceToPoint(shape, p);
        std::println("Distance from point ({:.2f}, {:.2f}) to shape is {:.2f}", p.x, p.y, distance);
    });
}

void PerformShapeAnalysis(std::span<const Shape> shapes) {
    /*
     * Используйте ranges и созданные классы чтобы:
     *     - Найти все пересечения между фигурами используя метод Bounding Box
     *     - Найти самую высокую фигуру (чья высота наибольшая)expected
     *     - Вывести расстояние между любыми двумя фигурами, которые поддерживают данную функциональность
     */
    std::println("\n=== Shape Analysis ===");

    auto shape_pairs = views::cartesian_product(shapes, shapes);
    rng::for_each(shape_pairs, [](const auto &pair) {
        const auto &t = pair;
        const auto &[shape1, shape2] = t;

        if (&shape1 != &shape2 && geometry::queries::BoundingBoxesOverlap(shape1, shape2)) {
            std::println("Bounding boxes of two shapes overlap.");
            return;
        }

        auto distance_str = geometry::queries::DistanceBetweenShapes(shape1, shape2)
                                .transform([&](double d) { return std::format("Distance: {:.2f}", d); })
                                .value_or(std::string("Distance between two shapes is not supported."));

        std::println("{}", distance_str);
    });
}

void PerformExtraShapeAnalysis(std::span<const Shape> shapes) {
    std::println("\n=== Shape Extra Analysis ===");

    /*
     * Используйте ranges и созданные классы чтобы:
     *     - Вывести 3 любые фигуры, которые находятся выше 50.0
     *     - Вывести фигуры с наименьшей и с наибольшей высотами
     */

    auto selected_shapes =
        shapes | std::views::filter([](const auto &val) { return queries::GetHeight(val) > 50.0; }) | views::take(3);

    auto [min_it, max_it] = rng::minmax_element(
        shapes, [](const Shape &a, const Shape &b) { return queries::GetHeight(a) < queries::GetHeight(b); });

    std::println("Shapes higher than 50.0:");
    rng::for_each(selected_shapes,
                  [](const Shape &shape) { std::println("Shape height: {:.2f}", queries::GetHeight(shape)); });

    if (min_it != shapes.end() && max_it != shapes.end()) {
        std::println("Shape with minimum height: {:.2f}", queries::GetHeight(*min_it));
        std::println("Shape with maximum height: {:.2f}", queries::GetHeight(*max_it));
    }
}

int main() {
    std::vector<Shape> shapes = utils::ParseShapes("circle 0 0 1.5; line 1 2 3 4; polygon 0 0 2 5; triangle 0 0 1 0 "
                                                   "0.5 1; polygon 0 0 1 2; badshape; circle 0 0 -1");
    std::println("Parsed {} shapes", shapes.size());

    // Выведите индекс каждой фигуры и её высоту

    std::ranges::for_each(shapes | views::enumerate, [](const auto &indexed_shape) {
        const auto &[index, shape] = indexed_shape;
        std::println("Shape[{}] height: {:.2f}", index, queries::GetHeight(shape));
    });

    //
    // Вызываем разработанные функции
    //
    PrintAllIntersections(shapes[0], shapes);

    PrintDistancesFromPointToShapes(Point2D{10.0, 10.0}, shapes);

    PerformShapeAnalysis(shapes);

    PerformExtraShapeAnalysis(shapes);

    //
    // Рисуем все фигуры
    //
    // Важно: после изучения графика - нажмите Enter чтобы продолжить выполнение и построить 2ой график
    //
    geometry::visualization::Draw(shapes);

    //
    // Формируем список из вершин всех фигур
    //
    std::vector<Point2D> points;
    std::ranges::for_each(shapes, [&points](const Shape &shape) {
        auto vertices_vector = queries::GetShapeVertices(shape);
        points.insert(points.end(), vertices_vector.begin(), vertices_vector.end());
    });

    //
    // Находим список точек, для построения выпуклой оболочки - convex hull - алгоритмом Грэхема
    // Создаём из них объект класса `Polygon` и добавляем его в список shapes
    // Рисуем все фигуры
    //

    auto result = convex_hull::GrahamScan(points);
    if (result.has_value()) {
        Polygon hull_polygon{*result};
        shapes.push_back(hull_polygon);
        geometry::visualization::Draw(shapes);
    } else {
        std::println("Graham Scan failed: {}", static_cast<int>(result.error()));
    }

    geometry::visualization::Draw(shapes);

    //
    // после изучения графика - нажмите Enter чтобы продолжить выполнение и построить 3ий график
    //

    {
        std::vector<Point2D> points = {{0, 0}, {10, 0}, {5, 8}, {15, 5}, {2, 12}};

        //
        // Используйте список точек points или свой, чтобы
        // выполнить алгоритм триангуляции Делоне алгоритмом Боуэра-Ватсона
        //
        // После успешного завершения алгоритма - выведите результат для проверки
        // используя geometry::visualization::Draw
        //

        auto result = triangulation::DelaunayTriangulation(points);
        if (result.has_value()) {
            std::vector<triangulation::DelaunayTriangle> triangles_shapes;
            for (const auto &triangle : *result) {
                triangles_shapes.push_back(triangle);
            }
            geometry::visualization::Draw(triangles_shapes);
        } else {
            std::println("Delaunay Triangulation failed: {}", static_cast<int>(result.error()));
        }
    }
    return 0;
}