#pragma once

#include <cmath>
#include <ostream>

namespace navigine::geometry {

struct XYPoint;

class Vector2d
{
public:
    double x;
    double y;

    Vector2d();
    Vector2d(double x, double y);
    Vector2d(const XYPoint& p);

    double    magnitude() const;
    Vector2d  normalized() const;
    Vector2d& normalize();
    bool      isNull() const;

    static double dotProduct(const Vector2d& v1, const Vector2d& v2);

    bool operator==(const Vector2d& v2) const;
    bool operator!=(const Vector2d& v2) const;

    Vector2d& operator+=(const Vector2d& v2);
    Vector2d& operator-=(const Vector2d& v2);

    Vector2d& operator*=(double multiplier);
    Vector2d& operator/=(double divisor);

    template<int N>
    double& at() = delete;

    template<int N>
    double at() const = delete;
};

inline Vector2d operator*(double multiplier, const Vector2d& v)
{
    return Vector2d(v.x * multiplier, v.y * multiplier);
}

inline Vector2d operator*(const Vector2d& v, double multiplier)
{
    return Vector2d(v.x * multiplier, v.y * multiplier);
}

inline Vector2d operator+(const Vector2d& v1, const Vector2d& v2)
{
    return Vector2d(v1.x + v2.x, v1.y + v2.y);
}

inline Vector2d operator-(const Vector2d& v1, const Vector2d& v2)
{
    return Vector2d(v1.x - v2.x, v1.y - v2.y);
}

inline Vector2d operator-(const Vector2d& v)
{
    return Vector2d(-v.x, -v.y);
}

inline Vector2d operator/(const Vector2d& v1, double divisor)
{
    return Vector2d(v1.x / divisor, v1.y / divisor);
}

template<> inline double& Vector2d::at<0>() { return x; }
template<> inline double& Vector2d::at<1>() { return y; }

template<> inline double Vector2d::at<0>() const { return x; }
template<> inline double Vector2d::at<1>() const { return y; }

std::ostream& operator<<(std::ostream& os, const Vector2d& v);

} // namespace navigine::geometry
