#include <navigine/geometry/geometry.h>

using namespace navigine::geometry;

namespace {

const auto PRECISION = 12;
const auto EPSILON = 1e-10;

int test_geolib_convex_hull(
    const std::vector<XYPoint>& inputPoints,
    const std::vector<XYPoint>& expectedPoints)
{
    std::cout << "test_geolib_convex_hull:" << std::endl;

    for(const auto& p : inputPoints) {
        std::cout << "  " << geolib::wkt(p, PRECISION) << std::endl;
    }

    auto result = geolib::convexHull(inputPoints);

    if (!std::equal(
        result.begin(), result.end(),
        expectedPoints.begin(), expectedPoints.end(),
        [&](auto p1, auto p2) {
            return geolib::equal(p1, p2, EPSILON);
        }))
    {
        std::cout << "result:   ";
        for(auto p : result) {
            std::cout << " " << geolib::wkt(p, PRECISION);
        }
        std::cout << std::endl;

        std::cout << "expected: ";
        for(auto p : expectedPoints) {
            std::cout << " " << geolib::wkt(p, PRECISION);
        }
        std::cout << std::endl;
        return 1;
    }

    return 0;
}

} // namespace

int test_geolib_convex_hull_1()
{
    return test_geolib_convex_hull({
            XYPoint{1, 0}, XYPoint{2, 0}, XYPoint{3, 0},
            XYPoint{0, 1}, XYPoint{1, 1}, XYPoint{2, 1}, XYPoint{3, 1},
            XYPoint{0, 2}, XYPoint{1, 2}, XYPoint{2, 2}, XYPoint{3, 2},
            XYPoint{0, 3}, XYPoint{1, 3}, XYPoint{2, 3}, XYPoint{3, 3},
            XYPoint{1, 0}, XYPoint{2, 0}, XYPoint{3, 0},                    // duplicating points
            XYPoint{0, 3}, XYPoint{1, 3}, XYPoint{2, 3}, XYPoint{3, 3},     // duplicating points
        }, {
            XYPoint{1, 0}, XYPoint{3, 0}, XYPoint{3, 3}, XYPoint{0, 3}, XYPoint{0, 1},
        });
}
