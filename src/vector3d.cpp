#include <cmath>
#include <limits>

#include <navigine/geometry/vector3d.h>
#include <navigine/geometry/utils.h>

namespace math = std;

namespace navigine::geometry {

Vector3d::Vector3d()
    : x(0.0)
    , y(0.0)
    , z(0.0)
{}

Vector3d::Vector3d(double x, double y, double z)
    : x(x)
    , y(y)
    , z(z)
{}

double Vector3d::magnitude() const
{
    return math::sqrt(x * x + y * y + z * z);
}

double Vector3d::squaredNorm() const
{
    return x * x + y * y + z * z;
}

Vector3d Vector3d::normalized() const
{
    const auto length = magnitude();
    return length > std::numeric_limits<double>::epsilon() ?
        Vector3d(x / length, y / length, z / length) :
        *this;
}

Vector3d& Vector3d::normalize()
{
    double length = magnitude();
    if (std::fabs(length) > std::numeric_limits<double>::epsilon()) {
        *this /= length;
    }
    return *this;
}

bool Vector3d::isNull() const
{
    return x == 0 && y == 0 && z == 0;
}

bool Vector3d::operator==(const Vector3d& v) const
{
    return checkCloseValues(x, v.x) &&
           checkCloseValues(y, v.y) &&
           checkCloseValues(z, v.z);
}

bool Vector3d::operator!=(const Vector3d& v) const
{
    return !(*this == v);
}

Vector3d& Vector3d::operator+=(const Vector3d& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

Vector3d& Vector3d::operator-=(const Vector3d& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

Vector3d& Vector3d::operator*=(double multiplier)
{
    x *= multiplier;
    y *= multiplier;
    z *= multiplier;
    return *this;
}

Vector3d& Vector3d::operator/=(double divisor)
{
    x /= divisor;
    y /= divisor;
    z /= divisor;
    return *this;
}

double Vector3d::dotProduct(const Vector3d& v1, const Vector3d& v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector3d Vector3d::crossProduct(const Vector3d& v1, const Vector3d& v2)
{
    return Vector3d(
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x);
}

std::ostream& operator<<(std::ostream& os, const Vector3d& v)
{
    os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
    return os;
}

} // namespace navigine::geometry
