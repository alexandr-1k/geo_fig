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
        double x1 = l1.start.x;
        double y1 = l1.start.y;
        double x2 = l1.end.x;
        double y2 = l1.end.y;
        double x3 = l2.start.x;
        double y3 = l2.start.y;
        double x4 = l2.end.x;
        double y4 = l2.end.y;
        double denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
        if (std::abs(denom) < 1e-9) {
            return std::nullopt;  // Lines are parallel
        }
        double px = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / denom;
        double py = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / denom;
        return Point2D{px, py};
    }

    std::optional<Point2D> operator()(const Line &line, const Circle &circle) const {
        Point2D d = line.end - line.start;
        Point2D f = line.start - circle.center_p;

        double a = d.Dot(d);
        double b = 2 * f.Dot(d);
        double c = f.Dot(f) - circle.radius * circle.radius;

        double discriminant = b * b - 4 * a * c;
        if (discriminant < 0) {
            return std::nullopt;  // No intersection
        } else {
            discriminant = std::sqrt(discriminant);
            double t1 = (-b - discriminant) / (2 * a);
            double t2 = (-b + discriminant) / (2 * a);

            if (t1 >= 0 && t1 <= 1) {
                return line.start + d * t1;
            }
            if (t2 >= 0 && t2 <= 1) {
                return line.start + d * t2;
            }
            return std::nullopt;  // Intersection points are not within the line segment
        }
    }
    std::optional<Point2D> operator()(const Circle &circle, const Line &line) const { return (*this)(line, circle); }

    std::optional<Point2D> operator()(const Circle &c1, const Circle &c2) const {
        double d = (c2.center_p - c1.center_p).Length();
        if (d > c1.radius + c2.radius || d < std::abs(c1.radius - c2.radius)) {
            return std::nullopt;  // No intersection
        }

        double a = (c1.radius * c1.radius - c2.radius * c2.radius + d * d) / (2 * d);
        double h = std::sqrt(c1.radius * c1.radius - a * a);

        Point2D p0 = c1.center_p + (c2.center_p - c1.center_p) * (a / d);
        Point2D offset = Point2D{-(c2.center_p.y - c1.center_p.y) * (h / d), (c2.center_p.x - c1.center_p.x) * (h / d)};

        Point2D intersection1 = p0 + offset;
        Point2D intersection2 = p0 - offset;

        return intersection1;  // Return one of the intersection points
    }

    template <typename T1, typename T2>
    std::optional<Point2D> operator()(const T1 &shape1, const T2 &shape2) const {
        return std::nullopt;
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