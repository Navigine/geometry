#pragma once

#include <navigine/geometry/geolib/detail/type_checks.h>
#include <navigine/geometry/geolib/detail/util.h>

namespace navigine::geometry::geolib::detail {

template <typename Point>
bool equalPoints(const Point& point1, const Point& point2, double epsilon = 0.0)
{
    static_assert(isPoint<Point>());

    const auto p1x = util::getX(point1);
    const auto p2x = util::getX(point2);
    const auto p1y = util::getY(point1);
    const auto p2y = util::getY(point2);

    return epsilon == 0.0 ?
        (p1x == p2x && p1y == p2y) :
        (std::fabs(p1x - p2x) < epsilon && std::fabs(p1y - p2y) < epsilon);
}

template <typename Point>
int comparePoints(const Point& point1, const Point& point2)
{
    static_assert(isPoint<Point>());

    if (util::getX(point1) < util::getX(point2)) {
        return -1;
    } else if (util::getX(point1) > util::getX(point2)) {
        return 1;
    }

    if (util::getY(point1) < util::getY(point2)) {
        return -1;
    } else if (util::getY(point1) > util::getY(point2)) {
        return 1;
    }

    return 0;
}

template <typename Segment>
int equalSegments(const Segment& segment1, const Segment& segment2, double epsilon = 0.0)
{
    static_assert(isSegment<Segment>());
    return equalPoints(segment1.first, segment2.first, epsilon) &&
           equalPoints(segment1.second, segment2.second, epsilon);
}

template <typename Segment>
int compareSegments(const Segment& segment1, const Segment& segment2)
{
    static_assert(isSegment<Segment>());

    if (int r = comparePoints(segment1.first, segment2.first)) {
        return r;
    }

    return comparePoints(segment1.second, segment2.second);
}

template <typename Box>
bool equalBoxes(const Box& box1, const Box& box2, double epsilon = 0.0)
{
    static_assert(isBox<Box>());
    return equalPoints(box1.minCorner, box2.minCorner, epsilon) &&
           equalPoints(box1.maxCorner, box2.maxCorner, epsilon);
}

template <typename Box>
int compareBoxes(const Box& box1, const Box& box2)
{
    static_assert(isBox<Box>());

    if (int r = comparePoints(box1.minCorner, box2.minCorner)) {
        return r;
    }

    return comparePoints(box1.maxCorner, box2.maxCorner);
}

template <typename LineString>
bool equalLineStrings(const LineString& linestring1, const LineString& linestring2, double epsilon = 0.0)
{
    static_assert(isLineString<LineString>());

    if (linestring1.points.size() != linestring2.points.size()) {
        return false;
    }

    for(size_t i = 0; i < linestring1.points.size(); ++i) {
        if (!equalPoints(linestring1.points[i], linestring2.points[i], epsilon)) {
            return false;
        }
    }

    return true;
}

template <typename LineString>
int compareLineStrings(const LineString& linestring1, const LineString& linestring2)
{
    static_assert(isLineString<LineString>());

    for(size_t i = 0; i < linestring1.points.size() && i < linestring2.points.size(); ++i) {
        if (int r = comparePoints(linestring1.points[i], linestring2.points[i])) {
            return r;
        }
    }

    if (linestring1.points.size() < linestring2.points.size()) {
        return -1;
    } else if (linestring1.points.size() > linestring2.points.size()) {
        return 1;
    }

    return 0;
}

template <typename Ring>
bool equalRings(const Ring& ring1, const Ring& ring2, double epsilon = 0.0)
{
    static_assert(isRing<Ring>());

    if (ring1.points.size() != ring2.points.size()) {
        return false;
    }

    for(size_t i = 0; i < ring1.points.size(); ++i) {
        if (!equalPoints(ring1.points[i], ring2.points[i], epsilon)) {
            return false;
        }
    }

    return true;
}

template <typename Ring>
int compareRings(const Ring& ring1, const Ring& ring2)
{
    static_assert(isRing<Ring>());

    for(size_t i = 0; i < ring1.points.size() && i < ring2.points.size(); ++i) {
        if (int r = comparePoints(ring1.points[i], ring2.points[i])) {
            return r;
        }
    }

    if (ring1.points.size() < ring2.points.size()) {
        return -1;
    } else if (ring1.points.size() > ring2.points.size()) {
        return 1;
    }

    return 0;
}

template <typename Polygon>
bool equalPolygons(const Polygon& poly1, const Polygon& poly2, double epsilon = 0.0)
{
    static_assert(isPolygon<Polygon>());

    if (!equalRings(poly1.outer, poly2.outer, epsilon)) {
        return false;
    }

    if (poly1.inners.size() != poly2.inners.size()) {
        return false;
    }

    for(size_t i = 0; i < poly1.inners.size(); ++i) {
        if (!equalRings(poly1.inners[i], poly2.inners[i], epsilon)) {
            return false;
        }
    }

    return true;
}

template <typename Polygon>
int comparePolygons(const Polygon& poly1, const Polygon& poly2)
{
    static_assert(isPolygon<Polygon>());

    if (int r = compareRings(poly1.outer, poly2.outer)) {
        return r;
    }

    for(size_t i = 0; i < poly1.inners.size() && i < poly2.inners.size(); ++i) {
        if (int r = compareRings(poly1.inners[i], poly2.inners[i])) {
            return r;
        }
    }

    if (poly1.inners.size() < poly2.inners.size()) {
        return -1;
    } else if (poly1.inners.size() > poly2.inners.size()) {
        return 1;
    }

    return 0;
}

template <typename MultiPolygon>
bool equalMultiPolygons(const MultiPolygon& geom1, const MultiPolygon& geom2, double epsilon = 0.0)
{
    static_assert(isMultiPolygon<MultiPolygon>());

    if (geom1.parts.size() != geom2.parts.size()) {
        return false;
    }

    for(size_t i = 0; i < geom1.parts.size(); ++i) {
        if (!equalPolygons(geom1.parts[i], geom2.parts[i], epsilon)) {
            return false;
        }
    }

    return true;
}

template <typename MultiPolygon>
int compareMultiPolygons(const MultiPolygon& geom1, const MultiPolygon& geom2)
{
    static_assert(isMultiPolygon<MultiPolygon>());

    for(size_t i = 0; i < geom1.parts.size() && i < geom2.parts.size(); ++i) {
        if (int r = comparePolygons(geom1.parts[i], geom2.parts[i])) {
            return r;
        }
    }

    if (geom1.parts.size() < geom2.parts.size()) {
        return -1;
    } else if (geom1.parts.size() > geom2.parts.size()) {
        return 1;
    }

    return 0;
}

template <typename Geometry>
int compare(const Geometry& geometry1, const Geometry& geometry2)
{
    static_assert(
        isPoint<Geometry>() ||
        isSegment<Geometry>() ||
        isBox<Geometry>() ||
        isLineString<Geometry>() ||
        isRing<Geometry>() ||
        isPolygon<Geometry>() ||
        isMultiPolygon<Geometry>());

    if constexpr (isPoint<Geometry>()) {
        return comparePoints(geometry1, geometry2);
    } else if constexpr (isSegment<Geometry>()) {
        return compareSegments(geometry1, geometry2);
    } else if constexpr (isBox<Geometry>()) {
        return compareBoxes(geometry1, geometry2);
    } else if constexpr (isLineString<Geometry>()) {
        return compareLineStrings(geometry1, geometry2);
    } else if constexpr (isRing<Geometry>()) {
        return compareRings(geometry1, geometry2);
    } else if constexpr (isPolygon<Geometry>()) {
        return comparePolygons(geometry1, geometry2);
    } else if constexpr (isMultiPolygon<Geometry>()) {
        return compareMultiPolygons(geometry1, geometry2);
    }
}

template <typename Geometry>
bool equal(const Geometry& geometry1, const Geometry& geometry2, double epsilon = 0.0)
{
    static_assert(
        isPoint<Geometry>() ||
        isSegment<Geometry>() ||
        isBox<Geometry>() ||
        isLineString<Geometry>() ||
        isRing<Geometry>() ||
        isPolygon<Geometry>() ||
        isMultiPolygon<Geometry>());

    if constexpr (isPoint<Geometry>()) {
        return equalPoints(geometry1, geometry2, epsilon);
    } else if constexpr (isSegment<Geometry>()) {
        return equalSegments(geometry1, geometry2, epsilon);
    } else if constexpr (isBox<Geometry>()) {
        return equalBoxes(geometry1, geometry2, epsilon);
    } else if constexpr (isLineString<Geometry>()) {
        return equalLineStrings(geometry1, geometry2, epsilon);
    } else if constexpr (isRing<Geometry>()) {
        return equalRings(geometry1, geometry2, epsilon);
    } else if constexpr (isPolygon<Geometry>()) {
        return equalPolygons(geometry1, geometry2, epsilon);
    } else if constexpr (isMultiPolygon<Geometry>()) {
        return equalMultiPolygons(geometry1, geometry2, epsilon);
    }
}

template <typename Geometry>
bool less(const Geometry& geometry1, const Geometry& geometry2)
{
    return detail::compare(geometry1, geometry2) < 0;
}

template <typename Geometry>
bool greater(const Geometry& geometry1, const Geometry& geometry2)
{
    return detail::compare(geometry1, geometry2) > 0;
}

} // namespace navigine::geometry::geolib::detail
