#pragma once

#include <navigine/geometry/geolib/detail/convex_hull.h>
#include <navigine/geometry/geolib/detail/iterate.h>
#include <navigine/geometry/geolib/detail/binary_operation.h>
#include <navigine/geometry/geolib/detail/type_checks.h>
#include <navigine/geometry/geolib/detail/util.h>

namespace navigine::geometry::geolib::detail {

template <typename Polygon>
Polygon makeEdgeBufferRound(
    const typename Polygon::point_type& p1,
    const typename Polygon::point_type& p2,
    double radius,
    unsigned int numPointsSemiCircle)
{
    static_assert(isPolygon<Polygon>());

    using Point = typename Polygon::point_type;

    const auto deltaAngle = M_PI / numPointsSemiCircle;

    std::vector<Point> points;

    for(unsigned int i = 0; i < 2 * numPointsSemiCircle; ++i) {
        const auto alpha = i * deltaAngle;
        Point point;
        util::getX(point) = util::getX(p1) + radius * std::cos(alpha);
        util::getY(point) = util::getY(p1) + radius * std::sin(alpha);
        points.push_back(snapPoint(point));
    }

    for(unsigned int i = 0; i < 2 * numPointsSemiCircle; ++i) {
        const auto alpha = i * deltaAngle;
        Point point;
        util::getX(point) = util::getX(p2) + radius * std::cos(alpha);
        util::getY(point) = util::getY(p2) + radius * std::sin(alpha);
        points.push_back(snapPoint(point));
    }

    Polygon result;
    result.outer.points = detail::convexHull(points);
    detail::correct(result);

    return result;
}

template <typename MultiPolygon, typename Geometry>
MultiPolygon buffer(
    const Geometry& geometry,
    double radius,
    unsigned int numPointsSemiCircle,
    bool skipErrors)
{
    static_assert(std::is_same<typename Geometry::point_type, typename MultiPolygon::point_type>::value == true);
    static_assert(isBox<Geometry>() || isRing<Geometry>() || isPolygon<Geometry>() || isMultiPolygon<Geometry>());
    static_assert(isMultiPolygon<MultiPolygon>());

    using Point = typename MultiPolygon::point_type;
    using Polygon = typename MultiPolygon::polygon_type;

    assert(radius > util::epsilon<Point>());
    assert(numPointsSemiCircle >= 2);

    std::vector<Polygon> polygons;

    iterateEdges(geometry, false,
        [&](const Point& p1, const Point& p2, const Point&, const Point&) {
            polygons.push_back(makeEdgeBufferRound<Polygon>(p1, p2, radius, numPointsSemiCircle));
        });

    MultiPolygon result = binaryOperation<MultiPolygon>(BinaryOperationType::UNION, geometry, MultiPolygon{});

    for(const auto& polygon : polygons) {
        try {
            result = binaryOperation<MultiPolygon>(BinaryOperationType::UNION, result, polygon);
        } catch (std::runtime_error& e) {
            if (!skipErrors) {
                throw;
            }
        }
    }

    return result;
}

} // namespace navigine::geometry::geolib::detail
