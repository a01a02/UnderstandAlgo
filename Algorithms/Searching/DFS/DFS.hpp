// DFS.hpp
#ifndef DFS_HPP
#define DFS_HPP

#include "../../../Structures/ADT/Graph.hpp"
#include <unordered_map>

namespace Searching {

    template <typename VerticeType, typename EdgeType>
    void DFSUtil(DerivedGraph<VerticeType, EdgeType>& graph, const VerticeType& vertex, std::unordered_map<VerticeType, bool>& visited);

    template <typename VerticeType, typename EdgeType>
    void DFS(DerivedGraph<VerticeType, EdgeType>& graph, const VerticeType& start);

}  // end of namespace Searching

#endif  // DFS_HPP