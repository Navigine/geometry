#pragma once

#include <initializer_list>
#include <ostream>
#include <vector>

#include <navigine/geometry/vector2d.h>
#include <navigine/geometry/vector3d.h>

namespace navigine::geometry {

class VectorXd
{
public:
    VectorXd() = default;
    VectorXd(unsigned int dimension);
    VectorXd(const Vector2d& v);
    VectorXd(const Vector3d& v);

    unsigned int dimension() const;

    void resize(unsigned int dimension);

    double& at(unsigned int i);
    const double& at(unsigned int i) const;

    VectorXd segment(unsigned int size, unsigned int startIndex) const;

    void fill(const std::initializer_list<double>& values);
    void fillValue(double value);
    void fillSegment(const VectorXd& segment, unsigned int startIndex);

    void setZero();

    double norm() const;
    VectorXd normalized() const;
    VectorXd& normalize();

    double dotProduct(const VectorXd& v) const;

    VectorXd& operator+=(const VectorXd& v);
    VectorXd& operator-=(const VectorXd& v);

    VectorXd operator+(const VectorXd& v) const;
    VectorXd operator-(const VectorXd& v) const;

    VectorXd& operator*=(double factor);
    VectorXd& operator/=(double factor);

    VectorXd operator*(double factor) const;
    VectorXd operator/(double factor) const;

    bool operator==(const VectorXd& v) const;

    Vector2d toVector2d() const;
    Vector3d toVector3d() const;

    bool isBounded(double value) const;
    bool isFinite() const;
    bool isEmpty() const;

private:
    std::vector<double> data_;
};

std::ostream& operator<<(std::ostream& os, const VectorXd& v);

} // namespace navigine::geometry
