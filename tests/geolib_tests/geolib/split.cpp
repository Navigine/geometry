#include <navigine/geometry/geometry.h>

#include "wkt_examples.h"

using namespace navigine::geometry;

namespace {

const auto PRECISION = 14;
const auto EPSILON = 1e-10;

int test_geolib_split(const std::string& wkt, const std::string& split)
{
    XYMultiPolygon geom;

    std::cout << "test_geolib_split: split=" << split << std::endl;

    if (!readWkt(wkt, geom)) {
        std::cout << "Error parsing wkt string: " << wkt << std::endl;
        return -1;
    }

    geolib::correct(geom);

    const auto area = geolib::area(geom);

    std::cout << geolib::wkt(geom, PRECISION) << std::endl;

    auto box = geolib::boundingBox(geom);
    box.minCorner.x -= 1.0;
    box.minCorner.y -= 1.0;
    box.maxCorner.x += 1.0;
    box.maxCorner.y += 1.0;

    auto boxMin = box;
    auto boxMax = box;

    if (split == "X") {
        const auto xmid = (box.minCorner.x + box.maxCorner.x) / 2;
        boxMin.maxCorner.x = xmid;
        boxMax.minCorner.x = xmid;
    } else if (split == "Y") {
        const auto ymid = (box.minCorner.y + box.maxCorner.y) / 2;
        boxMin.maxCorner.y = ymid;
        boxMax.minCorner.y = ymid;
    } else {
        return -1;
    }

    const auto geomMin = geolib::intersection(geom, boxMin);
    const auto geomMax = geolib::intersection(geom, boxMax);

    const auto areaMin = geolib::area(geomMin);
    const auto areaMax = geolib::area(geomMax);
    const auto areaSum = areaMin + areaMax;

    const auto relativeError = std::fabs(areaSum / area - 1.0);

    if (relativeError > EPSILON) {
        std::cout << "boxMin: " << geolib::wkt(boxMin, PRECISION) << std::endl;
        std::cout << "boxMax: " << geolib::wkt(boxMax, PRECISION) << std::endl;
        std::cout << "areaMin: " << areaMin << std::endl;
        std::cout << "areaMax: " << areaMax << std::endl;
        std::cout << "original area: " << area << std::endl;
        std::cout << "area of parts: " << areaSum << std::endl;
        std::cout << "relative error: " << relativeError << std::endl;
        return 1;
    }

    return 0;
}

} // namespace

int test_geolib_split_metropolis_1()
{
    return test_geolib_split(WKT_METROPOLIS_1, "X") ||
           test_geolib_split(WKT_METROPOLIS_1, "Y");
}

int test_geolib_split_metropolis_2()
{
    return test_geolib_split(WKT_METROPOLIS_2, "X") ||
           test_geolib_split(WKT_METROPOLIS_2, "Y");
}

int test_geolib_split_metropolis_3()
{
    return test_geolib_split(WKT_METROPOLIS_3, "X") ||
           test_geolib_split(WKT_METROPOLIS_3, "Y");
}

int test_geolib_split_afimoll_1()
{
    return test_geolib_split(WKT_AFIMOLL_1, "X") ||
           test_geolib_split(WKT_AFIMOLL_1, "Y");
}

int test_geolib_split_afimoll_2()
{
    return test_geolib_split(WKT_AFIMOLL_2, "X") ||
           test_geolib_split(WKT_AFIMOLL_2, "Y");
}

int test_geolib_split_afimoll_3()
{
    return test_geolib_split(WKT_AFIMOLL_3, "X") ||
           test_geolib_split(WKT_AFIMOLL_3, "Y");
}

int test_geolib_split_afimoll_4()
{
    return test_geolib_split(WKT_AFIMOLL_4, "X") ||
           test_geolib_split(WKT_AFIMOLL_4, "Y");
}

int test_geolib_split_afimoll_5()
{
    return test_geolib_split(WKT_AFIMOLL_5, "X") ||
           test_geolib_split(WKT_AFIMOLL_5, "Y");
}

int test_geolib_split_evropeiskyi_U1()
{
    return test_geolib_split(WKT_EVROPEISKYI_U1, "X") ||
           test_geolib_split(WKT_EVROPEISKYI_U1, "Y");
}

int test_geolib_split_evropeiskyi_1()
{
    return test_geolib_split(WKT_EVROPEISKYI_1, "X") ||
           test_geolib_split(WKT_EVROPEISKYI_1, "Y");
}

int test_geolib_split_evropeiskyi_2()
{
    return test_geolib_split(WKT_EVROPEISKYI_2, "X") ||
           test_geolib_split(WKT_EVROPEISKYI_2, "Y");
}

int test_geolib_split_evropeiskyi_3()
{
    return test_geolib_split(WKT_EVROPEISKYI_3, "X") ||
           test_geolib_split(WKT_EVROPEISKYI_3, "Y");
}

int test_geolib_split_evropeiskyi_4()
{
    return test_geolib_split(WKT_EVROPEISKYI_4, "X") ||
           test_geolib_split(WKT_EVROPEISKYI_4, "Y");
}

int test_geolib_split_dubai_mall_1()
{
    return test_geolib_split(WKT_DUBAI_MALL_1, "X") ||
           test_geolib_split(WKT_DUBAI_MALL_1, "Y");
}

int test_geolib_split_dubai_mall_2()
{
    return test_geolib_split(WKT_DUBAI_MALL_2, "X") ||
           test_geolib_split(WKT_DUBAI_MALL_2, "Y");
}

int test_geolib_split_dubai_mall_3()
{
    return test_geolib_split(WKT_DUBAI_MALL_3, "X") ||
           test_geolib_split(WKT_DUBAI_MALL_3, "Y");
}

int test_geolib_split_dubai_mall_4()
{
    return test_geolib_split(WKT_DUBAI_MALL_4, "X") ||
           test_geolib_split(WKT_DUBAI_MALL_4, "Y");
}

int test_geolib_split_dubai_mall_G()
{
    return test_geolib_split(WKT_DUBAI_MALL_G, "X") ||
           test_geolib_split(WKT_DUBAI_MALL_G, "Y");
}

int test_geolib_split_dubai_mall_L()
{
    return test_geolib_split(WKT_DUBAI_MALL_L, "X") ||
           test_geolib_split(WKT_DUBAI_MALL_L, "Y");
}

int test_geolib_split_dubai_mall_LG()
{
    return test_geolib_split(WKT_DUBAI_MALL_LG, "X") ||
           test_geolib_split(WKT_DUBAI_MALL_LG, "Y");
}
