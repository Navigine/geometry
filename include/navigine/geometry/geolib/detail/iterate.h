#pragma once

#include <navigine/geometry/geolib/detail/type_checks.h>
#include <navigine/geometry/geolib/detail/util.h>

namespace navigine::geometry::geolib::detail {

template <typename Geometry>
void iterateVertices(
    const Geometry& geometry,
    const bool reverse,
    const std::function<void(
        const typename Geometry::point_type& point,
        size_t index,
        size_t ringSize)>& func)
{
    static_assert(
        isBox<Geometry>() ||
        isRing<Geometry>() ||
        isPolygon<Geometry>() ||
        isMultiPolygon<Geometry>());

    using Point = typename Geometry::point_type;

    size_t shift = 0;

    const auto iteratePoints =
        [&](const std::vector<Point>& points) {
            for(size_t i = 0; i < points.size(); ++i) {
                const auto index = reverse ? points.size() - i - 1 : i;
                func(points[index], shift + i, i + 1 < points.size() ? 0 : points.size());
            }
            shift += points.size();
        };

    if constexpr (isBox<Geometry>()) {
        Point p1, p2, p3, p4;
        util::getX(p1) = util::getX(geometry.minCorner);
        util::getY(p1) = util::getY(geometry.minCorner);
        util::getX(p2) = util::getX(geometry.maxCorner);
        util::getY(p2) = util::getY(geometry.minCorner);
        util::getX(p3) = util::getX(geometry.maxCorner);
        util::getY(p3) = util::getY(geometry.maxCorner);
        util::getX(p4) = util::getX(geometry.minCorner);
        util::getY(p4) = util::getY(geometry.maxCorner);
        iteratePoints({p1, p2, p3, p4, p1});
    } else if constexpr (isRing<Geometry>()) {
        iteratePoints(geometry.points);
    } else if constexpr (isPolygon<Geometry>()) {
        iteratePoints(geometry.outer.points);
        for(const auto& inner : geometry.inners) {
            iteratePoints(inner.points);
        }
    } else if constexpr (isMultiPolygon<Geometry>()) {
        for(const auto& poly : geometry.parts) {
            iteratePoints(poly.outer.points);
            for(const auto& inner : poly.inners) {
                iteratePoints(inner.points);
            }
        }
    }
}

template <typename Geometry>
void iterateEdges(
    const Geometry& geometry,
    const bool reverse,
    const std::function<void(
        const typename Geometry::point_type& point1,
        const typename Geometry::point_type& point2,
        const typename Geometry::point_type& prevPoint,
        const typename Geometry::point_type& nextPoint)>& func)
{
    static_assert(
        isBox<Geometry>() ||
        isRing<Geometry>() ||
        isPolygon<Geometry>() ||
        isMultiPolygon<Geometry>());

    using Point = typename Geometry::point_type;

    const auto iteratePoints =
        [&](const std::vector<Point>& points) {
            //const auto n = points.size() - 1;
            //for(size_t i = 0; i < n; ++i) {
            //    const auto index1 = reverse ? n - i : i;
            //    const auto index2 = reverse ? n - i - 1 : i + 1;
            //    const auto prevIndex = reverse ? n - (n + i - 1) % n : (n + i - 1) % n;
            //    func(points[index1], points[index2], shift + i);
            //}

            const auto n = points.size() - 1;

            for(size_t i = 0; i < n; ++i) {
                auto index1 = i;
                auto index2 = i + 1;
                auto prevIndex = (i + n - 1) % n;
                auto nextIndex = (i + 2) % n;
                if (reverse) {
                    index1 = n - index1;
                    index2 = n - index2;
                    prevIndex = n - prevIndex;
                    nextIndex = n - nextIndex;
                }
                func(points[index1], points[index2], points[prevIndex], points[nextIndex]);
            }
            //shift += points.size();
        };

    if constexpr (isBox<Geometry>()) {
        Point p1, p2, p3, p4;
        util::getX(p1) = util::getX(geometry.minCorner);
        util::getY(p1) = util::getY(geometry.minCorner);
        util::getX(p2) = util::getX(geometry.maxCorner);
        util::getY(p2) = util::getY(geometry.minCorner);
        util::getX(p3) = util::getX(geometry.maxCorner);
        util::getY(p3) = util::getY(geometry.maxCorner);
        util::getX(p4) = util::getX(geometry.minCorner);
        util::getY(p4) = util::getY(geometry.maxCorner);
        iteratePoints({p1, p2, p3, p4, p1});
    } else if constexpr (isRing<Geometry>()) {
        iteratePoints(geometry.points);
    } else if constexpr (isPolygon<Geometry>()) {
        iteratePoints(geometry.outer.points);
        for(const auto& inner : geometry.inners) {
            iteratePoints(inner.points);
        }
    } else if constexpr (isMultiPolygon<Geometry>()) {
        for(const auto& poly : geometry.parts) {
            iteratePoints(poly.outer.points);
            for(const auto& inner : poly.inners) {
                iteratePoints(inner.points);
            }
        }
    }
}

} // namespace navigine::geometry::geolib::detail
