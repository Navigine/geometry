#pragma once

#include <navigine/geometry/geolib/detail/contains.h>
#include <navigine/geometry/geolib/detail/correct.h>
#include <navigine/geometry/geolib/detail/graph.h>
#include <navigine/geometry/geolib/detail/iterate.h>
#include <navigine/geometry/geolib/detail/point_helper.h>
#include <navigine/geometry/geolib/detail/type_checks.h>
#include <navigine/geometry/geolib/detail/util.h>
#include <navigine/geometry/geolib/detail/wkt_write.h>

namespace navigine::geometry::geolib::detail {

template <typename Point>
Point snapPoint(const Point& point)
{
    static_assert(isPoint<Point>());

    static const auto epsilon = util::epsilon<Point>();

    Point result;
    util::getX(result) = std::round(util::getX(point) / epsilon) * epsilon;
    util::getY(result) = std::round(util::getY(point) / epsilon) * epsilon;
    return point;
}

enum EdgeType {
    NONE = -1,
    INNER = 0,
    OUTER = 1,
    BORDER = 2,
};

template <typename Point>
EdgeType getEdgeType(
    const Graph<Point>& graph,
    const typename Graph<Point>::Edge& edge,
    typename Graph<Point>::TColor color)
{
    static_assert(isPoint<Point>());

    assert(edge.color != color);

    auto eIter = graph.edgeFind(color, edge.src, edge.dst);

    if (eIter != graph.edgeEnd()) {
#ifdef GEOLIB_DEBUG_ENABLED
        std::cout << "getEdgeType: [color=" << edge.color << ", src=" << edge.src << ", dst=" << edge.dst << "]: BORDER\n";
#endif
        return EdgeType::BORDER;
    }

    if (graph.edgeFind(color, edge.dst, edge.src) != graph.edgeEnd()) {
#ifdef GEOLIB_DEBUG_ENABLED
        std::cout << "getEdgeType: [color=" << edge.color << ", src=" << edge.src << ", dst=" << edge.dst << "]: NONE\n";
#endif
        return EdgeType::NONE;
    }

    const auto point = (edge.srcIter->point + edge.dstIter->point) / 2;
    const auto index = graph.pointIndex(color, point);

#ifdef GEOLIB_DEBUG_ENABLED
    const auto length = distance(edge.srcIter->point, edge.dstIter->point);
    std::cout << "getEdgeType: [color=" << edge.color << ", src=" << edge.src << ", dst=" << edge.dst << ", length=" << length << "]: "
              << "point(" << util::getX(point) << ", " << util::getY(point) << "), index = " << index << "\n";
#endif

    if (!index) {
        return EdgeType::NONE;
    }

    return index > 0 ?
        EdgeType::INNER :
        EdgeType::OUTER;
}

template <typename MultiPolygon>
MultiPolygon makeGeometryFromRings(std::vector<typename MultiPolygon::ring_type>&& rings)
{
    static_assert(isMultiPolygon<MultiPolygon>());

    if (rings.empty()) {
        return {};
    }

    std::vector<std::pair<size_t, double>> inners;
    std::vector<std::pair<size_t, double>> outers;

    for(size_t i = 0; i < rings.size(); ++i) {
        auto area = detail::area(rings[i]);
        if (area > 0) {
            // counter-clockwise ring
            outers.push_back(std::make_pair(i, area));
        } else {
            // clockwise ring
            inners.push_back(std::make_pair(i, -area));
        }
    }

    std::stable_sort(inners.begin(), inners.end(),
        [](const auto& lhs, const auto& rhs) {
            return lhs.second < rhs.second;
        });

    std::stable_sort(outers.begin(), outers.end(),
        [](const auto& lhs, const auto& rhs) {
            return lhs.second < rhs.second;
        });

    if (outers.empty()) {
        // In fact, this is an error.
        return {};
    }

    MultiPolygon result;

    for(auto outer : outers) {
        result.parts.push_back({});
        result.parts.back().outer = std::move(rings[outer.first]);
    }

    for(auto inner : inners) {
        auto innerRing = std::move(rings[inner.first]);
        auto innerRingPoint = (innerRing.points[0] + innerRing.points[1]) / 2;
        for(auto& poly : result.parts) {
            auto index = pointIndex(poly.outer, innerRingPoint);
            if (index == 0) {
                throw std::runtime_error("Error in makeGeometryFromRings: inner-outer ring overlap. Check input geometries.");
            }
            if (index > 0) {
                poly.inners.push_back(std::move(innerRing));
                break;
            }
        }
    }

    detail::correct(result);

    return result;
}

enum class BinaryOperationType {
    UNION = 0,
    INTERSECTION = 1,
    DIFFERENCE = 2,
};

template <typename MultiPolygon, typename Geometry1, typename Geometry2>
MultiPolygon binaryOperation(
    BinaryOperationType op,
    const Geometry1& geometry1,
    const Geometry2& geometry2)
{
#ifdef GEOLIB_DEBUG_ENABLED
    std::cout << wkt(geometry1, 20) << "\n";
    std::cout << wkt(geometry2, 20) << "\n";
#endif

    static_assert(std::is_same<typename Geometry1::point_type, typename Geometry2::point_type>::value == true);
    static_assert(std::is_same<typename Geometry1::point_type, typename MultiPolygon::point_type>::value == true);
    static_assert(isBox<Geometry1>() || isRing<Geometry1>() || isPolygon<Geometry1>() || isMultiPolygon<Geometry1>());
    static_assert(isBox<Geometry2>() || isRing<Geometry2>() || isPolygon<Geometry2>() || isMultiPolygon<Geometry2>());
    static_assert(isMultiPolygon<MultiPolygon>());

    using Point = typename MultiPolygon::point_type;
    using Ring = typename MultiPolygon::ring_type;
    using VertexIterator = typename Graph<Point>::VertexIterator;
    using EdgeIterator = typename Graph<Point>::EdgeIterator;
    using TId = typename Graph<Point>::TId;
    using TColor = typename Graph<Point>::TColor;
    using Edge = typename Graph<Point>::Edge;

    Graph<Point> graph(2);

    iterateEdges(geometry1, false,
        [&](const Point& p1, const Point& p2, const Point&, const Point&) {
            auto srcIter = graph.vertexAdd(snapPoint(p1));
            auto dstIter = graph.vertexAdd(snapPoint(p2));
            graph.edgeAdd(Edge{
                .color = 0,
                .src = srcIter->id,
                .dst = dstIter->id,
                .srcIter = srcIter,
                .dstIter = dstIter});
        });

    iterateEdges(geometry2, op == BinaryOperationType::DIFFERENCE,
        [&](const Point& p1, const Point& p2, const Point&, const Point&) {
            auto srcIter = graph.vertexAdd(snapPoint(p1));
            auto dstIter = graph.vertexAdd(snapPoint(p2));
            graph.edgeAdd(Edge{
                .color = 1,
                .src = srcIter->id,
                .dst = dstIter->id,
                .srcIter = srcIter,
                .dstIter = dstIter});
        });

#ifdef GEOLIB_DEBUG_ENABLED
    std::cout << graph << "\n";
#endif

    graph.correct();

//#ifdef GEOLIB_DEBUG_ENABLED
//    std::cout << graph << "\n";
//#endif

    static const auto filterEdge =
        [](EdgeType type, TColor color, BinaryOperationType op) {
            switch (op) {
                case BinaryOperationType::UNION:
                    return type == EdgeType::OUTER || (type == EdgeType::BORDER && color == 0);
                case BinaryOperationType::INTERSECTION:
                    return type == EdgeType::INNER || (type == EdgeType::BORDER && color == 0);
                case BinaryOperationType::DIFFERENCE:
                    return (type == EdgeType::OUTER && color == 0) ||
                           (type == EdgeType::INNER && color > 0) ||
                           (type == EdgeType::BORDER && color == 0);
                default:
                    return false;
            }
        };

    std::vector<EdgeIterator> edgesToRemove;

    for(TColor color = 0; color < 2; ++color) {
        auto otherColor = color == 0 ? 1 : 0;
        std::optional<Edge> prevEdge;
        std::optional<EdgeType> prevType;
        graph.iterateEdgesByColor(color,
            [&](auto eIter) {
                auto type = (prevEdge && prevEdge->dst == eIter->src && eIter->srcIter->color == color) ?
                    *prevType : getEdgeType(graph, *eIter, otherColor);
                //auto type = getEdgeType(graph, *eIter, otherColor);
                if (!filterEdge(type, color, op)) {
                    edgesToRemove.push_back(eIter);
                }
                prevEdge = *eIter;
                prevType = type;
            });
    }

    for(auto eIter : edgesToRemove) {
#ifdef GEOLIB_DEBUG_ENABLED
        std::cout << "Filtering out edge [color=" << eIter->color << ", src=" << eIter->src << ", dst=" << eIter->dst << "]\n";
#endif
        graph.edgeRemove(eIter);
    }

//#ifdef GEOLIB_DEBUG_ENABLED
//    std::cout << graph << "\n";
//#endif

    std::vector<Ring> rings;

    while (graph.edgeBegin() != graph.edgeEnd()) {
        std::vector<VertexIterator> ipath;
        std::unordered_set<TId> vids;

        auto eIter = graph.edgeBegin();
        ipath.push_back(eIter->srcIter);
        vids.insert(eIter->src);

        while (eIter != graph.edgeEnd()) {
            const auto dst = eIter->dst;
            ipath.push_back(eIter->dstIter);

            if (!vids.insert(eIter->dst).second) {
                auto startIter = std::find(ipath.begin(), ipath.end(), ipath.back());
                assert(std::next(startIter) != ipath.end());

#ifdef GEOLIB_DEBUG_ENABLED
                std::cout << "ipath: ";
                for(auto vIter : ipath) {
                    std::cout << vIter->id << " ";
                }
                std::cout << "\n";
#endif

                Ring ring;
                for(auto iter = startIter; iter != ipath.end(); ++iter) {
                    ring.points.push_back((*iter)->point);
                }
                rings.push_back(std::move(ring));

                ipath.erase(std::next(startIter), ipath.end());
                vids.clear();
                for(const auto& iter : ipath) {
                    vids.insert(iter->id);
                }
            }
            graph.edgeRemove(eIter);
            eIter = graph.edgeFind(std::nullopt, dst);
        }

        if (ipath.size() >= 2) {
#ifdef GEOLIB_DEBUG_ENABLED
            std::cout << "incomplete ring: ";
            for(auto vIter : ipath) {
                std::cout << vIter->id << " ";
            }
            std::cout << "\n";
#endif
            throw std::runtime_error("Error in binaryOperation: incomplete ring. Check input geometries.");
        }
    }

    return makeGeometryFromRings<MultiPolygon>(std::move(rings));
}

} // namespace navigine::geometry::geolib::detail
