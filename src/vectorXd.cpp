#include <algorithm>
#include <cmath>
#include <numeric>
#include <stdexcept>
#include <string>

#include <navigine/geometry/utils.h>
#include <navigine/geometry/vectorXd.h>

namespace math = std;

namespace navigine::geometry {

VectorXd::VectorXd(unsigned int dimension)
    : data_(dimension, 0.0)
{}

VectorXd::VectorXd(const Vector2d& v)
{
    data_.push_back(v.x);
    data_.push_back(v.y);
}

VectorXd::VectorXd(const Vector3d& v)
{
    data_.push_back(v.x);
    data_.push_back(v.y);
    data_.push_back(v.z);
}

unsigned int VectorXd::dimension() const
{
    return data_.size();
}

void VectorXd::resize(unsigned int dimension)
{
    data_.resize(dimension);
}

double& VectorXd::at(unsigned int i)
{
    return data_.at(i);
}

const double& VectorXd::at(unsigned int i) const
{
    return data_.at(i);
}

VectorXd VectorXd::segment(unsigned int size, unsigned int startIndex) const
{
    NC_ASSERT(startIndex + size <= data_.size(), "Invalid size or startIndex segment");

    VectorXd segment(size);
    const auto start = data_.begin() + startIndex;
    std::copy(start, start + size, segment.data_.begin());

    return segment;
}

void VectorXd::fill(const std::initializer_list<double>& values)
{
    NC_ASSERT(data_.size() == values.size(), "Invalid size initializer list");

    std::copy(values.begin(), values.end(), data_.begin());
}

void VectorXd::fillValue(double value)
{
    data_.assign(data_.size(), value);
}

void VectorXd::fillSegment(const VectorXd& segment, unsigned int startIndex)
{
    NC_ASSERT(segment.data_.size() + startIndex <= data_.size(), "Invalid size segment or startIndex");

    std::copy(segment.data_.begin(), segment.data_.end(), data_.begin() + startIndex);
}

void VectorXd::setZero()
{
    data_.assign(data_.size(), 0.0);
}

double VectorXd::norm() const
{
    double sum = 0.0;
    for (const auto& elem : data_) {
        sum += elem * elem;
    }

    return math::sqrt(sum);
}

VectorXd VectorXd::normalized() const
{
    return *this / norm();
}

VectorXd& VectorXd::normalize()
{
    *this /= norm();

    return *this;
}

double VectorXd::dotProduct(const VectorXd& v) const
{
    const auto size = data_.size();
    NC_ASSERT(size == v.data_.size(), "Vectors must be same dimensions");

    double dotProduct = 0.0;
    for (size_t i = 0; i < size; ++i) {
        dotProduct += data_[i] * v.data_[i];
    }

    return dotProduct;
}

VectorXd& VectorXd::operator+=(const VectorXd& v)
{
    const auto size = data_.size();
    NC_ASSERT(size == v.data_.size(), "Vectors must be same dimensions");

    for (size_t i = 0; i < size; ++i) {
        data_[i] += v.data_[i];
    }

    return *this;
}

VectorXd& VectorXd::operator-=(const VectorXd& v)
{
    const auto size = data_.size();
    NC_ASSERT(size == v.data_.size(), "Vectors must be same dimensions");

    for (size_t i = 0; i < size; ++i) {
        data_[i] -= v.data_[i];
    }

    return *this;
}

VectorXd VectorXd::operator+(const VectorXd& v) const
{
    const auto size = data_.size();
    NC_ASSERT(size == v.data_.size(), "Vectors must be same dimensions");

    VectorXd result(size);
    for (size_t i = 0; i < size; ++i) {
        result.data_[i] = data_[i] + v.data_[i];
    }

    return result;
}

VectorXd VectorXd::operator-(const VectorXd& v) const
{
    const auto size = data_.size();
    NC_ASSERT(size == v.data_.size(), "Vectors must be same dimensions");

    VectorXd result(size);
    for (size_t i = 0; i < size; ++i) {
        result.data_[i] = data_[i] - v.data_[i];
    }

    return result;
}

VectorXd& VectorXd::operator*=(double factor)
{
    for (auto& elem : data_) {
        elem *= factor;
    }

    return *this;
}

VectorXd& VectorXd::operator/=(double factor)
{
    for (auto& elem : data_) {
        elem /= factor;
    }

    return *this;
}

VectorXd VectorXd::operator*(double factor) const
{
    VectorXd result(data_.size());
    for (size_t i = 0; i < data_.size(); ++i) {
        result.data_[i] = data_[i] * factor;
    }

    return result;
}

VectorXd VectorXd::operator/(double factor) const
{
    VectorXd result(data_.size());
    for (size_t i = 0; i < data_.size(); ++i) {
        result.data_[i] = data_[i] / factor;
    }

    return result;
}

bool VectorXd::operator==(const VectorXd& v) const
{
    const auto size = data_.size();
    NC_ASSERT(size == v.data_.size(), "Vectors must be same dimensions");

    for (size_t i = 0; i < size; ++i) {
        if (!checkCloseValues(data_[i], v.data_[i]))
            return false;
    }

    return true;
}

Vector2d VectorXd::toVector2d() const
{
    NC_ASSERT(data_.size() >= 2, "Size vectorXd cannot be less than 2");

    return Vector2d(data_[0], data_[1]);
}

Vector3d VectorXd::toVector3d() const
{
    NC_ASSERT(data_.size() >= 3, "Size vectorXd cannot be less than 3");

    return Vector3d(data_[0], data_[1], data_[2]);
}

bool VectorXd::isBounded(double value) const
{
    for (const auto& elem : data_) {
        if (std::isnan(elem) || std::fabs(elem) > value) {
            return false;
        }
    }

    return true;
}

bool VectorXd::isFinite() const
{
    for (const auto& elem : data_) {
        if (std::isinf(elem) || std::isnan(elem)) {
            return false;
        }
    }

    return true;
}

bool VectorXd::isEmpty() const
{
    return data_.empty();
}

std::ostream& operator<<(std::ostream& os, const VectorXd& v)
{
    const auto dimension = v.dimension();
    os << "[";
    for (size_t i = 0; i < dimension; ++i) {
        if (i > 0) {
            os << ", ";
        }
        os << v.at(i);
    }
    os << "]";

    return os;
}

} // namespace navigine::geometry
