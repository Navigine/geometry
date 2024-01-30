#include <algorithm>
#include <cctype>
#include <cmath>

#include <navigine/geometry/utils.h>

namespace navigine::geometry {

bool checkCloseValues(double v1, double v2)
{
    return std::fabs(v1 - v2) < EPSILON;
}

bool checkCloseValues(
    const std::optional<double>& v1,
    const std::optional<double>& v2)
{
    if (!v1 && !v2) {
        return true;
    }

    if (!v1 || !v2) {
        return false;
    }

    return checkCloseValues(*v1, *v2);
}

} // namespace navigine::geometry
