#include <navigine/geometry/geometry.h>

using namespace navigine::geometry;

namespace {

const int PRECISION = 20;

template <typename Geometry>
int test_geolib_read_wkt_geometry(
    const std::string& wkt,
    const Geometry& expectedGeom)
{
    Geometry geom;

    std::cout << "readWkt: " << wkt << std::endl;

    if (!geolib::readWkt(wkt, geom)) {
        std::cout << "Error parsing wkt string" << std::endl;
        return 1;
    }

    if (!geolib::equal(geom, expectedGeom)) {
        std::cout << "geolib: " << geolib::wkt(geom, PRECISION) << std::endl;
        std::cout << "expected: " << geolib::wkt(expectedGeom, PRECISION) << std::endl;
        return 1;
    }

    return 0;
}

} // namespace

int test_geolib_read_wkt_point()
{
    return test_geolib_read_wkt_geometry(
        "POINT(1.23456789 -3.987654321e2)",
        XYPoint{1.23456789, -3.987654321e2});
}

int test_geolib_read_wkt_segment()
{
    return test_geolib_read_wkt_geometry(
        "SEGMENT(1 2, 3 4)",
        XYSegment{.first = XYPoint{1, 2}, .second = XYPoint{3, 4}});
}

int test_geolib_read_wkt_ring()
{
    XYRing ring;

    ring.points.push_back(XYPoint{0, 4});
    ring.points.push_back(XYPoint{2, 2});
    ring.points.push_back(XYPoint{0, 0});
    ring.points.push_back(XYPoint{5, 0});
    ring.points.push_back(XYPoint{5, 4});
    ring.points.push_back(XYPoint{0, 4});

    return test_geolib_read_wkt_geometry(
        "LINEARRING(0 4, 2 2, 0 0, 5 0, 5 4, 0 4)",
        ring);
}

int test_geolib_read_wkt_polygon()
{
    XYPolygon poly;

    poly.outer.points.push_back(XYPoint{0, 4});
    poly.outer.points.push_back(XYPoint{0, 0});
    poly.outer.points.push_back(XYPoint{4, 0});
    poly.outer.points.push_back(XYPoint{4, 4});
    poly.outer.points.push_back(XYPoint{0, 4});

    poly.inners.push_back({});
    poly.inners.back().points.push_back(XYPoint{1, 1});
    poly.inners.back().points.push_back(XYPoint{1, 3});
    poly.inners.back().points.push_back(XYPoint{3, 3});
    poly.inners.back().points.push_back(XYPoint{3, 1});
    poly.inners.back().points.push_back(XYPoint{1, 1});

    return test_geolib_read_wkt_geometry(
        "POLYGON((0 4, 0 0, 4 0, 4 4, 0 4), (1 1, 1 3, 3 3, 3 1, 1 1))",
        poly);
}

int test_geolib_read_wkt_multipolygon()
{
    XYMultiPolygon geom;

    geom.parts.resize(3);

    geom.parts[0].outer.points.push_back(XYPoint{0, 0});
    geom.parts[0].outer.points.push_back(XYPoint{2, 0});
    geom.parts[0].outer.points.push_back(XYPoint{1.33, 1.33});
    geom.parts[0].outer.points.push_back(XYPoint{0, 0});

    geom.parts[1].outer.points.push_back(XYPoint{4, 0});
    geom.parts[1].outer.points.push_back(XYPoint{5, 0});
    geom.parts[1].outer.points.push_back(XYPoint{5, 4});
    geom.parts[1].outer.points.push_back(XYPoint{4, 0});

    geom.parts[2].outer.points.push_back(XYPoint{4, 3});
    geom.parts[2].outer.points.push_back(XYPoint{5, 4});
    geom.parts[2].outer.points.push_back(XYPoint{4, 4});
    geom.parts[2].outer.points.push_back(XYPoint{4, 3});

    return test_geolib_read_wkt_geometry(
        "MULTIPOLYGON(((0 0, 2 0, 1.33 1.33, 0 0)), ((4 0, 5 0, 5 4, 4 0)), ((4 3, 5 4, 4 4, 4 3)))",
        geom);
}
