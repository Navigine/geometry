#pragma once

#include <navigine/geometry/geolib/detail/type_checks.h>
#include <navigine/geometry/geolib/detail/util.h>

namespace navigine::geometry::geolib::detail {

template <typename Geometry>
bool isValid(const Geometry& /*geometry*/, std::string& /*message*/)
{
    // TODO: implement validity check
    return true;
}

} // namespace navigine::geometry::geolib::detail
