#pragma once

#include <navigine/geometry/geolib/detail/type_checks.h>
#include <navigine/geometry/geolib/detail/util.h>

namespace navigine::geometry::geolib::detail {

template <typename Ring>
double ringArea(const Ring& ring)
{
    static_assert(isRing<Ring>());

    double area = 0.0;
    for(size_t i = 0; i + 1 < ring.points.size(); ++i) {
        const auto& p1 = ring.points[i];
        const auto& p2 = ring.points[i+1];
        area += (util::getX(p1) - util::getX(p2)) * (util::getY(p1) + util::getY(p2)) / 2;
    }

    return area;
}

template <typename Geometry>
double area(Geometry& geometry)
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
        return 0.0;
    } else if constexpr (isSegment<Geometry>() || isLineString<Geometry>()) {
        return 0.0;
    } else if constexpr (isBox<Geometry>()) {
        return (util::getX(geometry.maxCorner) - util::getX(geometry.minCorner)) *
               (util::getY(geometry.maxCorner) - util::getY(geometry.minCorner));
    } else if constexpr (isRing<Geometry>()) {
        return ringArea(geometry);
    } else if constexpr (isPolygon<Geometry>()) {
        double area = ringArea(geometry.outer);
        for(const auto& inner : geometry.inners) {
            area += ringArea(inner);
        }
        return area;
    } else if constexpr (isMultiPolygon<Geometry>()) {
        double area = 0.0;
        for(const auto& part : geometry.parts) {
            area += ringArea(part.outer);
            for(const auto& inner : part.inners) {
                area += ringArea(inner);
            }
        }
        return area;
    }
}

} // namespace navigine::geometry::geolib::detail
