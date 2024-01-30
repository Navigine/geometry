#pragma once

#include <navigine/geometry/geolib/detail/area.h>
#include <navigine/geometry/geolib/detail/compare.h>
#include <navigine/geometry/geolib/detail/type_checks.h>
#include <navigine/geometry/geolib/detail/util.h>

namespace navigine::geometry::geolib::detail {

template <typename Ring>
bool checkClockwiseRing(const Ring& ring)
{
    static_assert(isRing<Ring>());
    return ringArea(ring) < 0;
}

template <typename Ring>
void correctRing(Ring& ring, bool isOuter)
{
    static_assert(isRing<Ring>());

    if (ring.points.size() < 3) {
        return;
    }

    if (!detail::equal(ring.points.back(), ring.points.front())) {
        ring.points.push_back(ring.points.front());
    }

    if (checkClockwiseRing(ring) == isOuter) {
        // Invalid ring orientation
        std::reverse(ring.points.begin(), ring.points.end());
    }
}

template <typename Polygon>
void correctPolygon(Polygon& poly)
{
    static_assert(isPolygon<Polygon>());

    correctRing(poly.outer, true);
    for(auto& inner : poly.inners) {
        correctRing(inner, false);
    }
}

template <typename MultiPolygon>
void correctMultiPolygon(MultiPolygon& geom)
{
    static_assert(isMultiPolygon<MultiPolygon>());

    for(auto& part : geom.parts) {
        correctPolygon(part);
    }
}

template <typename Geometry>
void correct(Geometry& geometry)
{
    static_assert(
        isPoint<Geometry>() ||
        isSegment<Geometry>() ||
        isBox<Geometry>() ||
        isLineString<Geometry>() ||
        isRing<Geometry>() ||
        isPolygon<Geometry>() ||
        isMultiPolygon<Geometry>());

    if constexpr (isRing<Geometry>()) {
        detail::correctRing(geometry, true);
    } else if constexpr (isPolygon<Geometry>()) {
        detail::correctPolygon(geometry);
    } else if constexpr (isMultiPolygon<Geometry>()) {
        detail::correctMultiPolygon(geometry);
    }
}

template <typename Ring>
void sortRing(Ring& ring)
{
    static_assert(isRing<Ring>());

    if (ring.points.size() < 3) {
        return;
    }

    // Rearranging points inside ring (so that min point comes first)
    auto first = ring.points.begin();
    auto last = std::next(first, ring.points.size() - 1);
    std::rotate(first, std::min_element(first, last, less<typename Ring::point_type>), last);
    ring.points.back() = ring.points.front();
}

template <typename Polygon>
void sortPolygon(Polygon& poly)
{
    static_assert(isPolygon<Polygon>());

    sortRing(poly.outer);
    for (auto& ring : poly.inners) {
        sortRing(ring);
    }

    std::stable_sort(
        poly.inners.begin(),
        poly.inners.end(),
        less<typename Polygon::ring_type>);
}

template <typename MultiPolygon>
void sortMultiPolygon(MultiPolygon& geom)
{
    static_assert(isMultiPolygon<MultiPolygon>());

    for (auto& poly : geom.parts) {
        sortPolygon(poly);
    }

    std::stable_sort(
        geom.parts.begin(),
        geom.parts.end(),
        less<typename MultiPolygon::polygon_type>);
}

template <typename Geometry>
void sort(Geometry& geometry)
{
    static_assert(
        isPoint<Geometry>() ||
        isSegment<Geometry>() ||
        isBox<Geometry>() ||
        isLineString<Geometry>() ||
        isRing<Geometry>() ||
        isPolygon<Geometry>() ||
        isMultiPolygon<Geometry>());

    if constexpr (isRing<Geometry>()) {
        detail::sortRing(geometry);
    } else if constexpr (isPolygon<Geometry>()) {
        detail::sortPolygon(geometry);
    } else if constexpr (isMultiPolygon<Geometry>()) {
        detail::sortMultiPolygon(geometry);
    }
}

} // namespace navigine::geometry::geolib::detail
