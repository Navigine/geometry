#pragma once

#include <navigine/geometry/geolib/detail/intersection.h>
#include <navigine/geometry/geolib/detail/iterate.h>
#include <navigine/geometry/geolib/detail/type_checks.h>
#include <navigine/geometry/geolib/detail/util.h>

namespace navigine::geometry::geolib::detail {

template <typename Point>
int updatePointIndex(
    const Point& point,
    const Point& A,
    const Point& B,
    int index)
{
    if (!index) {
        return 0;
    }

    auto px = util::getX(point);
    auto py = util::getY(point);
    auto ax = util::getX(A);
    auto ay = util::getY(A);
    auto bx = util::getX(B);
    auto by = util::getY(B);

    if (ay > by) {
        std::swap(ax, bx);
        std::swap(ay, by);
    }

    if (ay == by && ay == py) {
        if ((ax <= px && px <= bx) || (bx <= px && px <= ax)) {
            return 0;
        }
        return index;
    }

    if (ay >= py || by < py) {
        return index;
    }

    assert(ay < py && py <= by);

    auto lhs = (ax - px) * (by - ay);
    auto rhs = (ay - py) * (bx - ax);

    if (lhs == rhs) {
        return 0;
    }

    return lhs < rhs ? index : -index;
}

template <typename Box>
int pointIndexBox(const Box& box, const typename Box::point_type& point)
{
    static_assert(isBox<Box>());

    if (!std::isfinite(util::getX(point)) ||
        !std::isfinite(util::getY(point))) {
        return -1;
    }

    if (util::getX(point) < util::getX(box.minCorner) ||
        util::getX(point) > util::getX(box.maxCorner) ||
        util::getY(point) < util::getY(box.minCorner) ||
        util::getY(point) > util::getY(box.maxCorner)) {
        return -1;
    }

    if (util::getX(point) > util::getX(box.minCorner) ||
        util::getX(point) < util::getX(box.maxCorner) ||
        util::getY(point) > util::getY(box.minCorner) ||
        util::getY(point) < util::getY(box.maxCorner)) {
        return 1;
    }

    return 0;
}

template <typename Ring>
int pointIndexRing(const Ring& ring, const typename Ring::point_type& point)
{
    static_assert(isRing<Ring>());

    if (!std::isfinite(util::getX(point)) ||
        !std::isfinite(util::getY(point))) {
        return -1;
    }

    int index = -1;

    for(size_t i = 0; i + 1 < ring.points.size() && index != 0; ++i) {
        index = updatePointIndex(point, ring.points[i], ring.points[i + 1], index);
    }

    return index;
}

template <typename Polygon>
int pointIndexPolygon(const Polygon& poly, const typename Polygon::point_type& point)
{
    static_assert(isPolygon<Polygon>());

    using Ring = typename Polygon::ring_type;

    if (!std::isfinite(util::getX(point)) ||
        !std::isfinite(util::getY(point))) {
        return -1;
    }

    int index = pointIndexRing<Ring>(poly.outer, point);

    if (index <= 0) {
        return index;
    }

    for(const auto& inner : poly.inners) {
        int index = pointIndexRing<Ring>(inner, point);
        if (index >= 0) {
            return -index;
        }
    }

    return 1;
}

template <typename MultiPolygon>
int pointIndexMultiPolygon(const MultiPolygon& geometry, const typename MultiPolygon::point_type& point)
{
    static_assert(isMultiPolygon<MultiPolygon>());

    using Polygon = typename MultiPolygon::polygon_type;

    if (!std::isfinite(util::getX(point)) ||
        !std::isfinite(util::getY(point))) {
        return -1;
    }

    for(const auto& part : geometry.parts) {
        int index = pointIndexPolygon<Polygon>(part, point);
        if (index >= 0) {
            return index;
        }
    }

    return -1;
}

template <typename Geometry>
int pointIndex(const Geometry& geometry, const typename Geometry::point_type& point)
{
    static_assert(
        isBox<Geometry>() ||
        isRing<Geometry>() ||
        isPolygon<Geometry>() ||
        isMultiPolygon<Geometry>());

    if constexpr (isBox<Geometry>()) {
        return detail::pointIndexBox<Geometry>(geometry, point);
    } else if constexpr (isRing<Geometry>()) {
        return detail::pointIndexRing<Geometry>(geometry, point);
    } else if constexpr (isPolygon<Geometry>()) {
        return detail::pointIndexPolygon<Geometry>(geometry, point);
    } else if constexpr (isMultiPolygon<Geometry>()) {
        return detail::pointIndexMultiPolygon<Geometry>(geometry, point);
    }
}

template <typename Geometry1>
bool containsSegment(
    const Geometry1& geometry1,
    const typename Geometry1::point_type& first,
    const typename Geometry1::point_type& second)
{
    if (pointIndex<Geometry1>(geometry1, first) < 0 ||
        pointIndex<Geometry1>(geometry1, second) < 0) {
        return false;
    }

    using Point = typename Geometry1::point_type;

    std::map<double, Point> crossPoints;

    try {
        iterateEdges(geometry1, false,
            [&](const Point& p1, const Point& p2, const Point&, const Point&) {
                double u1 = NAN, u2 = NAN, v1 = NAN, v2 = NAN;
                Point i1, i2;
                int count = segmentIntersection(
                    first, second, p1, p2,
                    u1, v1, i1,
                    u2, v2, i2);

                if (count == 1 && 0.0 < u1 && u1 < 1.0 && 0.0 < v1 && v1 < 1.0) {
                    throw false; // Breaking loop
                }
                if (count >= 1 && 0.0 < u1 && u1 < 1.0) {
                    crossPoints.emplace(u1, i1);
                }
                if (count >= 2 && 0.0 < u2 && u2 < 1.0) {
                    crossPoints.emplace(u2, i2);
                }
            });
    } catch (...) {
        return false;
    }

    if (crossPoints.empty()) {
        return true;
    }

    auto src = first;
    crossPoints.emplace(1.0, second);
    for(const auto& [_, point] : crossPoints) {
        if (pointIndex(geometry1, (src + point) / 2) < 0) {
            return false;
        }
        src = point;
    }

    return true;
}

template <typename Geometry1, typename Geometry2>
bool contains(const Geometry1& geometry1, const Geometry2& geometry2)
{
    static_assert(isBox<Geometry1>() || isRing<Geometry1>() || isPolygon<Geometry1>() || isMultiPolygon<Geometry1>());
    static_assert(isPoint<Geometry2>() || isSegment<Geometry2>() || isLineString<Geometry2>());

    if constexpr (isPoint<Geometry2>()) {
        static_assert(std::is_same<typename Geometry1::point_type, Geometry2>::value == true);
        return detail::pointIndex<Geometry1>(geometry1, geometry2) >= 0;
    } else if constexpr (isSegment<Geometry2>()) {
        return detail::containsSegment<Geometry1>(geometry1, geometry2.first, geometry2.second);
        static_assert(std::is_same<typename Geometry1::point_type, typename Geometry2::point_type>::value == true);
    } else if constexpr (isLineString<Geometry2>()) {
        static_assert(std::is_same<typename Geometry1::point_type, typename Geometry2::point_type>::value == true);
        for(size_t i = 0; i + 1 < geometry2.points.size(); ++i) {
            if (!detail::containsSegment<Geometry1>(geometry1, geometry2.points[i], geometry2.points[i+1])) {
                return false;
            }
        }
        return true;
    }
}

} // namespace navigine::geometry::geolib::detail
