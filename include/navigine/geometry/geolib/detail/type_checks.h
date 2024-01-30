#pragma once

#include <navigine/geometry/geolib/detail/util.h>

#define GEOLIB_GEOMETRY_HAS_FIELD(Geometry, field) (requires(const Geometry& t) { t.field; } || false)
#define GEOLIB_GEOMETRY_HAS_FUNCTION(Geometry, function) (requires(const Geometry& t) { function(t); } || false)

namespace navigine::geometry::geolib::detail {

template <typename Geometry>
constexpr bool isPoint()
{
    return GEOLIB_GEOMETRY_HAS_FUNCTION(Geometry, util::getX) &&
           GEOLIB_GEOMETRY_HAS_FUNCTION(Geometry, util::getY);
}

template <typename Geometry>
constexpr bool isSegment()
{
    if constexpr (GEOLIB_GEOMETRY_HAS_FIELD(Geometry, geometry_type)) {
        return Geometry::geometry_type == std::string_view("SEGMENT");
    } else {
        return false;
    }
}

template <typename Geometry>
constexpr bool isBox()
{
    if constexpr (GEOLIB_GEOMETRY_HAS_FIELD(Geometry, geometry_type)) {
        return Geometry::geometry_type == std::string_view("BOX");
    } else {
        return false;
    }
}

template <typename Geometry>
constexpr bool isLineString()
{
    if constexpr (GEOLIB_GEOMETRY_HAS_FIELD(Geometry, geometry_type)) {
        return Geometry::geometry_type == std::string_view("LINESTRING");
    } else {
        return false;
    }
}

template <typename Geometry>
constexpr bool isRing()
{
    if constexpr (GEOLIB_GEOMETRY_HAS_FIELD(Geometry, geometry_type)) {
        return Geometry::geometry_type == std::string_view("LINEARRING");
    } else {
        return false;
    }
}

template <typename Geometry>
constexpr bool isPolygon()
{
    if constexpr (GEOLIB_GEOMETRY_HAS_FIELD(Geometry, geometry_type)) {
        return Geometry::geometry_type == std::string_view("POLYGON");
    } else {
        return false;
    }
}

template <typename Geometry>
constexpr bool isMultiPolygon()
{
    if constexpr (GEOLIB_GEOMETRY_HAS_FIELD(Geometry, geometry_type)) {
        return Geometry::geometry_type == std::string_view("MULTIPOLYGON");
    } else {
        return false;
    }
}

template <class T>
concept IsPoint = isPoint<T>();

} // namespace navigine::geometry::geolib::detail

#undef GEOLIB_GEOMETRY_HAS_FIELD
#undef GEOLIB_GEOMETRY_HAS_FUNCTION
