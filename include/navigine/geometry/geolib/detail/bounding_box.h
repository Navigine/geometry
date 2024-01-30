#pragma once

#include <navigine/geometry/geolib/detail/type_checks.h>
#include <navigine/geometry/geolib/detail/util.h>

namespace navigine::geometry::geolib::detail {

template <typename Box, typename Geometry>
Box boundingBox(const Geometry& geometry)
{
    static_assert(
        isSegment<Geometry>() ||
        isBox<Geometry>() ||
        isLineString<Geometry>() ||
        isRing<Geometry>() ||
        isPolygon<Geometry>() ||
        isMultiPolygon<Geometry>());

    Box box;

    if constexpr (isSegment<Geometry>()) {
        util::getX(box.minCorner) = std::min(util::getX(geometry.first), util::getX(geometry.second));
        util::getY(box.minCorner) = std::min(util::getY(geometry.first), util::getY(geometry.second));
        util::getX(box.maxCorner) = std::max(util::getX(geometry.first), util::getX(geometry.second));
        util::getY(box.maxCorner) = std::max(util::getY(geometry.first), util::getY(geometry.second));
        return box;
    }

    if constexpr (isBox<Geometry>()) {
        box.minCorner = geometry.minCorner;
        box.maxCorner = geometry.maxCorner;
        return box;
    }

    util::getX(box.minCorner) = std::numeric_limits<double>::infinity();
    util::getY(box.minCorner) = std::numeric_limits<double>::infinity();
    util::getX(box.maxCorner) = -std::numeric_limits<double>::infinity();
    util::getY(box.maxCorner) = -std::numeric_limits<double>::infinity();

    const auto iteratePoints =
        [&box](const auto& points) {
            for(const auto& p : points) {
                util::getX(box.minCorner) = std::min(util::getX(box.minCorner), util::getX(p));
                util::getY(box.minCorner) = std::min(util::getY(box.minCorner), util::getY(p));
                util::getX(box.maxCorner) = std::max(util::getX(box.maxCorner), util::getX(p));
                util::getY(box.maxCorner) = std::max(util::getY(box.maxCorner), util::getY(p));
            }
        };

    if constexpr (isLineString<Geometry>() || isRing<Geometry>()) {
        iteratePoints(geometry.points);
    } else if constexpr (isPolygon<Geometry>()) {
        iteratePoints(geometry.outer.points);
    } else if constexpr (isMultiPolygon<Geometry>()) {
        for(const auto& part : geometry.parts) {
            iteratePoints(part.outer.points);
        }
    }

    return box;
}

} // namespace navigine::geometry::geolib::detail
