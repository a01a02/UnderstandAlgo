// Created by Aaron H 5/28/24
#ifndef ISCYCLIC_HPP
#define ISCYCLIC_HPP
#include "../../Structures/ADT/Graph.hpp"
namespace GraphAlgorithms {

    template <typename VerticeType, typename EdgeType>
    bool isCyclic(DerivedGraph<VerticeType, EdgeType>& graph);

    template <typename VerticeType, typename EdgeType>
    bool isCyclicUtil(DerivedGraph<VerticeType, EdgeType>& graph, const VerticeType &vertex, std::unordered_map<VerticeType, bool> &visited, std::unordered_map<VerticeType, bool> &recursionStack);

}  // namespace GraphAlgorithms
#include "IsCyclic.tpp"
#endif

