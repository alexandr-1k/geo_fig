#pragma once
#include "geometry.hpp"
#include <cmath>
#include <optional>
#include <variant>

namespace geometry::intersections {

/*
 * Класс для поиска пересечений между двумя фигурами
 *
 * Требуется организовать возможность нахождения пересечений только для следующих комбинаций фигур:
 *    - Line   & Line
 *    - Line   & Circle
 *    - Circle & Circle
 *
 * Для всех остальных требуется выбросить исключение std::logic_error
 */
class IntersectionVisitor {
public:
    std::optional<Point2D> operator()(const Line &l1, const Line &l2) const {
        double x1 = l1.start.x, y1 = l1.start.y;
        double x2 = l1.end.x, y2 = l1.end.y;

        double x3 = l2.start.x, y3 = l2.start.y;
        double x4 = l2.end.x, y4 = l2.end.y;
        double a1 = y2 - y1;
        double b1 = x1 - x2;
        double c1 = a1 * x1 + b1 * y1;

        double a2 = y4 - y3;
        double b2 = x3 - x4;
        double c2 = a2 * x3 + b2 * y3;

        double determinant = a1 * b2 - a2 * b1;

        if (std::abs(determinant) < 1e-9) {
            return std::nullopt;  // Lines are parallel
        }

        double x = (b1 * c2 - b2 * c1) / determinant;
        double y = (a2 * c1 - a1 * c2) / determinant;
        return Point2D{x, y};
    }

    std::optional<Point2D> operator()(const Line &line, const Circle &circle) const {
        Point2D d = line.end - line.start;
        Point2D f = line.start - circle.center_p;

        double a = d.Dot(d);
        double b = 2 * f.Dot(d);
        double c = f.Dot(f) - circle.radius * circle.radius;

        double discriminant = b * b - 4 * a * c;
        if (discriminant < 0) {
            return {};
        }
        discriminant = std::sqrt(discriminant);
        double t1 = (-b - discriminant) / (2 * a);
        double t2 = (-b + discriminant) / (2 * a);
        if (t1 >= 0 && t1 <= 1) {
            return line.start + d * t1;
        }
        if (t2 >= 0 && t2 <= 1) {
            return line.start + d * t2;
        }
        return {};
    }

    std::optional<Point2D> operator()(const Circle &c1, const Circle &c2) const {
        double d = c1.center_p.DistanceTo(c2.center_p);
        if (d > c1.radius + c2.radius || d < std::abs(c1.radius - c2.radius)) {
            return {};
        }
        double a = (c1.radius * c1.radius - c2.radius * c2.radius + d * d) / (2 * d);
        double h = std::sqrt(c1.radius * c1.radius - a * a);
        Point2D p0 = c1.center_p + (c2.center_p - c1.center_p) * (a / d);
        Point2D intersection1 =
            Point2D{p0.x + h * (c2.center_p.y - c1.center_p.y) / d, p0.y - h * (c2.center_p.x - c1.center_p.x) / d};
        return intersection1;
    }

    template <typename T1, typename T2>
    std::optional<Point2D> operator()(const T1 &shape1, const T2 &shape2) const {
        throw std::logic_error("Intersection not implemented for these shape types.");
    }
};

inline std::optional<Point2D> GetIntersectPoint(const Shape &shape1, const Shape &shape2) {
    IntersectionVisitor visitor;
    auto intersects = std::visit(visitor, shape1, shape2);
    if (intersects) {
        return *intersects;
    }
    return std::nullopt;
}

}  // namespace geometry::intersections