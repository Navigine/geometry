#pragma once

#include <string>
#include <vector>

#include <navigine/geometry/geolib/detail/area.h>
#include <navigine/geometry/geolib/detail/binary_operation.h>
#include <navigine/geometry/geolib/detail/bounding_box.h>
#include <navigine/geometry/geolib/detail/buffer.h>
#include <navigine/geometry/geolib/detail/centroid.h>
#include <navigine/geometry/geolib/detail/contains.h>
#include <navigine/geometry/geolib/detail/compare.h>
#include <navigine/geometry/geolib/detail/convex_hull.h>
#include <navigine/geometry/geolib/detail/correct.h>
#include <navigine/geometry/geolib/detail/graph.h>
#include <navigine/geometry/geolib/detail/intersection.h>
#include <navigine/geometry/geolib/detail/iterate.h>
#include <navigine/geometry/geolib/detail/num_points.h>
#include <navigine/geometry/geolib/detail/point_helper.h>
#include <navigine/geometry/geolib/detail/type_checks.h>
#include <navigine/geometry/geolib/detail/util.h>
#include <navigine/geometry/geolib/detail/validity.h>
#include <navigine/geometry/geolib/detail/wkt_read.h>
#include <navigine/geometry/geolib/detail/wkt_write.h>

namespace navigine::geometry::geolib {

template <typename Point>
struct Segment
{
    using point_type = Point;
    static constexpr auto geometry_type = "SEGMENT";
    point_type first;
    point_type second;
};

template <typename Point>
struct Box
{
    using point_type = Point;
    static constexpr auto geometry_type = "BOX";
    point_type minCorner;
    point_type maxCorner;
};

template <typename Point>
struct LineString
{
    using point_type = Point;
    static constexpr auto geometry_type = "LINESTRING";
    std::vector<point_type> points;
};

template <typename Point>
struct Ring
{
    using point_type = Point;
    static constexpr auto geometry_type = "LINEARRING";
    std::vector<point_type> points;
};

template <typename Point>
struct Polygon
{
    using point_type = Point;
    using ring_type = Ring<Point>;
    static constexpr auto geometry_type = "POLYGON";
    ring_type outer;
    std::vector<ring_type> inners;
};

template <typename Point>
struct MultiPolygon
{
    using point_type = Point;
    using ring_type = Ring<Point>;
    using polygon_type = Polygon<Point>;
    static constexpr auto geometry_type = "MULTIPOLYGON";
    std::vector<polygon_type> parts;
};

template <typename Geometry>
unsigned int numPoints(const Geometry& geometry)
{
    return detail::numPoints(geometry);
}

template <typename Geometry>
bool isEmpty(const Geometry& geometry)
{
    return detail::numPoints(geometry) == 0;
}

template <typename Geometry>
double area(const Geometry& geometry)
{
    return detail::area(geometry);
}

template <typename Geometry>
typename Geometry::point_type centroid(const Geometry& geometry)
{
    return detail::centroid(geometry);
}

template <typename Geometry>
std::string wkt(const Geometry& geometry, int precision = -1, bool fixed = false)
{
    return detail::wkt(geometry, precision, fixed);
}

template <typename Geometry>
bool readWkt(const std::string& wkt, Geometry& geometry)
{
    return detail::readWkt(wkt, geometry);
}

template <typename Geometry>
void correct(Geometry& geometry)
{
    detail::correct(geometry);
}

template <typename Geometry>
int compare(const Geometry& geometry1, const Geometry& geometry2)
{
    return detail::compare(geometry1, geometry2);
}

template <typename Geometry>
bool equal(const Geometry& geometry1, const Geometry& geometry2, double epsilon = 0.0)
{
    return detail::equal(geometry1, geometry2, epsilon);
}

template <typename Geometry>
bool less(const Geometry& geometry1, const Geometry& geometry2)
{
    return detail::less(geometry1, geometry2);
}

template <typename Geometry>
bool greater(const Geometry& geometry1, const Geometry& geometry2)
{
    return detail::greater(geometry1, geometry2);
}

template <typename Geometry1, typename Geometry2>
bool contains(const Geometry1& geometry1, const Geometry2& geometry2)
{
    return detail::contains(geometry1, geometry2);
}

template <typename Geometry>
Box<typename Geometry::point_type> boundingBox(const Geometry& geometry)
{
    return detail::boundingBox<Box<typename Geometry::point_type>>(geometry);
}

template <typename Geometry>
bool isValid(const Geometry& geometry, std::string& message)
{
    return detail::isValid(geometry, message);
}

template <typename Point>
std::vector<Point> convexHull(const std::vector<Point>& points)
{
    return detail::convexHull<Point>(points);
}

template <typename Geometry1, typename Geometry2>
MultiPolygon<typename Geometry1::point_type> intersection(const Geometry1& geometry1, const Geometry2& geometry2)
{
    return detail::binaryOperation<MultiPolygon<typename Geometry1::point_type>>(
        detail::BinaryOperationType::INTERSECTION, geometry1, geometry2);
}

template <typename Geometry1, typename Geometry2>
MultiPolygon<typename Geometry1::point_type> union_(const Geometry1& geometry1, const Geometry2& geometry2)
{
    return detail::binaryOperation<MultiPolygon<typename Geometry1::point_type>>(
        detail::BinaryOperationType::UNION, geometry1, geometry2);
}

template <typename Geometry1, typename Geometry2>
MultiPolygon<typename Geometry1::point_type> difference(const Geometry1& geometry1, const Geometry2& geometry2)
{
    return detail::binaryOperation<MultiPolygon<typename Geometry1::point_type>>(
        detail::BinaryOperationType::DIFFERENCE, geometry1, geometry2);
}

template <typename Geometry>
MultiPolygon<typename Geometry::point_type> buffer(const Geometry& geometry, double radius, unsigned int numPointsSemiCircle = 18)
{
    return detail::buffer<MultiPolygon<typename Geometry::point_type>>(geometry, radius, numPointsSemiCircle, true);
}

} // namespace navigine::geometry::geolib
