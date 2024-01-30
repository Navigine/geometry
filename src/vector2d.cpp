#include <cmath>
#include <limits>

#include <navigine/geometry/geometry.h>
#include <navigine/geometry/vector2d.h>
#include <navigine/geometry/utils.h>

namespace math = std;

namespace navigine::geometry {

Vector2d::Vector2d()
    : x(0.0)
    , y(0.0)
{}

Vector2d::Vector2d(double x, double y)
    : x(x)
    , y(y)
{}

Vector2d::Vector2d(const XYPoint& p)
    : x(p.x)
    , y(p.y)
{}

double Vector2d::magnitude() const
{
    return math::sqrt(x * x + y * y);
}

Vector2d Vector2d::normalized() const
{
    const auto length = magnitude();
    return length > std::numeric_limits<double>::epsilon() ?
        Vector2d(x / length, y / length) :
        *this;
}

Vector2d& Vector2d::normalize()
{
    double length = magnitude();
    if (std::fabs(length) > std::numeric_limits<double>::epsilon()) {
        *this /= length;
    }
    return *this;
}

bool Vector2d::isNull() const
{
    return x == 0 && y == 0;
}

bool Vector2d::operator==(const Vector2d& v) const
{
    return checkCloseValues(x, v.x) &&
           checkCloseValues(y, v.y);
}

bool Vector2d::operator!=(const Vector2d& v) const
{
    return !(*this == v);
}

Vector2d& Vector2d::operator+=(const Vector2d& v)
{
    x += v.x;
    y += v.y;
    return *this;
}

Vector2d& Vector2d::operator-=(const Vector2d& v)
{
    x -= v.x;
    y -= v.y;
    return *this;
}

Vector2d& Vector2d::operator*=(double multiplier)
{
    x *= multiplier;
    y *= multiplier;
    return *this;
}

Vector2d& Vector2d::operator/=(double divisor)
{
    x /= divisor;
    y /= divisor;
    return *this;
}

double Vector2d::dotProduct(const Vector2d& v1, const Vector2d& v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

std::ostream& operator<<(std::ostream& os, const Vector2d& v)
{
    os << "[" << v.x << ", " << v.y << "]";
    return os;
}

} // namespace navigine::geometry
