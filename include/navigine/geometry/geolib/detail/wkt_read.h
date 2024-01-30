#pragma once

#include <navigine/geometry/geolib/detail/type_checks.h>
#include <navigine/geometry/geolib/detail/util.h>

namespace navigine::geometry::geolib::detail {

inline int skipWhitespaces(std::istream& is)
{
    int count = 0;
    while (is.peek() == ' ') {
        is.get();
        ++count;
    }
    return count;
}

inline double readDouble(std::istream& is)
{
    double result = 0.0;
    is >> result;
    if (is.fail()) {
        throw std::runtime_error("readDouble: parse error at " + std::to_string(is.tellg()));
    }
    return result;
}

inline void readSymbol(std::istream& is, char c)
{
    if (is.peek() != c) {
        throw std::runtime_error("readSymbol: parse error at " + std::to_string(is.tellg()));
    }
    is.get();
}

inline void readSymbols(std::istream& is, const std::string& symbols)
{
    for(auto c : symbols) {
        readSymbol(is, c);
    }
}

template <typename Point>
Point readPoint(std::istream& is)
{
    static_assert(isPoint<Point>());

    skipWhitespaces(is);
    double x = readDouble(is);
    readSymbol(is, ' ');
    skipWhitespaces(is);
    double y = readDouble(is);

    Point result;
    util::getX(result) = x;
    util::getY(result) = y;
    return result;
}

template <typename Segment>
Segment readSegment(std::istream& is)
{
    static_assert(isSegment<Segment>());

    using Point = typename Segment::point_type;

    Segment result;

    skipWhitespaces(is);

    readSymbol(is, '(');

    result.first = readPoint<Point>(is);
    skipWhitespaces(is);
    readSymbol(is, ',');
    result.second = readPoint<Point>(is);

    skipWhitespaces(is);
    readSymbol(is, ')');
    return result;
}

template <typename LineString>
LineString readLineString(std::istream& is)
{
    static_assert(isLineString<LineString>());

    using Point = typename LineString::point_type;

    LineString result;

    skipWhitespaces(is);

    readSymbol(is, '(');
    result.points.push_back(readPoint<Point>(is));

    while (true) {
        const auto pos = is.tellg();
        try {
            skipWhitespaces(is);
            readSymbol(is, ',');
            result.points.push_back(readPoint<Point>(is));
        } catch (std::runtime_error& e) {
            is.seekg(pos);
            break;
        }
    }

    skipWhitespaces(is);
    readSymbol(is, ')');
    return result;
}

template <typename Ring>
Ring readRing(std::istream& is)
{
    static_assert(isRing<Ring>());

    using Point = typename Ring::point_type;

    Ring result;

    skipWhitespaces(is);

    readSymbol(is, '(');
    result.points.push_back(readPoint<Point>(is));

    while (true) {
        const auto pos = is.tellg();
        try {
            skipWhitespaces(is);
            readSymbol(is, ',');
            result.points.push_back(readPoint<Point>(is));
        } catch (std::runtime_error& e) {
            is.seekg(pos);
            break;
        }
    }

    skipWhitespaces(is);
    readSymbol(is, ')');
    return result;
}

template <typename Polygon>
Polygon readPolygon(std::istream& is)
{
    static_assert(isPolygon<Polygon>());

    using Ring = typename Polygon::ring_type;

    Polygon result;

    skipWhitespaces(is);

    readSymbol(is, '(');
    result.outer = readRing<Ring>(is);

    while (true) {
        const auto pos = is.tellg();
        try {
            skipWhitespaces(is);
            readSymbol(is, ',');
            result.inners.push_back(readRing<Ring>(is));
        } catch (std::runtime_error& e) {
            is.seekg(pos);
            break;
        }
    }

    skipWhitespaces(is);
    readSymbol(is, ')');
    return result;
}

template <typename MultiPolygon>
MultiPolygon readMultiPolygon(std::istream& is)
{
    static_assert(isMultiPolygon<MultiPolygon>());

    using Polygon = typename MultiPolygon::polygon_type;

    MultiPolygon result;

    skipWhitespaces(is);
    readSymbol(is, '(');

    while (true) {
        const auto pos = is.tellg();
        try {
            if (!result.parts.empty()) {
                skipWhitespaces(is);
                readSymbol(is, ',');
            }
            result.parts.push_back(readPolygon<Polygon>(is));
        } catch (std::runtime_error& e) {
            is.seekg(pos);
            break;
        }
    }

    skipWhitespaces(is);
    readSymbol(is, ')');
    return result;
}

template <typename Geometry>
bool readWkt(const std::string& wkt, Geometry& geometry)
{
    static_assert(
        isPoint<Geometry>() ||
        isSegment<Geometry>() ||
        isLineString<Geometry>() ||
        isRing<Geometry>() ||
        isPolygon<Geometry>() ||
        isMultiPolygon<Geometry>());

    std::istringstream is(wkt);

    detail::skipWhitespaces(is);

    try {
        if constexpr (isPoint<Geometry>()) {
            detail::readSymbols(is, "POINT");
            detail::skipWhitespaces(is);
            detail::readSymbol(is, '(');
            geometry = detail::readPoint<Geometry>(is);
            detail::skipWhitespaces(is);
            detail::readSymbol(is, ')');
        } else if constexpr (isSegment<Geometry>()) {
            detail::readSymbols(is, Geometry::geometry_type);
            geometry = detail::readSegment<Geometry>(is);
        } else if constexpr (isLineString<Geometry>()) {
            detail::readSymbols(is, Geometry::geometry_type);
            geometry = detail::readLineString<Geometry>(is);
        } else if constexpr (isRing<Geometry>()) {
            detail::readSymbols(is, Geometry::geometry_type);
            geometry = detail::readRing<Geometry>(is);
        } else if constexpr (isPolygon<Geometry>()) {
            detail::readSymbols(is, Geometry::geometry_type);
            geometry = detail::readPolygon<Geometry>(is);
        } else {
            static_assert(isMultiPolygon<Geometry>());
            detail::readSymbols(is, Geometry::geometry_type);
            geometry = detail::readMultiPolygon<Geometry>(is);
        }
    } catch (std::runtime_error& e) {
        std::cout << e.what() << "\n";
        return false;
    }

    detail::skipWhitespaces(is);

    return is.eof();
}

} // namespace navigine::geometry::geolib::detail
