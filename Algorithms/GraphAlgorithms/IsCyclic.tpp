// IsCyclic.tpp
#include "../Searching/DFS/DFS.hpp"

namespace GraphAlgorithms {

    template <typename VerticeType, typename EdgeType>
    bool isCyclicUtil(DerivedGraph<VerticeType, EdgeType>& graph, const VerticeType &vertex,
                      std::unordered_map<VerticeType, bool> &visited,
                      std::unordered_map<VerticeType, bool> &recursionStack) {
        if (visited[vertex]) {
            return recursionStack[vertex]; // If visited and in recursion stack, it's a cycle
        }

        visited[vertex] = true;
        recursionStack[vertex] = true;

        for (auto iter = graph.adjacentBegin(vertex); iter != graph.adjacentEnd(vertex); ++iter) {
            auto& adjacentVertex = iter->first;  // Assuming *iter gives std::pair
            if (isCyclicUtil(graph, adjacentVertex, visited, recursionStack)) {
                return true;
            }
        }

        recursionStack[vertex] = false;  // Remove the vertex from recursion stack
        return false;
    }

    template <typename VerticeType, typename EdgeType>
    bool isCyclic(DerivedGraph<VerticeType, EdgeType>& graph) {
        std::unordered_map<VerticeType, bool> visited;
        std::unordered_map<VerticeType, bool> recursionStack;

        const auto& vertices = graph.getVertices();  // Optimize to avoid repeated calls
        for (const auto& vertex : vertices) {
            if (!visited[vertex]) {  // Only process unvisited nodes to avoid redundant work
                if (isCyclicUtil(graph, vertex, visited, recursionStack)) {
                    return true;
                }
            }
        }

        return false;
    }
}  // namespace GraphAlgorithms
