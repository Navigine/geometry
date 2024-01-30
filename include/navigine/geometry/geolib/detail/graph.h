#pragma once

#include <navigine/geometry/geolib/detail/contains.h>
#include <navigine/geometry/geolib/detail/compare.h>
#include <navigine/geometry/geolib/detail/intersection.h>
#include <navigine/geometry/geolib/detail/point_helper.h>
#include <navigine/geometry/geolib/detail/type_checks.h>
#include <navigine/geometry/geolib/detail/util.h>

namespace navigine::geometry::geolib::detail {

template <typename Point>
class Graph
{
public:
    using TId = unsigned int;
    using TColor = unsigned int;

    struct Vertex
    {
        Point point;
        TId id;
        std::optional<TColor> color = std::nullopt;

        inline bool operator==(const Vertex& v) const {
            return id == v.id;
        }

        inline bool operator<(const Vertex& v) const {
            return id < v.id;
        }
    };

    using VertexIterator = typename std::set<Vertex>::const_iterator;

    struct Edge
    {
        TColor color;
        TId src;
        TId dst;
        VertexIterator srcIter;
        VertexIterator dstIter;

        inline bool operator==(const Edge& e) const {
            return color == e.color && src == e.src && dst == e.dst;
        }

        inline bool operator<(const Edge& e) const {
            return (color < e.color) ||
                   (color == e.color && src < e.src) ||
                   (color == e.color && src == e.src && dst < e.dst);
        }
    };

    using EdgeIterator = typename std::set<Edge>::const_iterator;
    using EdgeFunction = std::function<void(EdgeIterator)>;

    Graph(TColor maxColors = 2)
        : maxColors_(maxColors)
        , nextId_(0)
    {}

    TColor maxColors() const {
        return maxColors_;
    }

    size_t vertexCount() const {
        return vertices_.size();
    }

    VertexIterator vertexAdd(Point point) {
        point = snapPoint(point);
        auto iter = pointMap_.find(point);
        if (iter != pointMap_.end()) {
            return iter->second;
        }

        const auto id = nextId_++;

        const auto vIter = vertices_.insert(Vertex{point, id}).first;

        vertexMap_[id] = vIter;
        pointMap_[point] = vIter;

        return vIter;
    }

    VertexIterator vertexBegin() const {
        return vertices_.begin();
    }

    VertexIterator vertexEnd() const {
        return vertices_.end();
    }

    VertexIterator vertexFind(TId id) const {
        auto iter = vertexMap_.find(id);
        if (iter == vertexMap_.end()) {
            return vertices_.end();
        }
        return iter->second;
    }

    size_t edgeCount() const {
        return edges_.size();
    }

    void edgeAdd(const Edge& e) {
        if (e.src == e.dst) {
            return;
        }

        auto p = edges_.insert(e);
        assert(e.color < maxColors_);
        assert(p.second && "trying to insert multiedge");
    }

    void edgeRemove(EdgeIterator iter) {
        edges_.erase(iter);
    }

    EdgeIterator edgeBegin() const {
        return edges_.begin();
    }

    EdgeIterator edgeEnd() const {
        return edges_.end();
    }

    EdgeIterator edgeFind(TColor color) const {
        auto iter = edges_.lower_bound(makeFakeEdge(color, 0, 0));
        if (iter != edges_.end() && iter->color == color) {
            return iter;
        }
        return edges_.end();
    }

    EdgeIterator edgeFind(std::optional<TColor> color, TId src) const {
        if (color) {
            auto iter = edges_.lower_bound(makeFakeEdge(*color, src, 0));
            if (iter != edges_.end() && iter->color == *color && iter->src == src) {
                return iter;
            }
        } else {
            // Any color
            for(TColor color = 0; color < maxColors_; ++color) {
                auto iter = edges_.lower_bound(makeFakeEdge(color, src, 0));
                if (iter != edges_.end() && iter->src == src) {
                    return iter;
                }
            }
        }
        return edges_.end();
    }

    EdgeIterator edgeFind(std::optional<TColor> color, TId src, TId dst) const {
        if (color) {
            auto iter = edges_.lower_bound(makeFakeEdge(*color, src, dst));
            if (iter != edges_.end() && iter->color == *color && iter->src == src && iter->dst == dst) {
                return iter;
            }
        } else {
            // Any color
            for(TColor color = 0; color < maxColors_; ++color) {
                auto iter = edges_.lower_bound(makeFakeEdge(color, src, dst));
                if (iter != edges_.end() && iter->src == src && iter->dst == dst) {
                    return iter;
                }
            }
        }
        return edges_.end();
    }

    void iterateEdgesByColor(TColor color, const EdgeFunction& func) const {
        auto iter = edges_.lower_bound(makeFakeEdge(color, 0, 0));
        while (iter != edges_.end() && iter->color == color) {
            func(iter++);
        }
    }

    void correct() {
        static const auto comp =
            [](EdgeIterator eIter1, EdgeIterator eIter2) {
                return *eIter1 < *eIter2;
            };

        std::map<
            EdgeIterator,
            std::multimap<double, Point>,
            decltype(comp)> crossPoints(comp);

        // Crossing edges of different colors. Splitting edges by the cross points.
        for(TColor color1 = 0; color1 < maxColors_; ++color1) {
            for(TColor color2 = color1 + 1; color2 < maxColors_; ++color2) {
                const auto startIter1 = edgeFind(color1);
                const auto startIter2 = edgeFind(color2);
                for(auto eIter1 = startIter1; eIter1 != edges_.end() && eIter1->color == color1; ++eIter1) {
                    for(auto eIter2 = startIter2; eIter2 != edges_.end() && eIter2->color == color2; ++eIter2) {

                        double u1, u2, v1, v2;
                        Point i1, i2;

                        int count = segmentIntersection<Point>(
                            eIter1->srcIter->point,
                            eIter1->dstIter->point,
                            eIter2->srcIter->point,
                            eIter2->dstIter->point,
                            u1, v1, i1,
                            u2, v2, i2);

                        if (count >= 1) {
#ifdef GEOLIB_DEBUG_ENABLED
                            auto d1 = distance(eIter1->srcIter->point, eIter1->dstIter->point);
                            auto d2 = distance(eIter2->srcIter->point, eIter2->dstIter->point);
                            std::cout << "cross: " << eIter1->src << "->" << eIter1->dst << " and "
                                      << eIter2->src << "->" << eIter2->dst << ", "
                                      << "u1=" << u1 << ", v1=" << v1 << ", "
                                      << "i1=Point(" << util::getX(i1) << ", " << util::getY(i1) << "), "
                                      << "d1=" << d1 << ", d2=" << d2 << "\n";
#endif
                            if (0 < u1 && u1 < 1) {
                                crossPoints[eIter1].emplace(u1, i1);
                            }
                            if (0 < v1 && v1 < 1) {
                                crossPoints[eIter2].emplace(v1, i1);
                            }
                        }

                        if (count >= 2) {
#ifdef GEOLIB_DEBUG_ENABLED
                            auto d1 = distance(eIter1->srcIter->point, eIter1->dstIter->point);
                            auto d2 = distance(eIter2->srcIter->point, eIter2->dstIter->point);
                            std::cout << "cross: " << eIter1->src << "->" << eIter1->dst << " and "
                                      << eIter2->src << "->" << eIter2->dst << ", "
                                      << "u2=" << u2 << ", v2=" << v2 << ", "
                                      << "i2=Point(" << util::getX(i2) << ", " << util::getY(i2) << "), "
                                      << "d1=" << d1 << ", d2=" << d2 << "\n";
#endif
                            if (0 < u2 && u2 < 1) {
                                crossPoints[eIter1].emplace(u2, i2);
                            }
                            if (0 < v2 && v2 < 1) {
                                crossPoints[eIter2].emplace(v2, i2);
                            }
                        }
                    }
                }
            }
        }

        for(auto& [eIter, points] : crossPoints) {
            auto color = eIter->color;
            auto srcIter = eIter->srcIter;
            auto dstIter = eIter->dstIter;
#ifdef GEOLIB_DEBUG_ENABLED
            std::cout << "Splitting edge [color=" << color << ", src=" << srcIter->id << ", dst=" << dstIter->id << "]\n";
#endif
            edgeRemove(eIter);
            for(const auto& [_, point] : points) {
                auto dstIter = vertexAdd(point);
#ifdef GEOLIB_DEBUG_ENABLED
                std::cout << " - adding point (" << util::getX(point) << ", " << util::getY(point) << ") => Vertex[" << dstIter->id << "]\n";
                std::cout << " - adding edge [color=" << color << ", src=" << srcIter->id << ", dst=" << dstIter->id << "]\n";
#endif
                edgeAdd(Edge{
                    .color = color,
                    .src = srcIter->id,
                    .dst = dstIter->id,
                    .srcIter = srcIter,
                    .dstIter = dstIter});
                srcIter = dstIter;
            }
#ifdef GEOLIB_DEBUG_ENABLED
            std::cout << " - adding edge [color=" << color << ", src=" << srcIter->id << ", dst=" << dstIter->id << "]\n";
#endif
            edgeAdd(Edge{
                .color = color,
                .src = srcIter->id,
                .dst = dstIter->id,
                .srcIter = srcIter,
                .dstIter = dstIter});
        }

        for(const auto& e : edges_) {
            auto& src = const_cast<Vertex&>(*e.srcIter);
            auto& dst = const_cast<Vertex&>(*e.dstIter);
            if (!src.color) {
                src.color = e.color;
            } else if (src.color != e.color) {
                src.color = std::numeric_limits<TColor>::max();
            }
            if (!dst.color) {
                dst.color = e.color;
            } else if (dst.color != e.color) {
                dst.color = std::numeric_limits<TColor>::max();
            }
        }
    }

    int pointIndex(TColor color, const Point& point) const {
        int index = -1;
        iterateEdgesByColor(color,
            [&](auto eIter) {
                index = updatePointIndex(
                    point,
                    eIter->srcIter->point,
                    eIter->dstIter->point,
                    index);
            });
        return index;
    }

private:
    Edge makeFakeEdge(TColor color, TId src, TId dst) const {
        return Graph::Edge{
            .color = color,
            .src = src,
            .dst = dst,
            .srcIter = {},
            .dstIter = {}};
    }

    const TColor maxColors_;

    std::set<Vertex> vertices_;
    std::unordered_map<TId, VertexIterator> vertexMap_;
    std::unordered_map<Point, VertexIterator, PointHasher<Point>, PointComparator<Point>> pointMap_;
    std::set<Edge> edges_;
    TId nextId_;
};

template <typename Point>
std::ostream& operator<<(std::ostream& os, const Graph<Point>& graph)
{
    os << "Vertices:\n";
    for(auto iter = graph.vertexBegin(); iter != graph.vertexEnd(); ++iter) {
        os << "  Vertex[" << iter->id << "] = Point(" << util::getX(iter->point) << ", " << util::getY(iter->point) << ")\n";
    }
    os << "Edges:\n";
    for(auto iter = graph.edgeBegin(); iter != graph.edgeEnd(); ++iter) {
        os << "  Edge[color=" << iter->color << ", src=" << iter->src << ", dst=" << iter->dst << "]\n";
    }
    return os;
}

} // namespace navigine::geometry::geolib::detail
