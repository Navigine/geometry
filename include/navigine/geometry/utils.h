#pragma once

#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <optional>
#include <ostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <type_traits>
#include <vector>

namespace navigine::geometry {

const double EPSILON = 1e-10;

using Timestamp = int64_t;

bool checkCloseValues(double v1, double v2);

bool checkCloseValues(
    const std::optional<double>& v1,
    const std::optional<double>& v2);

#define NC_ASSERT_IMPL(cond)                                                        \
    do {                                                                            \
        if (!(cond)) {                                                              \
            std::stringstream ss;                                                   \
            ss << "**** **** **** **** **** **** **** **** **** **** **** ****\n";  \
            ss << "Assertion failed at " << __FILE__ << ":" << __LINE__ << ":\n";   \
            ss << ">>> " << #cond << "\n\n";                                        \
            std::cerr << ss.str();                                                  \
            ::std::abort();                                                         \
        }                                                                           \
    } while (false)

#define NC_ASSERT_MSG_IMPL(cond, message)                                           \
    do {                                                                            \
        if (!(cond)) {                                                              \
            std::stringstream ss;                                                   \
            ss << "**** **** **** **** **** **** **** **** **** **** **** ****\n";  \
            ss << "Fatal error at " << __FILE__ << ":" << __LINE__ << ":\n";        \
            ss << ">>> " << message << "\n\n";                                      \
            std::cerr << ss.str();                                                  \
            ::std::abort();                                                         \
        }                                                                           \
    } while (false)

#define NC_ASSERT_SELECT_3RD(x, y, z, ...) z
#define NC_ASSERT_IMPL_HELPER(...) \
    NC_ASSERT_SELECT_3RD(__VA_ARGS__, NC_ASSERT_MSG_IMPL, NC_ASSERT_IMPL)

#define NC_ASSERT(...) NC_ASSERT_IMPL_HELPER(__VA_ARGS__)(__VA_ARGS__)

} // namespace navigine::geometry
