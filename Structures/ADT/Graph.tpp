#include "Graph.hpp"
#include "../../Algorithms/GraphAlgorithms/IsCyclic.hpp"

//Begin implementation of ElementaryGraph template methods

template<typename VerticeType, typename EdgeType>
DerivedGraph<VerticeType, EdgeType>::DerivedGraph() {
    adjacencyList = std::unordered_map<VerticeType, std::vector<std::pair<VerticeType, EdgeType>>>();
}

template<typename VerticeType, typename EdgeType>
DerivedGraph<VerticeType, EdgeType>::DerivedGraph(const DerivedGraph<VerticeType, EdgeType>& other) {
    adjacencyList = other.adjacencyList;
    graphType = other.graphType;
}

template<typename VerticeType, typename EdgeType>
DerivedGraph<VerticeType, EdgeType>::DerivedGraph(DerivedGraph<VerticeType, EdgeType>&& other) noexcept {
    adjacencyList = std::move(other.adjacencyList);
    graphType = std::move(other.graphType);
}

template<typename VerticeType, typename EdgeType>
DerivedGraph<VerticeType, EdgeType>& DerivedGraph<VerticeType, EdgeType>::operator=(const DerivedGraph<VerticeType, EdgeType>& other) {
    if (&other != this) {
        adjacencyList = other.adjacencyList;
        graphType = other.graphType;
    }
    return *this;
}

template<typename VerticeType, typename EdgeType>
DerivedGraph<VerticeType, EdgeType>& DerivedGraph<VerticeType, EdgeType>::operator=(DerivedGraph<VerticeType, EdgeType>&& other) noexcept {
    if (&other != this) {
        adjacencyList = std::move(other.adjacencyList);
        graphType = std::move(other.graphType);
    }
    return *this;
}

template<typename VerticeType, typename EdgeType>
void DerivedGraph<VerticeType, EdgeType>::addVertex(const VerticeType& vertex) {
    if (adjacencyList.find(vertex) != adjacencyList.end()) {
        throw std::runtime_error("Vertex already exists in the graph");
    }
    adjacencyList.emplace(vertex, std::vector<std::pair<VerticeType, EdgeType>>());
}

template<typename VerticeType, typename EdgeType>
void DerivedGraph<VerticeType, EdgeType>::removeVertex(const VerticeType &vertex) {
    auto it = adjacencyList.find(vertex);
    if (it == adjacencyList.end()) {
        throw std::runtime_error("Vertex does not exist in the graph");
    }
    adjacencyList.erase(it);
    for (auto &vertexPair : adjacencyList) {
        vertexPair.second.erase(
                std::remove_if(vertexPair.second.begin(), vertexPair.second.end(),
                               [&vertex](const std::pair<VerticeType, EdgeType>& edgePair) {
                                   return edgePair.first == vertex;
                               }),
                vertexPair.second.end()
        );
    }
}

template<typename VerticeType, typename EdgeType>
void DerivedGraph<VerticeType, EdgeType>::addEdge(const VerticeType &source, const VerticeType &destination, const EdgeType &weight, bool checkForCycle) {
    if (adjacencyList.find(source) == adjacencyList.end() || adjacencyList.find(destination) == adjacencyList.end()) {
        throw std::runtime_error("One or both vertices do not exist in the graph");
    }
    auto& sourceEdges = adjacencyList[source];
    sourceEdges.reserve(sourceEdges.size() + 1);
    if (std::any_of(sourceEdges.begin(), sourceEdges.end(), [&destination](const auto& pair) {
        return pair.first == destination;
    })) {
        throw std::runtime_error("An edge between these vertices already exists.");
    }
    sourceEdges.emplace_back(destination, weight);
    if (checkForCycle && this->graphType == DAG) {
        if (GraphAlgorithms::isCyclic(*this)) {
            sourceEdges.pop_back();
            throw std::runtime_error("Edge creation results in a cycle in the graph");
        }
    }
}

template<typename VerticeType, typename EdgeType>
void DerivedGraph<VerticeType, EdgeType>::addDirectionalEdge(const VerticeType &source, const VerticeType &destination, const EdgeType &weight, bool isDirected, bool checkForCycle) {
    addEdge(source, destination, weight, checkForCycle);
    if (!isDirected) {
        addEdge(destination, source, weight, checkForCycle);
    }
}

template<typename VerticeType, typename EdgeType>
void DerivedGraph<VerticeType, EdgeType>::removeEdge(const VerticeType& vertex1, const VerticeType& vertex2) {
    auto it1 = adjacencyList.find(vertex1);
    auto it2 = adjacencyList.find(vertex2);
    if (it1 == adjacencyList.end() || it2 == adjacencyList.end()) {
        throw std::runtime_error("One or both vertices do not exist in the graph");
    }
    auto& edges1 = it1->second;
    auto edgeToBeRemoved1 = std::find_if(edges1.begin(), edges1.end(),
                                         [&vertex2](auto const &edge) { return edge.first == vertex2; });
    if (edgeToBeRemoved1 != edges1.end()) {
        edges1.erase(std::remove_if(edges1.begin(), edges1.end(),
                                    [&vertex2](auto const &edge) { return edge.first == vertex2; }),
                     edges1.end());
    } else {
        throw std::runtime_error("Edge does not exist in the graph");
    }
    auto& edges2 = it2->second;
    auto edgeToBeRemoved2 = std::find_if(edges2.begin(), edges2.end(),
                                         [&vertex1](auto const &edge) { return edge.first == vertex1; });
    if (edgeToBeRemoved2 != edges2.end()) {
        edges2.erase(std::remove_if(edges2.begin(), edges2.end(),
                                    [&vertex1](auto const &edge) { return edge.first == vertex1; }),
                     edges2.end());
    }
}

template<typename VerticeType, typename EdgeType>
unsigned int DerivedGraph<VerticeType, EdgeType>::numVertices() const {
    return adjacencyList.size();
}

template<typename VerticeType, typename EdgeType>
unsigned int DerivedGraph<VerticeType, EdgeType>::numEdges() const {
    unsigned int count = 0;
    for (const auto &vertexPair : adjacencyList) {
        count += vertexPair.second.size();
    }
    return count;
}

template<typename VerticeType, typename EdgeType>
bool DerivedGraph<VerticeType, EdgeType>::hasEdge(const VerticeType &v1, const VerticeType &v2) const {
    auto it = adjacencyList.find(v1);
    if (it == adjacencyList.end()) {
        return false;
    }
    for (const auto& adjVertex : it->second) {
        if (adjVertex.first == v2) {
            return true;
        }
    }
    return false;
}

template<typename VerticeType, typename EdgeType>
DerivedGraph<VerticeType, EdgeType> DerivedGraph<VerticeType, EdgeType>::from_edges(const std::vector<std::tuple<VerticeType, VerticeType, EdgeType>>& edges, GraphType type) {
    DerivedGraph<VerticeType, EdgeType> g(type);
    for (const auto& edge : edges) {
        VerticeType source, destination;
        EdgeType weight;
        std::tie(source, destination, weight) = edge;
        if (g.adjacencyList.find(source) == g.adjacencyList.end()) {
            g.addVertex(source);
        }
        if (g.adjacencyList.find(destination) == g.adjacencyList.end()) {
            g.addVertex(destination);
        }
        g.addEdge(source, destination, weight, true);
    }
    return g;
}

template<typename VerticeType, typename EdgeType>
std::vector<VerticeType> DerivedGraph<VerticeType, EdgeType>::getVertices() const {
    std::vector<VerticeType> vertices;
    vertices.reserve(adjacencyList.size());
    for (const auto& pair : adjacencyList) {
        vertices.push_back(pair.first);
    }
    return vertices;
}

template<typename VerticeType, typename EdgeType>
auto DerivedGraph<VerticeType, EdgeType>::adjacentBegin(const VerticeType& vertex) -> typename decltype(adjacencyList)::mapped_type::iterator {
    return adjacencyList[vertex].begin();
}

template<typename VerticeType, typename EdgeType>
auto DerivedGraph<VerticeType, EdgeType>::adjacentEnd(const VerticeType& vertex) -> typename decltype(adjacencyList)::mapped_type::iterator {
    return adjacencyList[vertex].end();
}
