#include <navigine/geometry/geometry.h>

using namespace navigine::geometry;

namespace {

const int PRECISION = 20;

int test_geolib_segment_intersection(
    const XYPoint& p1,
    const XYPoint& p2,
    const XYPoint& q1,
    const XYPoint& q2,
    std::optional<int> expected_count,
    std::optional<double> expected_u1 = std::nullopt,
    std::optional<double> expected_v1 = std::nullopt,
    std::optional<XYPoint> expected_i1 = std::nullopt,
    std::optional<double> expected_u2 = std::nullopt,
    std::optional<double> expected_v2 = std::nullopt,
    std::optional<XYPoint> expected_i2 = std::nullopt)
{
    int count = 0;
    double u1 = NAN;
    double v1 = NAN;
    double u2 = NAN;
    double v2 = NAN;
    XYPoint i1, i2;

    std::cout << std::setprecision(20);
    std::cout << "segmentIntersection:\n";
    std::cout << "\tp1: " << geolib::wkt(p1, PRECISION) << std::endl;
    std::cout << "\tp2: " << geolib::wkt(p2, PRECISION) << std::endl;
    std::cout << "\tq1: " << geolib::wkt(q1, PRECISION) << std::endl;
    std::cout << "\tq2: " << geolib::wkt(q2, PRECISION) << std::endl;

    count = geolib::detail::segmentIntersection(
        p1, p2, q1, q2,
        u1, v1, i1,
        u2, v2, i2);

    std::cout << "Result:\n";
    std::cout << "\tcount: " << count << std::endl;
    std::cout << "\tu1: " << u1 << std::endl;
    std::cout << "\tv1: " << v1 << "\n ";
    std::cout << "\ti1: " << geolib::wkt(i1, PRECISION) << std::endl;
    std::cout << "\tu2: " << u2 << std::endl;
    std::cout << "\tv2: " << v2 << std::endl;
    std::cout << "\ti2: " << geolib::wkt(i2, PRECISION) << std::endl;

    bool ok = true;

    if (expected_count && count != *expected_count) {
        std::cout << "*** Test failed: count: " << count << ", expected: " << *expected_count << std::endl;
        ok = false;
    }

    if (expected_u1 && u1 != *expected_u1) {
        std::cout << "*** Test failed: u1: " << u1 << ", expected: " << *expected_u1 << std::endl;
        ok = false;
    }

    if (expected_v1 && v1 != *expected_v1) {
        std::cout << "*** Test failed: v1: " << v1 << ", expected: " << *expected_v1 << std::endl;
        ok = false;
    }

    if (expected_i1 && !geolib::equal(i1, *expected_i1)) {
        std::cout << "*** Test failed: i1: " << geolib::wkt<XYPoint>(i1, PRECISION) << ", "
                  << "expected: " << geolib::wkt(*expected_i1, PRECISION) << std::endl;
        ok = false;
    }

    if (expected_u2 && u2 != *expected_u2) {
        std::cout << "*** Test failed: u2: " << u2 << ", expected: " << *expected_u2 << std::endl;
        ok = false;
    }

    if (expected_v2 && v2 != *expected_v2) {
        std::cout << "*** Test failed: v2: " << v2 << ", expected: " << *expected_v2 << std::endl;
        ok = false;
    }

    if (expected_i2 && !geolib::equal(i2, *expected_i2)) {
        std::cout << "*** Test failed: i2: " << geolib::wkt<XYPoint>(i2, PRECISION) << ", "
                  << "expected: " << geolib::wkt(*expected_i2, PRECISION) << std::endl;
        ok = false;
    }

    return ok ? 0 : -1;
}

} // namespace

int test_geolib_segment_intersection_1()
{
    // Equal points: p1 = q1
    const auto p1 = XYPoint{50000000.123456789123456789, -1000.987654321987654321};
    const auto p2 = XYPoint{0.12121212121212121212, 0.21212121212121212121};
    const auto q1 = p1;
    const auto q2 = XYPoint{400.987654321987654321, 4.123456789123456789};

    return test_geolib_segment_intersection(p1, p2, q1, q2,
        1,
        0.0, 0.0, p1,
        std::nullopt, std::nullopt, std::nullopt);
}

int test_geolib_segment_intersection_2()
{
    // Equal points: p1 = q2
    const auto p1 = XYPoint{50000000.123456789123456789, -1000.987654321987654321};
    const auto p2 = XYPoint{0.12121212121212121212, 0.21212121212121212121};
    const auto q1 = XYPoint{400.987654321987654321, 4.123456789123456789};
    const auto q2 = p1;

    return test_geolib_segment_intersection(p1, p2, q1, q2,
        1,
        0.0, 1.0, p1);
}

int test_geolib_segment_intersection_3()
{
    // Equal points: p2 = q1
    const auto p1 = XYPoint{0.12121212121212121212, 0.21212121212121212121};
    const auto p2 = XYPoint{50000000.123456789123456789, -1000.987654321987654321};
    const auto q1 = p2;
    const auto q2 = XYPoint{400.987654321987654321, 4.123456789123456789};

    return test_geolib_segment_intersection(p1, p2, q1, q2,
        1,
        1.0, 0.0, p2);
}

int test_geolib_segment_intersection_4()
{
    // Equal points: p2 = q2
    const auto p1 = XYPoint{0.12121212121212121212, 0.21212121212121212121};
    const auto p2 = XYPoint{50000000.123456789123456789, -1000.987654321987654321};
    const auto q1 = XYPoint{400.987654321987654321, 4.123456789123456789};
    const auto q2 = p2;

    return test_geolib_segment_intersection(p1, p2, q1, q2,
        1,
        1.0, 1.0, p2);
}

int test_geolib_segment_intersection_5()
{
    // Basic
    const auto p1 = XYPoint{0, 0};
    const auto p2 = XYPoint{1, 1};
    const auto q1 = XYPoint{0, 1};
    const auto q2 = XYPoint{1, 0};

    return test_geolib_segment_intersection(p1, p2, q1, q2,
        1,
        0.5, 0.5, XYPoint{0.5, 0.5});
}

int test_geolib_segment_intersection_6()
{
    // Coincident lines: [q1,q2] \subset [p1,p2]
    const auto p1 = XYPoint{0, 0};
    const auto p2 = XYPoint{5, 0};
    const auto q1 = XYPoint{4, 0};
    const auto q2 = XYPoint{2, 0};

    return test_geolib_segment_intersection(p1, p2, q1, q2,
        2,
        0.8, 0.0, q1,
        0.4, 1.0, q2);
}

int test_geolib_segment_intersection_7()
{
    // Coincident lines: p1----------p2=q1----------q2
    const auto p1 = XYPoint{0, 0};
    const auto p2 = XYPoint{2, 0};
    const auto q1 = XYPoint{2, 0};
    const auto q2 = XYPoint{4, 0};

    return test_geolib_segment_intersection(p1, p2, q1, q2,
        1,
        1.0, 0.0, p2);
}

int test_geolib_segment_intersection_8()
{
    // Coincident lines: p1=q2----------p2=q1
    const auto p1 = XYPoint{0, 0};
    const auto p2 = XYPoint{2, 0};
    const auto q1 = XYPoint{2, 0};
    const auto q2 = XYPoint{0, 0};

    return test_geolib_segment_intersection(p1, p2, q1, q2,
        2,
        0.0, 1.0, p1,
        1.0, 0.0, p2);
}

int test_geolib_segment_intersection_9()
{
    // Parallel lines
    const auto p1 = XYPoint{-10, -5};
    const auto p2 = XYPoint{10, 5};
    const auto q1 = XYPoint{0.61, 0.3};
    const auto q2 = XYPoint{0.81, 0.4};

    return test_geolib_segment_intersection(p1, p2, q1, q2,
        0);
}

int test_geolib_segment_intersection_10()
{
    // Rounding errors
    const auto p1 = XYPoint{4, 1};
    const auto p2 = XYPoint{1, 1};
    const auto q1 = XYPoint{3, 0};
    const auto q2 = XYPoint{3, 2};

    return test_geolib_segment_intersection(p1, p2, q1, q2,
        1,
        0.33333333333333331483, 0.5, XYPoint{3.0000000000000004441, 1});
}

int test_geolib_segment_intersection_11()
{
    // q2 is inside [p1,p2] + rounding errors
    const auto p1 = XYPoint{0, 4};
    const auto p2 = XYPoint{0, 0.5};
    const auto q1 = XYPoint{-0.5, 4};
    const auto q2 = XYPoint{0, 3.5};

    return test_geolib_segment_intersection(p1, p2, q1, q2,
        1,
        0.14285714285714284921, 1.0, q2);
}

int test_geolib_segment_intersection_12()
{
    // q1 is inside [p1,p2] + rounding errors
    const auto p1 = XYPoint{0, 4};
    const auto p2 = XYPoint{0, 0.5};
    const auto q1 = XYPoint{0, 3.5};
    const auto q2 = XYPoint{4, 3.5};

    return test_geolib_segment_intersection(p1, p2, q1, q2,
        1,
        0.14285714285714284921, 0.0, q1);
}
