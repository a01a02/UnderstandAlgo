#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_map>
#include <vector>

enum GraphType {
    DAG,
    UDG
};

template<typename VerticeType, typename EdgeType>
class Graph {
public:
    [[maybe_unused]] virtual void addVertex(const VerticeType& vertex) = 0;
    [[maybe_unused]] virtual void removeVertex(const VerticeType& vertex) = 0;
    [[maybe_unused]] virtual void addEdge(const VerticeType &source, const VerticeType &destination, const EdgeType &weight, bool checkForCycle) = 0;
    [[maybe_unused]] virtual void addDirectionalEdge(const VerticeType& source, const VerticeType& destination,
                                                     const EdgeType& weight, bool isDirected, bool checkForCycle = true) = 0;
    [[maybe_unused]] virtual void removeEdge(const VerticeType& vertex1, const VerticeType& vertex2) = 0;
    [[maybe_unused]] [[nodiscard]] virtual unsigned int numVertices() const = 0;
    [[maybe_unused]] [[nodiscard]] virtual unsigned int numEdges() const = 0;
};

template<typename VerticeType, typename EdgeType>
class DerivedGraph: public Graph<VerticeType, EdgeType> {
private:
    std::unordered_map<VerticeType, std::vector<std::pair<VerticeType, EdgeType>>> adjacencyList;
    GraphType graphType;
public:
    DerivedGraph();
    DerivedGraph(GraphType type) : graphType(type), adjacencyList() {}
    DerivedGraph(const DerivedGraph& other);
    DerivedGraph(DerivedGraph&& other) noexcept;
    DerivedGraph& operator=(const DerivedGraph& other);
    DerivedGraph& operator=(DerivedGraph&& other) noexcept;

    void addVertex(const VerticeType& vertex) override;

    void removeVertex(const VerticeType &vertex) override;

    void addEdge(const VerticeType &source, const VerticeType &destination, const EdgeType &weight, bool checkForCycle) override;

    void addDirectionalEdge(const VerticeType &source, const VerticeType &destination, const EdgeType &weight, bool isDirected,
                            bool checkForCycle = true) override;

    void removeEdge(const VerticeType &vertex1, const VerticeType &vertex2) override;

    [[nodiscard]] unsigned int numVertices() const override;

    [[nodiscard]] unsigned int numEdges() const override;

    bool hasEdge(const VerticeType &v1, const VerticeType &v2) const;

    [[maybe_unused]] static DerivedGraph from_edges(const std::vector<std::tuple<VerticeType, VerticeType, EdgeType>>& edges, GraphType type);

    std::vector<VerticeType> getVertices() const;

    auto adjacentBegin(const VerticeType& vertex) -> typename decltype(adjacencyList)::mapped_type::iterator;
    auto adjacentEnd(const VerticeType& vertex) -> typename decltype(adjacencyList)::mapped_type::iterator;
};
#include "Graph.tpp"
#endif