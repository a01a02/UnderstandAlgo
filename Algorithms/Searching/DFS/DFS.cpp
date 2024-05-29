// DFS.cpp
#include "DFS.hpp"

namespace Searching {

    template <typename VerticeType, typename EdgeType>
    void DFSUtil(DerivedGraph<VerticeType, EdgeType>& graph, const VerticeType& vertex, std::unordered_map<VerticeType, bool>& visited) {
        // mark the current node as visited
        visited[vertex] = true;

        // visit all the vertices adjacent to this vertex
        for (auto iter = graph.adjacentBegin(vertex); iter != graph.adjacentEnd(vertex); ++iter) {
            if (!visited[*iter]) {
                DFSUtil(graph, *iter, visited);
            }
        }
    }

    template <typename VerticeType, typename EdgeType>
    void DFS(DerivedGraph<VerticeType, EdgeType>& graph, const VerticeType& start) {
        std::unordered_map<VerticeType, bool> visited;

        // call the recursive helper function to print DFS traversal
        DFSUtil(graph, start, visited);
    }

}  // end of namespace Searching