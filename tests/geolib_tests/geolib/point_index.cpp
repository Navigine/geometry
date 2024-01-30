#include <navigine/geometry/geometry.h>

using namespace navigine::geometry;

namespace {

const auto PRECISION = 12;

int test_geolib_point_index(
    const std::string& wkt,
    const XYPoint& point,
    const int expectedIndex)
{
    XYMultiPolygon geom;

    std::cout << "test_geolib_point_index: " << geolib::wkt(point, PRECISION) << std::endl;

    if (!readWkt(wkt, geom)) {
        std::cout << "Error parsing wkt string: " << wkt << std::endl;
        return -1;
    }

    geolib::correct(geom);
    std::cout << geolib::wkt(geom, PRECISION) << std::endl;

    auto index = geolib::detail::pointIndex(geom, point);

    if (index != expectedIndex) {
        std::cout << "result:   " << index << std::endl;
        std::cout << "expected: " << expectedIndex << std::endl;
        return 1;
    }

    return 0;
}

} // namespace

int test_geolib_point_index_1()
{
    // Outer point
    return test_geolib_point_index(
        "MULTIPOLYGON(((1 0, 3 0, 3 3, 0 3, 0 1, 1 1, 1 0), (1 1, 1 2, 2 2, 2 1, 1 1)))",
        XYPoint{0, 0}, -1);
}

int test_geolib_point_index_2()
{
    // Outer point
    return test_geolib_point_index(
        "MULTIPOLYGON(((1 0, 3 0, 3 3, 0 3, 0 1, 1 1, 1 0), (1 1, 1 2, 2 2, 2 1, 1 1)))",
        XYPoint{-1, 1}, -1);
}

int test_geolib_point_index_3()
{
    // Outer point
    return test_geolib_point_index(
        "MULTIPOLYGON(((1 0, 3 0, 3 3, 0 3, 0 1, 1 1, 1 0), (1 1, 1 2, 2 2, 2 1, 1 1)))",
        XYPoint{1.5, 1.5}, -1);
}

int test_geolib_point_index_4()
{
    // Border point
    return test_geolib_point_index(
        "MULTIPOLYGON(((1 0, 3 0, 3 3, 0 3, 0 1, 1 1, 1 0), (1 1, 1 2, 2 2, 2 1, 1 1)))",
        XYPoint{0.5, 1}, 0);
}

int test_geolib_point_index_5()
{
    // Border point
    return test_geolib_point_index(
        "MULTIPOLYGON(((1 0, 3 0, 3 3, 0 3, 0 1, 1 1, 1 0), (1 1, 1 2, 2 2, 2 1, 1 1)))",
        XYPoint{1, 1}, 0);
}

int test_geolib_point_index_6()
{
    // Border point
    return test_geolib_point_index(
        "MULTIPOLYGON(((1 0, 3 0, 3 3, 0 3, 0 1, 1 1, 1 0), (1 1, 1 2, 2 2, 2 1, 1 1)))",
        XYPoint{1.5, 2}, 0);
}

int test_geolib_point_index_7()
{
    // Inner point
    return test_geolib_point_index(
        "MULTIPOLYGON(((1 0, 3 0, 3 3, 0 3, 0 1, 1 1, 1 0), (1 1, 1 2, 2 2, 2 1, 1 1)))",
        XYPoint{0.5, 2}, 1);
}

int test_geolib_point_index_8()
{
    // Inner point
    return test_geolib_point_index(
        "MULTIPOLYGON(((1 0, 3 0, 3 3, 0 3, 0 1, 1 1, 1 0), (1 1, 1 2, 2 2, 2 1, 1 1)))",
        XYPoint{2, 0.5}, 1);
}

int test_geolib_point_index_9()
{
    // Inner point
    return test_geolib_point_index(
        "MULTIPOLYGON(((1 0, 3 0, 3 3, 0 3, 0 1, 1 1, 1 0), (1 1, 1 2, 2 2, 2 1, 1 1)))",
        XYPoint{2.1, 2.1}, 1);
}
