#pragma once

#include <cmath>
#include <ostream>

namespace navigine::geometry {

class Vector3d
{
public:
    double x;
    double y;
    double z;

    Vector3d();
    Vector3d(double x, double y, double z);

    double    magnitude()  const;
    double squaredNorm() const;

    Vector3d  normalized()  const;
    Vector3d& normalize();
    bool      isNull() const;

    static Vector3d crossProduct(const Vector3d& v1, const Vector3d& v2);
    static double   dotProduct  (const Vector3d& v1, const Vector3d& v2);

    bool operator==(const Vector3d& v2) const;
    bool operator!=(const Vector3d& v2) const;

    Vector3d& operator+=(const Vector3d& v2);
    Vector3d& operator-=(const Vector3d& v2);

    Vector3d& operator*=(double multiplier);
    Vector3d& operator/=(double divisor);

    template<int N>
    double& at() = delete;

    template<int N>
    double at() const = delete;
};

inline Vector3d operator*(double multiplier, const Vector3d& v)
{
    return Vector3d(v.x * multiplier, v.y * multiplier, v.z * multiplier);
}

inline Vector3d operator*(const Vector3d& v, double multiplier)
{
    return Vector3d(v.x * multiplier, v.y * multiplier, v.z * multiplier);
}

inline Vector3d operator+(const Vector3d& v1, const Vector3d& v2)
{
    return Vector3d(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

inline Vector3d operator-(const Vector3d& v1, const Vector3d& v2)
{
    return Vector3d(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

inline Vector3d operator-(const Vector3d& v)
{
    return Vector3d(-v.x, -v.y, -v.z);
}

inline Vector3d operator/(const Vector3d& v1, double divisor)
{
    return Vector3d(v1.x / divisor, v1.y / divisor, v1.z / divisor);
}

template<> inline double& Vector3d::at<0>() { return x; }
template<> inline double& Vector3d::at<1>() { return y; }
template<> inline double& Vector3d::at<2>() { return z; }

template<> inline double Vector3d::at<0>() const { return x; }
template<> inline double Vector3d::at<1>() const { return y; }
template<> inline double Vector3d::at<2>() const { return z; }

std::ostream& operator<<(std::ostream& os, const Vector3d& v);

} // namespace navigine::geometry
