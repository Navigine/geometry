#include <navigine/geometry/geometry.h>

using namespace navigine::geometry;

namespace {

const auto PRECISION = 12;

int test_geolib_contains_segment(
    const std::string& wkt1,
    const std::string& wkt2,
    bool expectedContains)
{
    XYMultiPolygon geom1;
    XYLineString geom2;

    std::cout << "test_geolib_contains_segment" << std::endl;

    if (!readWkt(wkt1, geom1)) {
        std::cout << "Error parsing wkt string: " << wkt1 << std::endl;
        return -1;
    }

    geolib::correct(geom1);
    std::cout << geolib::wkt(geom1, PRECISION) << std::endl;

    if (!readWkt(wkt2, geom2)) {
        std::cout << "Error parsing wkt string: " << wkt2 << std::endl;
        return -1;
    }

    std::cout << geolib::wkt(geom2, PRECISION) << std::endl;

    auto contains = geolib::detail::contains(geom1, geom2);

    if (contains != expectedContains) {
        std::cout << "result:   " << contains << std::endl;
        std::cout << "expected: " << expectedContains << std::endl;
        return 1;
    }

    return 0;
}

} // namespace

int test_geolib_contains_segment_1()
{
    return test_geolib_contains_segment(
        "MULTIPOLYGON(((1 0, 3 0, 3 3, 0 3, 0 1, 1 1, 1 0), (1 1, 1 2, 2 2, 2 1, 1 1)))",
        "LINESTRING(0.5 1.5, 0.5 2.5)",
        true);
}

int test_geolib_contains_segment_2()
{
    return test_geolib_contains_segment(
        "MULTIPOLYGON(((1 0, 3 0, 3 3, 0 3, 0 1, 1 1, 1 0), (1 1, 1 2, 2 2, 2 1, 1 1)))",
        "LINESTRING(0.5 2, 2.5 2)",
        true);
}

int test_geolib_contains_segment_3()
{
    return test_geolib_contains_segment(
        "MULTIPOLYGON(((1 0, 3 0, 3 3, 0 3, 0 1, 1 1, 1 0), (1 1, 1 2, 2 2, 2 1, 1 1)))",
        "LINESTRING(0 1, 2.001 3)",
        false);
}

int test_geolib_contains_segment_4()
{
    return test_geolib_contains_segment(
        "MULTIPOLYGON(((0 0, 3 0, 3 2, 2 2, 2 1, 1 1, 1 2, 0 2, 0 0)))",
        "LINESTRING(0 1, 3 1)",
        true);
}

int test_geolib_contains_segment_5()
{
    return test_geolib_contains_segment(
        "MULTIPOLYGON(((0 0, 3 0, 3 2, 2 2, 2 1, 1 1, 1 2, 0 2, 0 0)))",
        "LINESTRING(0 2, 3 2)",
        false);
}

int test_geolib_contains_segment_6()
{
    return test_geolib_contains_segment(
        "MULTIPOLYGON(((0 0, 3 0, 3 2, 2 2, 2 1, 1 1, 1 2, 0 2, 0 0)))",
        "LINESTRING(0 0, 1 0, 2 0, 3 0, 3 2, 2 2, 2 1, 1 1, 1 2, 0 2)",
        true);
}
