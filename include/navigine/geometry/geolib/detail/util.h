#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <functional>
#include <iostream>
#include <iomanip>
#include <limits>
#include <optional>
#include <map>
#include <memory>
#include <type_traits>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>

//#define GEOLIB_DEBUG_ENABLED

namespace navigine::geometry::geolib::util {

template <typename Point>
constexpr double epsilon() {
    return 1e-12;
}

template <typename Point>
double getX(const Point& point) = delete;

template <typename Point>
double getY(const Point& point) = delete;

template <typename Point>
double& getX(Point& point) = delete;

template <typename Point>
double& getY(Point& point) = delete;

} // namespace navigine::geometry::geolib::util
