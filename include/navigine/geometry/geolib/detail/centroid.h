#pragma once

#include <navigine/geometry/geolib/detail/type_checks.h>
#include <navigine/geometry/geolib/detail/util.h>

namespace navigine::geometry::geolib::detail {

template <typename Geometry>
typename Geometry::point_type centroid(const Geometry& geometry)
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
        return geometry;
    }

    typename Geometry::point_type result;

    if constexpr (isSegment<Geometry>()) {
        util::getX(result) = (util::getX(geometry.first) + util::getX(geometry.second)) / 2;
        util::getY(result) = (util::getY(geometry.first) + util::getY(geometry.second)) / 2;
        return result;
    } else if constexpr (isBox<Geometry>()) {
        util::getX(result) = (util::getX(geometry.minCorner) + util::getX(geometry.maxCorner)) / 2;
        util::getY(result) = (util::getY(geometry.minCorner) + util::getY(geometry.maxCorner)) / 2;
        return result;
    }

    util::getX(result) = 0.0;
    util::getY(result) = 0.0;
    size_t numPoints = 0;

    const auto iteratePoints =
        [&](const auto& points) {
            for(const auto& p : points) {
                util::getX(result) += util::getX(p);
                util::getY(result) += util::getY(p);
            }
            numPoints += points.size();
        };

    if constexpr (isLineString<Geometry>() || isRing<Geometry>()) {
        iteratePoints(geometry.points);
    } else if constexpr (isPolygon<Geometry>()) {
        iteratePoints(geometry.outer.points);
        for(const auto& inner : geometry.inners) {
            iteratePoints(inner.points);
        }
    } else if constexpr (isMultiPolygon<Geometry>()) {
        for(const auto& part : geometry.parts) {
            iteratePoints(part.outer.points);
            for(const auto& inner : part.inners) {
                iteratePoints(inner.points);
            }
        }
    }

    assert(numPoints > 0);
    util::getX(result) /= numPoints;
    util::getY(result) /= numPoints;
    return result;
}

} // namespace navigine::geometry::geolib::detail
