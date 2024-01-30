#include <algorithm>
#include <cmath>

#include <navigine/geometry/geometry.h>
#include <navigine/geometry/utils.h>

namespace math = std;

namespace navigine::geometry {

namespace {

constexpr double EQUATOR_CIRCUMFERENCE_METERS = 40075160.0;
constexpr double POLE_CIRCUMFERENCE_METERS = 40008000.0;
constexpr double EARTH_RADIUS = 6378000;

} // namespace

bool operator==(const GeoPoint& p1, const GeoPoint& p2)
{
    return checkCloseValues(p1.latitude, p2.latitude) &&
           checkCloseValues(p1.longitude, p2.longitude);
}

bool operator==(const XYPoint& p1, const XYPoint& p2)
{
    return checkCloseValues(p1.x, p2.x) &&
           checkCloseValues(p1.y, p2.y);
}

XYPoint& XYPoint::operator+=(const XYPoint& p)
{
    x += p.x;
    y += p.y;
    return *this;
}

XYPoint& XYPoint::operator-=(const XYPoint& p)
{
    x -= p.x;
    y -= p.y;
    return *this;
}

XYPoint& XYPoint::operator*=(double multiplier)
{
    x *= multiplier;
    y *= multiplier;
    return *this;
}

XYPoint& XYPoint::operator/=(double divisor)
{
    x /= divisor;
    y /= divisor;
    return *this;
}

XYPoint operator+(const XYPoint& p1, const XYPoint& p2)
{
    return XYPoint{p1.x + p2.x, p1.y + p2.y};
}

XYPoint operator-(const XYPoint& p1, const XYPoint& p2)
{
    return XYPoint{p1.x - p2.x, p1.y - p2.y};
}

XYPoint operator*(const XYPoint& p, double multiplier)
{
    return XYPoint{p.x * multiplier, p.y * multiplier};
}

XYPoint operator*(double multiplier, const XYPoint& p)
{
    return XYPoint{p.x * multiplier, p.y * multiplier};
}

XYPoint operator/(const XYPoint& p, double divisor)
{
    return XYPoint{p.x / divisor, p.y / divisor};
}

XYPoint operator+(const XYPoint& p, const Vector2d& v)
{
    return XYPoint{p.x + v.x, p.y + v.y};
}

XYPoint operator-(const XYPoint& p, const Vector2d& v)
{
    return XYPoint{p.x - v.x, p.y - v.y};
}

double GetBoxWidth(const XYBox& box)
{
    return box.maxCorner.x - box.minCorner.x;
}

double GetBoxHeight(const XYBox& box)
{
    return box.maxCorner.y - box.minCorner.y;
}

// Calculate norm of the point-vector
double GetNorm(const XYPoint& point)
{
    return math::sqrt(point.x * point.x + point.y * point.y);
}

// Get distance between points on the xy-plane
double GetDist(const XYPoint& point1, const XYPoint& point2)
{
    return math::sqrt(GetSqrDist(point1, point2));
}

// Get squared distance between points on the xy-plane
double GetSqrDist(const XYPoint& point1, const XYPoint& point2)
{
    return (point1.x - point2.x) * (point1.x - point2.x) +
           (point1.y - point2.y) * (point1.y - point2.y);
}

// Get geo-distance between points on the earth geoid
double GetGeoDist(const GeoPoint& point1, const GeoPoint& point2)
{
    const double lat1 = DegToRad(point1.latitude);
    const double lon1 = DegToRad(point1.longitude);
    const double lat2 = DegToRad(point2.latitude);
    const double lon2 = DegToRad(point2.longitude);
    const double dlat = lat1 - lat2;
    const double dlon = lon1 - lon2;
    const double a =
        math::pow(math::sin(dlat / 2.0), 2) +
        math::cos(lat1) * math::cos(lat2) * math::pow(math::sin(dlon / 2.0), 2);
    const double c = 2.0 * math::atan2(math::sqrt(a), math::sqrt(1.0 - a));
    return EARTH_RADIUS * c;
}

// Get ratio of segments separated by a point
double GetProjectionRatio(const XYSegment& segment, const XYPoint& point)
{
    const auto& A = segment.first;
    const auto& B = segment.second;
    const auto& P = point;

    // Find such point Q, that vector PQ is orthogonal to the vector AB.
    // Decompose vector PQ for basis (PA, PB):
    //  PQ = (1-k) * PA + k * PB

    double ab = ((B.x - A.x) * (B.x - A.x) + (B.y - A.y) * (B.y - A.y));
    double ao = ((P.x - A.x) * (B.x - A.x) + (P.y - A.y) * (B.y - A.y));

    return ab > EPSILON ? ao / ab : 0.0;
}

// Get projection point by ratio
XYPoint GetRatioPoint(const XYSegment& segment, double ratio)
{
    const auto& A = segment.first;
    const auto& B = segment.second;

    // Find such point P, that vector OP is orthogonal to the vector AB
    // Decompose vector OP for basis (OA, OB):
    //  OP = (1-k) * OA + k * OB
    // Return value: Projected point
    return XYPoint{
        .x = A.x + ratio * (B.x - A.x),
        .y = A.y + ratio * (B.y - A.y)};
}

Projection GetProjection(
    const XYSegment& segment,
    const XYPoint& point,
    double* advance)
{
    const auto& A = segment.first;
    const auto& B = segment.second;

    const auto k = GetProjectionRatio(segment, point);

    Projection projection;
    if (k < 0) {
        projection.point = A;
        projection.distance = GetDist(point, A);
        if (advance) {
            *advance = 0.0;
        }
    } else if (k > 1) {
        projection.point = B;
        projection.distance = GetDist(point, B);
        if (advance) {
            *advance = GetDist(A, B);
        }
    } else {
        const auto xyPoint = GetRatioPoint(segment, k);
        projection.point = xyPoint;
        projection.distance = GetDist(point, xyPoint);
        if (advance) {
            *advance = GetDist(A, xyPoint);
        }
    }

    return projection;
}

Projection GetProjection(
    const XYLineString& line,
    const XYPoint& point,
    double* advance)
{
    NC_ASSERT(!line.points.empty());

    auto result = Projection {
        .point = line.points.front(),
        .distance = GetDist(line.points.front(), point),
    };

    double sumAdvance = 0;
    double resultAdvance = 0;

    for (size_t i = 0; i + 1 < line.points.size(); ++i) {
        double simpleAdvance = 0;
        auto proj = GetProjection(XYSegment{line.points[i], line.points[i + 1]}, point, &simpleAdvance);

        auto dist = GetDist(proj.point, point);
        if (dist < result.distance) {
            result = Projection {
                .point = proj.point,
                .distance = dist,
            };
            resultAdvance = sumAdvance + simpleAdvance;
        }
        sumAdvance += GetDist(line.points[i], line.points[i + 1]);
    }

    if (advance) {
        *advance = resultAdvance;
    }

    return result;
}

Projection GetProjection(const XYRing& ring, const XYPoint& p)
{
    const auto size = geolib::numPoints(ring);

    auto result = Projection{
        .point = ring.points[0],
        .distance = GetNorm(ring.points[0] - p),
    };

    for(size_t i = 0; i + 1 < size; ++i) {
        const auto candidate = GetProjection(XYSegment{ring.points[i], ring.points[i+1]}, p);
        if (result.distance > candidate.distance) {
            result = candidate;
        }
    }

    return result;
}

Projection GetProjection(const XYPolygon& poly, const XYPoint& p)
{
    auto result = GetProjection(poly.outer, p);

    for(const auto& innerRing : poly.inners) {
        const auto candidate = GetProjection(innerRing, p);
        if (result.distance > candidate.distance) {
            result = candidate;
        }
    }

    return result;
}

Projection GetProjection(const XYMultiPolygon& geom, const XYPoint& p)
{
    if (geolib::contains(geom, p)) {
        return Projection{
            .point = p,
            .distance = 0.0,
        };
    }

    auto result = Projection{
        .point = p,
        .distance = std::numeric_limits<double>::infinity(),
    };

    for(const auto& poly : geom.parts) {
        const auto candidate = GetProjection(poly, p);
        if (result.distance > candidate.distance) {
            result = candidate;
        }
    }

    return result;
}

// Get distance from point to segment
double SegmentPointDist(const XYSegment& segment, const XYPoint& point)
{
    return GetProjection(segment, point).distance;
}

// Get distance from line to point
double LinePointDist(const XYSegment& line, const XYPoint& point)
{
    const auto& A = line.first;
    const auto& B = line.second;
    const auto& P = point;

    double d = GetDist(A, B);

    return d >= EPSILON ? std::fabs((B.x - A.x) * (P.y - A.y) - (P.x - A.x) * (B.y - A.y)) / d : NAN;
}

// Check if segments [AB] and [CD] have intersection
bool CheckIntersection(const XYSegment& segmentFirst, const XYSegment& segmentSecond)
{
    static const auto boundBoxCheck = [](double a, double b, double c, double d)
    {
        if (a > b) {
            std::swap(a, b);
        }
        if (c > d) {
            std::swap(c, d);
        }
        return std::max(a,c) < std::min(b,d);
    };

    const auto& A = segmentFirst.first;
    const auto& B = segmentFirst.second;

    const auto& C = segmentSecond.first;
    const auto& D = segmentSecond.second;

    const auto t1 = XYTriangle{.a = A, .b = B, .c = C};
    const auto t2 = XYTriangle{.a = A, .b = B, .c = D};
    const auto t3 = XYTriangle{.a = C, .b = D, .c = A};
    const auto t4 = XYTriangle{.a = C, .b = D, .c = B};

    return
        boundBoxCheck(A.x, B.x, C.x, D.x) &&
        boundBoxCheck(A.y, B.y, C.y, D.y) &&
        GetArea(t1) * GetArea(t2) < 0 &&
        GetArea(t3) * GetArea(t4) < 0;
}

std::optional<XYPoint> LineIntersection(
    const XYPoint& p1,
    const XYPoint& p2,
    const XYPoint& q1,
    const XYPoint& q2,
    double& u,
    double& v)
{
    const double a11 = p2.x - p1.x;
    const double a21 = p2.y - p1.y;
    const double a12 = q1.x - q2.x;
    const double a22 = q1.y - q2.y;
    const double b1 = q1.x - p1.x;
    const double b2 = q1.y - p1.y;

    const double det = Determinant(a11, a12, a21, a22);

    if (std::fabs(det) < std::numeric_limits<double>::epsilon()) {
        return std::nullopt;
    }

    u = Determinant(b1, a12, b2, a22) / det;
    v = Determinant(a11, b1, a21, b2) / det;

    return p1 * (1 - u) + p2 * u;
}

std::optional<XYPoint> SegmentIntersection(
    const XYPoint& p1,
    const XYPoint& p2,
    const XYPoint& q1,
    const XYPoint& q2,
    double& u,
    double& v)
{
    const auto pMinX = std::min(p1.x, p2.x);
    const auto pMaxX = std::max(p1.x, p2.x);
    const auto pMinY = std::min(p1.y, p2.y);
    const auto pMaxY = std::max(p1.y, p2.y);
    const auto qMinX = std::min(q1.x, q2.x);
    const auto qMaxX = std::max(q1.x, q2.x);
    const auto qMinY = std::min(q1.y, q2.y);
    const auto qMaxY = std::max(q1.y, q2.y);

    // Bounding boxes not intersecting => segments not intersecting either
    if (pMaxX < qMinX || qMaxX < pMinX ||
        pMaxY < qMinY || qMaxY < pMinY) {
        return std::nullopt;
    }

    const auto point = LineIntersection(p1, p2, q1, q2, u, v);

    if (!point || u < 0.0 || u > 1.0 || v < 0.0 || v > 1.0) {
        // Segments not intersecting
        return std::nullopt;
    }

    return *point;
}

// Calculate determinant of the 2x2 matrix:
//  a11 a12
//  a21 a22
double Determinant(
    double a11, double a12,
    double a21, double a22)
{
    return a11 * a22 - a12 * a21;
}

// Calculate determinant of the 3x3 matrix:
//  a11 a12 a13
//  a21 a22 a23
//  a31 a32 a33
double Determinant(
    double a11, double a12, double a13,
    double a21, double a22, double a23,
    double a31, double a32, double a33)
{
    return (a11 * a22 * a33 + a12 * a23 * a31 + a21 * a32 * a13) -
           (a21 * a12 * a33 + a11 * a23 * a32 + a13 * a22 * a31);
}

XYPoint GeoToLocal(const GeoPoint& point, const GeoPoint& bindPoint)
{
    const double deltaLat = point.latitude - bindPoint.latitude;
    const double deltaLon = NormalizedAngle(point.longitude - bindPoint.longitude, -180, 180);
    const double latitudeCircumference = EQUATOR_CIRCUMFERENCE_METERS * math::cos(DegToRad(bindPoint.latitude));
    return XYPoint{
        deltaLon * (latitudeCircumference / 360.0),
        deltaLat * (POLE_CIRCUMFERENCE_METERS / 360.0)};
}

GeoPoint LocalToGeo(const XYPoint& localPoint, const GeoPoint& bindPoint)
{
    const double latitudeCircumference = EQUATOR_CIRCUMFERENCE_METERS * math::cos(DegToRad(bindPoint.latitude));
    const double lat = (localPoint.y * (360.0 / POLE_CIRCUMFERENCE_METERS)) + bindPoint.latitude;
    const double lon = NormalizedAngle(localPoint.x * 360.0 / latitudeCircumference + bindPoint.longitude, -180, 180);
    return GeoPoint{lat, lon};
}

XYRing GeoToLocal(const GeoRing& ring, const GeoPoint& bindPoint)
{
    XYRing result;
    for(const auto& point : ring.points) {
        result.points.push_back(GeoToLocal(point, bindPoint));
    }
    geolib::correct(result);
    return result;
}

GeoRing LocalToGeo(const XYRing& ring, const GeoPoint& bindPoint)
{
    GeoRing result;
    for(const auto& point : ring.points) {
        result.points.push_back(LocalToGeo(point, bindPoint));
    }
    geolib::correct(result);
    return result;
}

XYPolygon GeoToLocal(const GeoPolygon& polygon, const GeoPoint& bindPoint)
{
    XYPolygon result;
    result.outer = GeoToLocal(polygon.outer, bindPoint);
    for(const auto& ring : polygon.inners) {
        result.inners.push_back(GeoToLocal(ring, bindPoint));
    }
    geolib::correct(result);
    return result;
}

GeoPolygon LocalToGeo(const XYPolygon& polygon, const GeoPoint& bindPoint)
{
    GeoPolygon result;
    result.outer = LocalToGeo(polygon.outer, bindPoint);
    for(const auto& ring : polygon.inners) {
        result.inners.push_back(LocalToGeo(ring, bindPoint));
    }
    geolib::correct(result);
    return result;
}

XYMultiPolygon GeoToLocal(const GeoMultiPolygon& geometry, const GeoPoint& bindPoint)
{
    XYMultiPolygon result;
    for(const auto& poly : geometry.parts) {
        result.parts.push_back(GeoToLocal(poly, bindPoint));
    }
    return result;
}

GeoMultiPolygon LocalToGeo(const XYMultiPolygon& geometry, const GeoPoint& bindPoint)
{
    GeoMultiPolygon result;
    for(const auto& poly : geometry.parts) {
        result.parts.push_back(LocalToGeo(poly, bindPoint));
    }
    return result;
}

void IterateVertices(
    const XYMultiPolygon& geometry,
    const std::function<void(const XYPoint&)>& func)
{
    for(const auto& poly : geometry.parts) {
        for(const auto& p : poly.outer.points) {
            func(p);
        }
        for(const auto& ring : poly.inners) {
            for(const auto& p : ring.points) {
                func(p);
            }
        }
    }
}

double DegToRad(double deg)
{
    return deg * M_PI / 180.0;
}

double RadToDeg(double rad)
{
    return rad / M_PI * 180.0;
}

double NormalizedAngle(double value, double min, double max)
{
    const double dist = max - min;
    if (dist < EPSILON) {
        return (min + max) / 2;
    }

    return value - std::floor((value - min) / dist) * dist;
}

double GetAverageAngle(
    const std::vector<double>& angles,
    const std::vector<double>& weights)
{
    double ax = 0.0;
    double ay = 0.0;
    for(size_t i = 0; i < angles.size(); ++i) {
        ax += math::cos(angles[i]) * weights.at(i);
        ay += math::sin(angles[i]) * weights.at(i);
    }

    return math::atan2(ay, ax);
}

double GetArea(const XYTriangle& t)
{
    return 0.5 * std::fabs(Determinant(
        (t.a.x - t.c.x), (t.b.x - t.c.x),
        (t.a.y - t.c.y), (t.b.y - t.c.y)));
}

XYPoint GetRandomPoint(const XYTriangle& t, Rng* rng)
{
    double r1 = rng->uniform(0, 1);
    double r2 = rng->uniform(0, 1);

    if (r1 + r2 > 1.0) {
        r1 = 1.0 - r1;
        r2 = 1.0 - r2;
    }

    return XYPoint{
        .x = t.a.x + r1 * (t.b.x - t.a.x) + r2 * (t.c.x - t.a.x),
        .y = t.a.y + r1 * (t.b.y - t.a.y) + r2 * (t.c.y - t.a.y)};
}

XYPoint GetCenter(const XYTriangle& t)
{
    return XYPoint{
        (t.a.x + t.b.x + t.c.x) / 3,
        (t.a.y + t.b.y + t.c.y) / 3};
}

} // namespace navigine::geometry
