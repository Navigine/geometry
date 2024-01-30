#include <navigine/geometry/geometry.h>
#include <navigine/geometry/geolib/detail/correct.h>

using namespace navigine::geometry;

namespace {

const auto PRECISION = 14;
const auto EPSILON = 1e-10;

int test_binary_operation(
    geolib::detail::BinaryOperationType op,
    const std::string& wkt1,
    const std::string& wkt2,
    const std::string& wktExpected)
{
    XYMultiPolygon geom1, geom2, geomExpected;

    std::cout << "test_binary_operation " << static_cast<int>(op) << std::endl;

    if (!readWkt(wkt1, geom1)) {
        std::cout << "Error parsing wkt string: " << wkt1 << std::endl;
        return -1;
    }

    if (!readWkt(wkt2, geom2)) {
        std::cout << "Error parsing wkt string: " << wkt2 << std::endl;
        return -1;
    }

    if (!readWkt(wktExpected, geomExpected)) {
        std::cout << "Error parsing wkt string: " << wktExpected << std::endl;
        return -1;
    }

    geolib::correct(geom1);
    geolib::correct(geom2);
    geolib::correct(geomExpected);

    geolib::detail::sort(geom1);
    geolib::detail::sort(geom2);
    geolib::detail::sort(geomExpected);

    std::cout << geolib::wkt(geom1, PRECISION) << std::endl;
    std::cout << geolib::wkt(geom2, PRECISION) << std::endl;

    auto result = geolib::detail::binaryOperation<XYMultiPolygon>(op, geom1, geom2);
    geolib::detail::sort(result);

    if (!geolib::equal(result, geomExpected, EPSILON)) {
        std::cout << "result:   " << geolib::wkt(result, PRECISION) << std::endl;
        std::cout << "expected: " << geolib::wkt(geomExpected, PRECISION) << std::endl;
        return 1;
    }

    return 0;
}

} // namespace

int test_geolib_union_1()
{
    return test_binary_operation(geolib::detail::BinaryOperationType::UNION,
        "MULTIPOLYGON(((0 4, 2 2, 0 0, 5 0, 5 4, 0 4)))",
        "MULTIPOLYGON(((1 5, 0 4, 2 0, 4 0, 5 4, 4 3, 4 5, 3 4, 1 5)))",
        "MULTIPOLYGON(((0 0, 2 0, 4 0, 5 0, 5 4, 4 4, 4 5, 3 4, 1 5, 0 4, 1.333333333333 1.333333333333, 0 0)))");
}

int test_geolib_union_2()
{
    return test_binary_operation(geolib::detail::BinaryOperationType::UNION,
        "MULTIPOLYGON(((0 2, 10 2, 10 6, 0 6, 0 2), (2 4, 3 4, 3 3, 2 3, 2 4)))",
        "MULTIPOLYGON(((1 0, 9 0, 9 5, 6 5, 6 1, 4 1, 4 5, 1 5, 1 0), (8 3, 7 3, 7 4, 8 4, 8 3)))",
        "MULTIPOLYGON(((0 2, 1 2, 1 0, 9 0, 9 2, 10 2, 10 6, 0 6, 0 2), (4 1, 4 2, 6 2, 6 1, 4 1)))");
}

int test_geolib_union_3()
{
    return test_binary_operation(geolib::detail::BinaryOperationType::UNION,
        "MULTIPOLYGON(((0 0, 4 0, 4 1, 1 1, 1 2, 4 2, 4 3, 0 3, 0 0)))",
        "MULTIPOLYGON(((3 0, 3 2, 1 2, 3 0)))",
        "MULTIPOLYGON(((0 0, 3 0, 4 0, 4 1, 3 1, 3 2, 4 2, 4 3, 0 3, 0 0), (1 1, 1 2, 2 1, 1 1)))");
}

int test_geolib_union_4()
{
    return test_binary_operation(geolib::detail::BinaryOperationType::UNION,
        "MULTIPOLYGON(((-1 -1, 1.5 -1, 1.5 4, -1 4, -1 -1)))",
        "MULTIPOLYGON(((1 0, 3 0, 3 3, 0 3, 0 1, 1 1, 1 0), (1 1, 1 2, 2 2, 2 1, 1 1)))",
        "MULTIPOLYGON(((-1 -1, 1.5 -1, 1.5 0, 3 0, 3 3, 1.5 3, 1.5 4, -1 4, -1 -1), (1.5 1, 1.5 2, 2 2, 2 1, 1.5 1)))");
}

int test_geolib_union_5()
{
    return test_binary_operation(geolib::detail::BinaryOperationType::UNION,
        "MULTIPOLYGON(((-2 -2, 2 -2, 0 0, -2 -2)), ((0 0, 2 2, -2 2, 0 0)))",
        "MULTIPOLYGON(((0 0, 0.5 -1.5, 2 -0.5, 0 0)), ((-0.5 1.5, -2 0.5, 0 0, -0.5 1.5)))",
        "MULTIPOLYGON(((-2 -2, 2 -2, 1.1 -1.1, 2 -0.5, 0 0, -2 -2)), ((-2 0.5, 0 0, 2 2, -2 2, -1.1 1.1, -2 0.5)))");
}

int test_geolib_union_6_extra()
{
    return test_binary_operation(geolib::detail::BinaryOperationType::UNION,
        "MULTIPOLYGON(((-0.5 0, 0 -0.5, 4 -0.5, 4.5 0, 4.5 4, 4 4.5, 3.5 4, 0 4, 0 0.5, -0.5 0)))",
        "MULTIPOLYGON(((4 3.5, 4.5 4, 4 4.5, 0 4.5, -0.5 4, 0 3.5, 4 3.5)))",
        "MULTIPOLYGON(((-0.5 0, 0 -0.5, 4 -0.5, 4.5 0, 4.5 4, 4 4.5, 0 4.5, -0.5 4, 0 3.5, 0 0.5, -0.5 0)))");
}

int test_geolib_union_7_extra()
{
    return test_binary_operation(geolib::detail::BinaryOperationType::UNION,
        "MULTIPOLYGON(((-0.1 4.006, -0.099 3.989, -0.096 3.971, -0.089 3.955, 1.9095 -0.041999999999997, 1.913 -0.05, 1.91925 -0.058749999999999, 1.92 -0.06, 1.9209166666667 -0.061083333333334, 1.923 -0.064, 1.9255 -0.0665, 1.931 -0.073, 1.9345 -0.0755, 1.936 -0.077, 1.95 -0.087, 1.9561538461538 -0.089692307692308, 1.96 -0.092, 1.964358974359 -0.093282051282051, 1.966 -0.094, 1.97 -0.094941176470588, 1.977 -0.097, 1.994 -0.1, 1.998 -0.099764705882353, 2 -0.1, 3.993 -0.1, 4 -0.1, 4.007 -0.099176470588235, 4.01 -0.099, 4.0136 -0.0984, 4.017 -0.098, 4.0194285714286 -0.097428571428571, 4.028 -0.096, 4.0322105263158 -0.094421052631579, 4.034 -0.094, 4.038 -0.092249999999999, 4.044 -0.09, 4.0463076923077 -0.088615384615385, 4.05 -0.087, 4.05525 -0.08325, 4.059 -0.081, 4.06225 -0.07825, 4.064 -0.077, 4.0655 -0.0755, 4.072 -0.07, 4.0756666666667 -0.065333333333333, 4.077 -0.064, 4.0798571428571 -0.060000000000004, 4.083 -0.056, 4.0838421052632 -0.054421052631577, 4.087 -0.05, 4.090652173913 -0.041652173913045, 4.091 -0.041, 4.0912608695652 -0.040260869565216, 4.094 -0.034, 4.0950588235294 -0.029500000000001, 4.097 -0.024, 5.097 3.976, 5.1 3.993, 5.099 4.01, 5.096 4.028, 5.09 4.044, 5.081 4.059, 5.07 4.072, 5.056 4.083, 5.041 4.091, 5.024 4.097, 5.007 4.1, 4.99 4.099, 4.972 4.096, 4.956 4.09, 4.941 4.081, 4.928 4.07, 4.917 4.056, 4.909 4.041, 4.903 4.024, 4.8626666666667 3.8626666666667, 4 3, 4 5, 3 4, 1 5, 0.070833333333333 4.0708333333333, 0.069 4.073, 0.055 4.083, 0.04 4.092, 0.023 4.097, 0.006 4.1, -0.011 4.099, -0.029 4.096, -0.045 4.089, -0.06 4.08, -0.073 4.069, -0.083 4.055, -0.092 4.04, -0.097 4.023, -0.1 4.006)))",
        "MULTIPOLYGON(((3.9 2.991, 3.903 2.974, 3.909 2.958, 3.918 2.943, 3.929 2.929, 3.943 2.918, 3.958 2.909, 3.974 2.903, 3.991 2.9, 4.009 2.9, 4.026 2.903, 4.042 2.909, 4.057 2.918, 4.071 2.929, 5.071 3.929, 5.082 3.943, 5.091 3.958, 5.097 3.974, 5.1 3.991, 5.1 4.009, 5.097 4.026, 5.091 4.042, 5.082 4.057, 5.071 4.071, 5.057 4.082, 5.042 4.091, 5.026 4.097, 5.009 4.1, 4.991 4.1, 4.974 4.097, 4.958 4.091, 4.943 4.082, 4.929 4.071, 3.929 3.071, 3.918 3.057, 3.909 3.042, 3.903 3.026, 3.9 3.009, 3.9 2.991)))",
        "MULTIPOLYGON(((-0.1 4.006, -0.099 3.989, -0.096 3.971, -0.089 3.955, 1.9095 -0.042, 1.913 -0.05, 1.91925 -0.05875, 1.92 -0.06, 1.920916666667 -0.061083333333, 1.923 -0.064, 1.9255 -0.0665, 1.931 -0.073, 1.9345 -0.0755, 1.936 -0.077, 1.95 -0.087, 1.956153846154 -0.089692307692, 1.96 -0.092, 1.964358974359 -0.093282051282, 1.966 -0.094, 1.97 -0.094941176471, 1.977 -0.097, 1.994 -0.1, 1.998 -0.099764705882, 2 -0.1, 3.993 -0.1, 4 -0.1, 4.007 -0.099176470588, 4.01 -0.099, 4.0136 -0.0984, 4.017 -0.098, 4.019428571429 -0.097428571429, 4.028 -0.096, 4.032210526316 -0.094421052632, 4.034 -0.094, 4.038 -0.09225, 4.044 -0.09, 4.046307692308 -0.088615384615, 4.05 -0.087, 4.05525 -0.08325, 4.059 -0.081, 4.06225 -0.07825, 4.064 -0.077, 4.0655 -0.0755, 4.072 -0.07, 4.075666666667 -0.065333333333, 4.077 -0.064, 4.079857142857 -0.06, 4.083 -0.056, 4.083842105263 -0.054421052632, 4.087 -0.05, 4.090652173913 -0.041652173913, 4.091 -0.041, 4.091260869565 -0.040260869565, 4.094 -0.034, 4.095058823529 -0.0295, 4.097 -0.024, 5.0955 3.97, 5.097 3.974, 5.1 3.991, 5.1 3.993, 5.1 4.009, 5.097 4.026, 5.091 4.042, 5.090333333333 4.043111111111, 5.09 4.044, 5.081 4.059, 5.073 4.068454545455, 5.071 4.071, 5.070540983607 4.071360655738, 5.07 4.072, 5.056 4.083, 5.05 4.0862, 5.042 4.091, 5.026 4.097, 5.009 4.1, 5.007 4.1, 4.991 4.1, 4.974 4.097, 4.958 4.091, 4.956888888889 4.090333333333, 4.956 4.09, 4.941 4.081, 4.931545454545 4.073, 4.929 4.071, 4.928 4.07, 4 3.142, 4 5, 3 4, 1 5, 0.070833333333 4.070833333333, 0.069 4.073, 0.055 4.083, 0.04 4.092, 0.023 4.097, 0.006 4.1, -0.011 4.099, -0.029 4.096, -0.045 4.089, -0.06 4.08, -0.073 4.069, -0.083 4.055, -0.092 4.04, -0.097 4.023, -0.1 4.006)))");
}

int test_geolib_intersection_1()
{
    return test_binary_operation(geolib::detail::BinaryOperationType::INTERSECTION,
        "MULTIPOLYGON(((0 4, 2 2, 0 0, 5 0, 5 4, 0 4)))",
        "MULTIPOLYGON(((1 5, 0 4, 2 0, 4 0, 5 4, 4 3, 4 5, 3 4, 1 5)))",
        "MULTIPOLYGON(((0 4, 2 2, 1.333333333333 1.333333333333, 2 0, 4 0, 5 4, 4 3, 4 4, 3 4, 0 4)))");
}

int test_geolib_intersection_2()
{
    return test_binary_operation(geolib::detail::BinaryOperationType::INTERSECTION,
        "MULTIPOLYGON(((0 2, 10 2, 10 6, 0 6, 0 2), (2 4, 3 4, 3 3, 2 3, 2 4)))",
        "MULTIPOLYGON(((1 0, 9 0, 9 5, 6 5, 6 1, 4 1, 4 5, 1 5, 1 0), (8 3, 7 3, 7 4, 8 4, 8 3)))",
        "MULTIPOLYGON(((1 2, 4 2, 4 5, 1 5, 1 2), (2 3, 2 4, 3 4, 3 3, 2 3)), ((6 2, 9 2, 9 5, 6 5, 6 2), (7 3, 7 4, 8 4, 8 3, 7 3)))");
}

int test_geolib_intersection_3()
{
    return test_binary_operation(geolib::detail::BinaryOperationType::INTERSECTION,
        "MULTIPOLYGON(((0 0, 4 0, 4 1, 1 1, 1 2, 4 2, 4 3, 0 3, 0 0)))",
        "MULTIPOLYGON(((3 0, 3 2, 1 2, 3 0)))",
        "MULTIPOLYGON(((2 1, 3 0, 3 1, 2 1)))");
}

int test_geolib_intersection_4()
{
    return test_binary_operation(geolib::detail::BinaryOperationType::INTERSECTION,
        "MULTIPOLYGON(((-1 -1, 1.5 -1, 1.5 4, -1 4, -1 -1)))",
        "MULTIPOLYGON(((1 0, 3 0, 3 3, 0 3, 0 1, 1 1, 1 0), (1 1, 1 2, 2 2, 2 1, 1 1)))",
        "MULTIPOLYGON(((0 1, 1 1, 1 2, 1.5 2, 1.5 3, 0 3, 0 1)), ((1 0, 1.5 0, 1.5 1, 1 1, 1 0)))");
}

int test_geolib_intersection_5()
{
    return test_binary_operation(geolib::detail::BinaryOperationType::INTERSECTION,
        "MULTIPOLYGON(((-2 -2, 2 -2, 0 0, -2 -2)), ((0 0, 2 2, -2 2, 0 0)))",
        "MULTIPOLYGON(((0 0, 0.5 -1.5, 2 -0.5, 0 0)), ((-0.5 1.5, -2 0.5, 0 0, -0.5 1.5)))",
        "MULTIPOLYGON(((-1.1 1.1, 0 0, -0.5 1.5, -1.1 1.1)), ((0 0, 0.5 -1.5, 1.1 -1.1, 0 0)))");
}

int test_geolib_difference_1()
{
    return test_binary_operation(geolib::detail::BinaryOperationType::DIFFERENCE,
        "MULTIPOLYGON(((0 4, 2 2, 0 0, 5 0, 5 4, 0 4)))",
        "MULTIPOLYGON(((1 5, 0 4, 2 0, 4 0, 5 4, 4 3, 4 5, 3 4, 1 5)))",
        "MULTIPOLYGON(((0 0, 2 0, 1.333333333333 1.333333333333, 0 0)), ((4 0, 5 0, 5 4, 4 0)), ((4 3, 5 4, 4 4, 4 3)))");
}

int test_geolib_difference_2()
{
    return test_binary_operation(geolib::detail::BinaryOperationType::DIFFERENCE,
        "MULTIPOLYGON(((0 2, 10 2, 10 6, 0 6, 0 2), (2 4, 3 4, 3 3, 2 3, 2 4)))",
        "MULTIPOLYGON(((1 0, 9 0, 9 5, 6 5, 6 1, 4 1, 4 5, 1 5, 1 0), (8 3, 7 3, 7 4, 8 4, 8 3)))",
        "MULTIPOLYGON(((0 2, 1 2, 1 5, 4 5, 4 2, 6 2, 6 5, 9 5, 9 2, 10 2, 10 6, 0 6, 0 2)), ((7 3, 8 3, 8 4, 7 4, 7 3)))");
}

int test_geolib_difference_3()
{
    return test_binary_operation(geolib::detail::BinaryOperationType::DIFFERENCE,
        "MULTIPOLYGON(((0 0, 4 0, 4 1, 1 1, 1 2, 4 2, 4 3, 0 3, 0 0)))",
        "MULTIPOLYGON(((3 0, 3 2, 1 2, 3 0)))",
        "MULTIPOLYGON(((0 0, 3 0, 2 1, 1 1, 1 2, 3 2, 4 2, 4 3, 0 3, 0 0)), ((3 0, 4 0, 4 1, 3 1, 3 0)))");
}

int test_geolib_difference_4()
{
    return test_binary_operation(geolib::detail::BinaryOperationType::DIFFERENCE,
        "MULTIPOLYGON(((-1 -1, 1.5 -1, 1.5 4, -1 4, -1 -1)))",
        "MULTIPOLYGON(((1 0, 3 0, 3 3, 0 3, 0 1, 1 1, 1 0), (1 1, 1 2, 2 2, 2 1, 1 1)))",
        "MULTIPOLYGON(((-1 -1, 1.5 -1, 1.5 0, 1 0, 1 1, 0 1, 0 3, 1.5 3, 1.5 4, -1 4, -1 -1)), ((1 1, 1.5 1, 1.5 2, 1 2, 1 1)))");
}

int test_geolib_difference_5()
{
    return test_binary_operation(geolib::detail::BinaryOperationType::DIFFERENCE,
        "MULTIPOLYGON(((-2 -2, 2 -2, 0 0, -2 -2)), ((0 0, 2 2, -2 2, 0 0)))",
        "MULTIPOLYGON(((0 0, 0.5 -1.5, 2 -0.5, 0 0)), ((-0.5 1.5, -2 0.5, 0 0, -0.5 1.5)))",
        "MULTIPOLYGON(((-2 -2, 2 -2, 1.1 -1.1, 0.5 -1.5, 0 0, -2 -2)), ((-2 2, -1.1 1.1, -0.5 1.5, 0 0, 2 2, -2 2)))");
}
