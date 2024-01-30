#include <navigine/geometry/geometry.h>
#include <navigine/geometry/geolib/detail/correct.h>

#include "wkt_examples.h"

using namespace navigine::geometry;

namespace {

const auto PRECISION = 12;
const auto EPSILON = 1e-10;

int test_geolib_buffer(
    const std::string& wkt,
    const std::string& expectedWkt,
    const double radius,
    const unsigned int numPoints)
{
    XYMultiPolygon geom;

    std::cout << "test_geolib_buffer: radius=" << radius << ", numPoints=" << numPoints << std::endl;

    if (!readWkt(wkt, geom)) {
        std::cout << "Error parsing wkt string: " << wkt << std::endl;
        return -1;
    }

    geolib::correct(geom);
    geolib::detail::sort(geom);

    std::cout << geolib::wkt(geom, PRECISION) << std::endl;

    auto result = geolib::detail::buffer<XYMultiPolygon>(geom, radius, numPoints, false);
    geolib::detail::sort(result);
    std::cout << geolib::wkt(result, PRECISION) << std::endl;

    if (!expectedWkt.empty()) {
        XYMultiPolygon expectedGeom;
        if (!readWkt(expectedWkt, expectedGeom)) {
            std::cout << "Error parsing wkt string: " << expectedWkt << std::endl;
            return -1;
        }
        geolib::correct(expectedGeom);
        geolib::detail::sort(expectedGeom);

        if (!geolib::equal(result, expectedGeom, EPSILON)) {
            std::cout << "Resulting geom : " << geolib::wkt(result, PRECISION) << std::endl;
            std::cout << "Expected geom  : " << geolib::wkt(expectedGeom, PRECISION) << std::endl;
            return 1;
        }
    }

    return 0;
}

} // namespace

int test_geolib_buffer_1()
{
    return test_geolib_buffer(
        "MULTIPOLYGON(((0 0, 4 0, 4 4, 0 4, 0 0)))",
        "MULTIPOLYGON(((-0.5 0, 0 -0.5, 4 -0.5, 4.5 0, 4.5 4, 4 4.5, 0 4.5, -0.5 4, -0.5 0)))",
        0.5, 2);
}

int test_geolib_buffer_2()
{
    return test_geolib_buffer(
        "MULTIPOLYGON(((0 0, 4 0, 4 4, 0 4, 0 0)))",
        "MULTIPOLYGON(((-0.5 0, -0.353553390593 -0.353553390593, -0 -0.5, 4 -0.5, 4.35355339059 -0.353553390593, 4.5 0, 4.5 4, 4.35355339059 4.35355339059, 4 4.5, 0 4.5, -0.353553390593 4.35355339059, -0.5 4, -0.5 0)))",
        0.5, 4);
}

int test_geolib_buffer_3()
{
    return test_geolib_buffer(
        "MULTIPOLYGON(((0 0, 4 0, 4 4, 0 4, 0 0)))",
        "MULTIPOLYGON(((-0.5 -0, -0.492403876506 -0.086824088833, -0.469846310393 -0.171010071663, -0.433012701892 -0.25, -0.383022221559 -0.321393804843, -0.321393804843 -0.383022221559, -0.25 -0.433012701892, -0.171010071663 -0.469846310393, -0.086824088833 -0.492403876506, 0 -0.5, 4 -0.5, 4.08682408883 -0.492403876506, 4.17101007166 -0.469846310393, 4.25 -0.433012701892, 4.32139380484 -0.383022221559, 4.38302222156 -0.321393804843, 4.43301270189 -0.25, 4.46984631039 -0.171010071663, 4.49240387651 -0.086824088833, 4.5 0, 4.5 4, 4.49240387651 4.08682408883, 4.46984631039 4.17101007166, 4.43301270189 4.25, 4.38302222156 4.32139380484, 4.32139380484 4.38302222156, 4.25 4.43301270189, 4.17101007166 4.46984631039, 4.08682408883 4.49240387651, 4 4.5, 0 4.5, -0.086824088833 4.49240387651, -0.171010071663 4.46984631039, -0.25 4.43301270189, -0.321393804843 4.38302222156, -0.383022221559 4.32139380484, -0.433012701892 4.25, -0.469846310393 4.17101007166, -0.492403876506 4.08682408883, -0.5 4, -0.5 -0)))",
        0.5, 18);
}

int test_geolib_buffer_4()
{
    return test_geolib_buffer(
        "MULTIPOLYGON(((1 5, 0 4, 2 0, 4 0, 5 4, 4 3, 4 5, 3 4, 1 5)))",
        "MULTIPOLYGON(((-0.5 4, 1.5 0, 2 -0.5, 4 -0.5, 4.5 0, 5.5 4, 5 4.5, 4.5 4, 4.5 5, 4 5.5, 3.5 5, 3 4.5, 1 5.5, 0.5 5, 0 4.5, -0.5 4)))",
        0.5, 2);
}

int test_geolib_buffer_5()
{
    return test_geolib_buffer(
        "MULTIPOLYGON(((1 5, 0 4, 2 0, 4 0, 5 4, 4 3, 4 5, 3 4, 1 5)))",
        "MULTIPOLYGON(((-0.5 4, -0.353553390593 3.64644660941, 1.64644660941 -0.353553390593, 2 -0.5, 4 -0.5, 4.35355339059 -0.353553390593, 4.5 0, 5.5 4, 5.35355339059 4.35355339059, 5 4.5, 4.64644660941 4.35355339059, 4.5 4.20710678119, 4.5 5, 4.35355339059 5.35355339059, 4 5.5, 3.64644660941 5.35355339059, 2.8821488698 4.58925565099, 1.35355339059 5.35355339059, 1 5.5, 0.646446609407 5.35355339059, -0.353553390593 4.35355339059, -0.5 4)))",
        0.5, 4);
}

int test_geolib_buffer_6()
{
    return test_geolib_buffer(
        "MULTIPOLYGON(((1 5, 0 4, 2 0, 4 0, 5 4, 4 3, 4 5, 3 4, 1 5)))",
        "MULTIPOLYGON(((-0.5 4, -0.492403876506 3.91317591117, -0.469846310393 3.82898992834, -0.433012701892 3.75, 1.56698729811 -0.25, 1.61697777844 -0.321393804843, 1.67860619516 -0.383022221559, 1.75 -0.433012701892, 1.82898992834 -0.469846310393, 1.91317591117 -0.492403876506, 2 -0.5, 4 -0.5, 4.08682408883 -0.492403876506, 4.17101007166 -0.469846310393, 4.25 -0.433012701892, 4.32139380484 -0.383022221559, 4.38302222156 -0.321393804843, 4.43301270189 -0.25, 4.46984631039 -0.171010071663, 4.49240387651 -0.086824088833, 5.49240387651 3.91317591117, 5.5 4, 5.49240387651 4.08682408883, 5.46984631039 4.17101007166, 5.43301270189 4.25, 5.38302222156 4.32139380484, 5.32139380484 4.38302222156, 5.25 4.43301270189, 5.17101007166 4.46984631039, 5.08682408883 4.49240387651, 5 4.5, 4.91317591117 4.49240387651, 4.82898992834 4.46984631039, 4.75 4.43301270189, 4.67860619516 4.38302222156, 4.61697777844 4.32139380484, 4.5 4.2044160264, 4.5 5, 4.49240387651 5.08682408883, 4.46984631039 5.17101007166, 4.43301270189 5.25, 4.38302222156 5.32139380484, 4.32139380484 5.38302222156, 4.25 5.43301270189, 4.17101007166 5.46984631039, 4.08682408883 5.49240387651, 4 5.5, 3.91317591117 5.49240387651, 3.82898992834 5.46984631039, 3.75 5.43301270189, 3.67860619516 5.38302222156, 3.61697777844 5.32139380484, 2.90239778366 4.60681381006, 1.25 5.43301270189, 1.17101007166 5.46984631039, 1.08682408883 5.49240387651, 1 5.5, 0.913175911167 5.49240387651, 0.828989928337 5.46984631039, 0.75 5.43301270189, 0.678606195157 5.38302222156, 0.616977778441 5.32139380484, -0.321393804843 4.38302222156, -0.383022221559 4.32139380484, -0.433012701892 4.25, -0.469846310393 4.17101007166, -0.492403876506 4.08682408883, -0.5 4)))",
        0.5, 18);
}

int test_geolib_buffer_metropolis_1()
{
    return test_geolib_buffer(WKT_METROPOLIS_1, "", 1.0, 18);
}

int test_geolib_buffer_metropolis_2()
{
    return test_geolib_buffer(WKT_METROPOLIS_2, "", 1.0, 18);
}

int test_geolib_buffer_metropolis_3()
{
    return test_geolib_buffer(WKT_METROPOLIS_3, "", 1.0, 18);
}

int test_geolib_buffer_afimoll_1()
{
    return test_geolib_buffer(WKT_AFIMOLL_1, "", 1.0, 18);
}

int test_geolib_buffer_afimoll_2()
{
    return test_geolib_buffer(WKT_AFIMOLL_2, "", 1.0, 18);
}

int test_geolib_buffer_afimoll_3()
{
    return test_geolib_buffer(WKT_AFIMOLL_3, "", 1.0, 18);
}

int test_geolib_buffer_afimoll_4()
{
    return test_geolib_buffer(WKT_AFIMOLL_4, "", 1.0, 18);
}

int test_geolib_buffer_afimoll_5()
{
    return test_geolib_buffer(WKT_AFIMOLL_5, "", 1.0, 18);
}

int test_geolib_buffer_evropeiskyi_U1()
{
    return test_geolib_buffer(WKT_EVROPEISKYI_U1, "", 1.0, 18);
}

int test_geolib_buffer_evropeiskyi_1()
{
    return test_geolib_buffer(WKT_EVROPEISKYI_1, "", 1.0, 18);
}

int test_geolib_buffer_evropeiskyi_2()
{
    return test_geolib_buffer(WKT_EVROPEISKYI_2, "", 1.0, 18);
}

int test_geolib_buffer_evropeiskyi_3()
{
    return test_geolib_buffer(WKT_EVROPEISKYI_3, "", 1.0, 18);
}

int test_geolib_buffer_evropeiskyi_4()
{
    return test_geolib_buffer(WKT_EVROPEISKYI_4, "", 1.0, 18);
}

int test_geolib_buffer_dubai_mall_1()
{
    return test_geolib_buffer(WKT_DUBAI_MALL_1, "", 1.0, 18);
}

int test_geolib_buffer_dubai_mall_2()
{
    return test_geolib_buffer(WKT_DUBAI_MALL_2, "", 1.0, 18);
}

int test_geolib_buffer_dubai_mall_3()
{
    return test_geolib_buffer(WKT_DUBAI_MALL_3, "", 1.0, 18);
}

int test_geolib_buffer_dubai_mall_4()
{
    return test_geolib_buffer(WKT_DUBAI_MALL_4, "", 1.0, 18);
}

int test_geolib_buffer_dubai_mall_G()
{
    return test_geolib_buffer(WKT_DUBAI_MALL_G, "", 1.0, 18);
}

int test_geolib_buffer_dubai_mall_L()
{
    return test_geolib_buffer(WKT_DUBAI_MALL_L, "", 1.0, 18);
}

int test_geolib_buffer_dubai_mall_LG()
{
    return test_geolib_buffer(WKT_DUBAI_MALL_LG, "", 1.0, 18);
}
