#include <navigine/geometry/geometry.h>

using namespace navigine::geometry;

int test_geometry_segment_point_dist_close_point()
{
    // point close to segment
    double dist = SegmentPointDist(
      XYSegment{XYPoint{2, 4}, XYPoint{7, 4}},
      XYPoint{3, 3});

    if (!(std::fabs(dist - 1.0) < 0.01)) {
      return 1;
    }

    return 0;
}

int test_geometry_segment_point_dist_far_point()
{
    double dist = SegmentPointDist(
      XYSegment{XYPoint{2, 4}, XYPoint{7, 4}},
      XYPoint{1, 7});

    if (!(std::fabs(dist - 3.16) < 0.1)) {
      return 1;
    }

    return 0;
}

int test_geometry_segment_point_dist_point_on_segment()
{
    // point on segment
    double dist = SegmentPointDist(
      XYSegment{XYPoint{2, 4}, XYPoint{7, 4}},
      XYPoint{5, 4});

    if (!(std::fabs(dist - 0) < 0.01)) {
      return 1;
    }

    return 0;
}

int test_geometry_segment_point_dist_zero_len_segment()
{
    // point on segment
    double dist = SegmentPointDist(
      XYSegment{XYPoint{2, 4}, XYPoint{2, 4}},
      XYPoint{1, 3});

    if (!(std::fabs(dist - 1.41) < 1)) {
      return 1;
    }

    return 0;
}
