// Created by Aaron H on 5/28/24.
#include "../Structures/ADT/Graph.hpp"
#include <gtest/gtest.h>
#include <unordered_set>
#include <iostream>
#include <chrono>
namespace {

    TEST(DerivedGraphTest, CopySemantics) {
        DerivedGraph<int, int> graph(DAG);
        graph.addVertex(1);
        graph.addVertex(2);
        graph.addEdge(1, 2, 3, true);

        // Copy constructor
        DerivedGraph<int, int> copyGraph(graph);
        ASSERT_TRUE(copyGraph.hasEdge(1, 2));

        // Copy assignment
        DerivedGraph<int, int> anotherGraph;
        anotherGraph = graph;
        ASSERT_TRUE(anotherGraph.hasEdge(1, 2));
    }

    TEST(DerivedGraphTest, MoveSemantics) {
        DerivedGraph<int, int> graph(DAG);
        graph.addVertex(1);
        graph.addVertex(2);
        graph.addEdge(1, 2, 3, true);

        // Move constructor
        DerivedGraph<int, int> moveGraph(std::move(graph));
        ASSERT_TRUE(moveGraph.hasEdge(1, 2));

        // Create another graph to test move assignment
        DerivedGraph<int, int> graph2(DAG);
        graph2.addVertex(3);
        graph2.addVertex(4);
        graph2.addEdge(3, 4, 5, true);

        // Move assignment
        moveGraph = std::move(graph2);
        ASSERT_FALSE(moveGraph.hasEdge(1, 2));
        ASSERT_TRUE(moveGraph.hasEdge(3, 4));
    }

    TEST(DerivedGraphTest, TestNumVertices) {
        DerivedGraph<int, int> graph(DAG);
        ASSERT_EQ(graph.numVertices(), 0);
        graph.addVertex(1);
        ASSERT_EQ(graph.numVertices(), 1);
    }

    TEST(DerivedGraphTest, TestNumEdges) {
        DerivedGraph<int, int> graph(DAG);
        ASSERT_EQ(graph.numEdges(), 0);
        graph.addVertex(1);
        graph.addVertex(2);
        graph.addEdge(1, 2, 3, true);
        ASSERT_EQ(graph.numEdges(), 1);
    }

    TEST(DerivedGraphTest, TestAddVertex) {
        DerivedGraph<int, int> graph(DAG);
        graph.addVertex(1);
        ASSERT_EQ(graph.numVertices(), 1);
    }

    TEST(DerivedGraphTest, AddExistingVertex) {
        DerivedGraph<int, int> graph(DAG);
        graph.addVertex(1);
        // Modify this to match your error handling implementation.
        ASSERT_THROW(graph.addVertex(1), std::runtime_error);
    }

    TEST(DerivedGraphTest, TestAddEdge) {
        DerivedGraph<int, int> graph(DAG);
        graph.addVertex(1);
        graph.addVertex(2);
        ASSERT_NO_THROW(graph.addEdge(1, 2, 3, true));
        ASSERT_EQ(graph.numEdges(), 1);
        // Add an assertion to confirm that it throws an exception when introducing a cycle
        graph.addVertex(3);
        graph.addEdge(2, 3, 4, true);
        ASSERT_THROW(graph.addEdge(3, 1, 1, true), std::runtime_error);
    }

    TEST(DerivedGraphTest, AddEdgeWithNonexistentVertex) {
        DerivedGraph<int, int> graph(DAG);
        // Modify this according to your error handling implementation.
        ASSERT_THROW(graph.addEdge(1, 2, 3, true), std::runtime_error);

        graph.addVertex(1);
        ASSERT_THROW(graph.addEdge(1, 2, 3, true), std::runtime_error);
        ASSERT_THROW(graph.addEdge(2, 1, 3, true), std::runtime_error);
    }

    TEST(DerivedGraphTest, TestRemoveEdge) {
        DerivedGraph<int, int> graph(DAG);
        graph.addVertex(1);
        graph.addVertex(2);
        graph.addEdge(1, 2, 3, true);
        graph.removeEdge(1, 2);
        ASSERT_EQ(graph.numEdges(), 0);
    }

    TEST(DerivedGraphTest, RemoveNonexistentEdge) {
        DerivedGraph<int, int> graph(DAG);
        // Modify this according to your error handling implementation.
        ASSERT_THROW(graph.removeEdge(1, 2), std::runtime_error);

        graph.addVertex(1);
        graph.addVertex(2);
        ASSERT_THROW(graph.removeEdge(1, 2), std::runtime_error);
    }

    TEST(DerivedGraphTest, TestRemoveVertex) {
        DerivedGraph<int, int> graph(DAG);
        graph.addVertex(1);
        graph.removeVertex(1);
        ASSERT_EQ(graph.numVertices(), 0);
    }

    TEST(DerivedGraphTest, RemoveNonexistentVertex) {
        DerivedGraph<int, int> graph(DAG);
        // Modify this according to your error handling implementation.
        ASSERT_THROW(graph.removeVertex(1), std::runtime_error);

        graph.addVertex(1);
        graph.removeVertex(1);
        ASSERT_THROW(graph.removeVertex(1), std::runtime_error);
    }

    TEST(DerivedGraphTest, EmptyGraph) {
        DerivedGraph<int, int> graph(DAG);
        ASSERT_EQ(graph.numVertices(), 0);
        ASSERT_EQ(graph.numEdges(), 0);
        ASSERT_THROW(graph.removeVertex(1), std::runtime_error);
        ASSERT_THROW(graph.removeEdge(1, 2), std::runtime_error);
    }

    TEST(DerivedGraphTest, EdgeManagementOnVertexRemoval) {
        DerivedGraph<int, int> graph(DAG);
        graph.addVertex(1);
        graph.addVertex(2);
        graph.addEdge(1, 2, 3, true);
        graph.removeVertex(1);
        ASSERT_EQ(graph.numEdges(), 0);
    }

    TEST(DerivedGraphTest, AddEdgeSameVertex) {
        // If self loop creates a cycle, it shouldn't be allowed
        DerivedGraph<int, int> graph(DAG);
        graph.addVertex(1);
        ASSERT_THROW(graph.addEdge(1, 1, 1, true), std::runtime_error);
    }

    TEST(DerivedGraphTest, VertexAndEdgeCountAfterVertexRemoval) {
        DerivedGraph<int, int> graph(DAG);
        graph.addVertex(1);
        graph.addVertex(2);
        graph.addEdge(1, 2, 3, true);  //<--- Add the edge between vertex 1 and 2
        graph.addVertex(3);
        graph.addEdge(2, 3, 4, true);  //<--- Add the edge between vertex 2 and 3
        graph.removeVertex(1);
        ASSERT_EQ(graph.numVertices(), 2);
        ASSERT_EQ(graph.numEdges(), 1);  //<--- The number of edges should be 1 as one of the edges was removed due to vertex removal.
    }

    TEST(TestDerivedGraph, TestHasEdge) {
        DerivedGraph<int, int> graph(DAG);
        graph.addVertex(1);
        graph.addVertex(2);
        graph.addEdge(1, 2, 3, true);

        // Checks if an edge exists between 1 and 2.
        ASSERT_TRUE(graph.hasEdge(1, 2));

        // Checks if an edge exists between 2 and 1.
        // If your graph is undirected, this should return true.
        // IF your graph is directed, this should return false.
        // Adjust this line as per your graph's context.
        ASSERT_FALSE(graph.hasEdge(2, 1));
    }

    TEST(DerivedGraphTest, EmptyGraphAfterRemovalAllVertices) {
        DerivedGraph<int, int> graph(DAG);
        graph.addVertex(1);
        graph.addVertex(2);
        graph.removeVertex(1);
        graph.removeVertex(2);
        ASSERT_EQ(graph.numEdges(), 0);
        ASSERT_EQ(graph.numVertices(), 0);
    }

    TEST(DerivedGraphTest, TestHasEdgeNonexistentVertices) {
        DerivedGraph<int, int> graph(DAG);
        graph.addVertex(1);
        graph.addVertex(2);
        graph.addEdge(1, 2, 3, true);

        ASSERT_FALSE(graph.hasEdge(1, 3));
        ASSERT_FALSE(graph.hasEdge(3, 1));
        ASSERT_FALSE(graph.hasEdge(3, 3));
    }

    TEST(DerivedGraphTest, AddNegativeWeightEdge) {
        DerivedGraph<int, int> graph(DAG);
        graph.addVertex(1);
        graph.addVertex(2);
        // Modify this according to your error handling implementation.
        ASSERT_NO_THROW(graph.addEdge(1, 2, -3, true));
    }

    TEST(DerivedGraphTest, AddDuplicateVerticesAndEdges) {
        DerivedGraph<int, int> graph(DAG);
        graph.addVertex(1);
        graph.addVertex(2);
        graph.addEdge(1, 2, 3, true);

        // Modify this according to your error handling implementation.
        ASSERT_THROW(graph.addVertex(1), std::runtime_error);
        ASSERT_THROW(graph.addEdge(1, 2, 3, true), std::runtime_error);
    }

    TEST(DerivedGraphTest, HasSelfLoop) {
        DerivedGraph<int, int> graph(DAG);
        graph.addVertex(1);
        EXPECT_THROW(graph.addEdge(1, 1, 1, true), std::runtime_error);
        ASSERT_FALSE(graph.hasEdge(1, 1)); // After attempting to add a self-loop, this should still be false
    }

    TEST(DerivedGraphTest, StringVerticeHandling) {
        DerivedGraph<std::string, int> graph(DAG);
        graph.addVertex("one");
        graph.addVertex("two");
        graph.addEdge("one", "two", 1, true);
        ASSERT_EQ(graph.numVertices(), 2);
        ASSERT_TRUE(graph.hasEdge("one", "two"));
    }

    TEST(DerivedGraphTest, InitializationWithPreexistingData) {
        // Assuming you have this method
        DerivedGraph<int, int> graph = DerivedGraph<int, int>::from_edges({{1, 2, 1}, {2, 3, 2}}, DAG);
        ASSERT_EQ(graph.numVertices(), 3);
        ASSERT_EQ(graph.numEdges(), 2);
    }

    TEST(DerivedGraphTest, LargeDataSet) {
        DerivedGraph<int, int> graph(DAG);
        for(int i=0 ; i<1000000 ; i++)
            graph.addVertex(i);
        ASSERT_EQ(graph.numVertices(), 1000000);
    }

    TEST(DerivedGraphTest, RandomVertexAdditionsAndDeletions) {
        DerivedGraph<int, int> graph(DAG);
        std::unordered_set<int> vertices;

        // Randomly add vertices into the graph.
        srand(time(0));
        for (int i = 0; i < 500; i++) {
            int random_vertex = rand() % 1000;

            // Check if the vertex exists already
            if (vertices.find(random_vertex) == vertices.end()) {
                vertices.insert(random_vertex);
                graph.addVertex(random_vertex);
            }
        }

        ASSERT_EQ(graph.numVertices(), vertices.size());

        // Randomly remove some vertices from the graph.
        for (int i = 0; i < 200 && !vertices.empty(); i++) {
            auto remove_at = vertices.begin();
            std::advance(remove_at, rand() % vertices.size());
            graph.removeVertex(*remove_at);
            vertices.erase(remove_at);
        }

        ASSERT_EQ(graph.numVertices(), vertices.size());
    }

    TEST(DerivedGraphTest, CheckGetVertices) {
        DerivedGraph<int, int> graph(DAG);
        graph.addVertex(1);
        graph.addVertex(2);
        std::vector<int> vertices = graph.getVertices();
        std::sort(vertices.begin(), vertices.end());
        ASSERT_EQ(vertices, (std::vector<int>{1, 2}));
    }

    TEST(DerivedGraphTest, CheckAdjacencyListIterators) {
        DerivedGraph<int, int> graph(DAG);
        graph.addVertex(1);
        graph.addVertex(2);
        graph.addEdge(1, 2, 1, true);
        auto iterStart = graph.adjacentBegin(1);
        auto iterEnd = graph.adjacentEnd(1);
        ASSERT_EQ(iterStart->first, 2);
        ASSERT_EQ(iterStart->second, 1);
        ASSERT_EQ(++iterStart, iterEnd);
    }

    TEST(DerivedGraphTest, RemoveEdgeFromDisconnectedVertex) {
        DerivedGraph<int, int> graph(DAG);
        graph.addVertex(1);
        graph.addVertex(2);
        EXPECT_THROW(graph.removeEdge(1, 2), std::runtime_error);
        ASSERT_EQ(graph.numEdges(), 0);
    }

    TEST(DerivedGraphTest, RemoveNonexistentVertexOrEdge) {
        DerivedGraph<int, int> graph(DAG);
        graph.addVertex(1);
        graph.addVertex(2);
        graph.addEdge(1, 2, 1, true);
        EXPECT_THROW(graph.removeVertex(3), std::runtime_error);
        EXPECT_THROW(graph.removeEdge(1, 3), std::runtime_error);
        ASSERT_EQ(graph.numVertices(), 2);
    }

    TEST(DerivedGraphTest, CheckGraphStateAfterMultipleOperations) {
        DerivedGraph<int, int> graph(DAG);
        graph.addVertex(1);
        graph.addVertex(2);
        graph.addEdge(1, 2, 1, true);
        graph.addVertex(3);
        graph.addEdge(2, 3, 2, true);
        graph.removeEdge(2, 3);
        EXPECT_THROW(graph.removeEdge(2, 3), std::runtime_error);
        std::vector<int> vertices = graph.getVertices();
        std::sort(vertices.begin(), vertices.end());
        ASSERT_EQ(vertices, (std::vector<int>{1, 2, 3}));
        ASSERT_EQ(graph.numEdges(), 1);
    }

    TEST(DerivedGraphTest, StressTest) {
        DerivedGraph<int, int> graph(DAG);
        for(int i = 0; i < 5000; ++i) graph.addVertex(i);
        ASSERT_EQ(graph.numVertices(), 5000);
        for(int i = 0; i < 4999; ++i) {
            graph.addEdge(i, i + 1, i, true);
        }
        ASSERT_EQ(graph.numEdges(), 4999);
        ASSERT_TRUE(graph.hasEdge(0, 1));
        ASSERT_TRUE(graph.hasEdge(4998, 4999));
    }

    // A performance test with a large number of vertices.
    TEST(DerivedGraphTest, PerformanceTestLargeNumberOfVertices) {
        DerivedGraph<int, int> graph(DAG);
        for (int i = 0; i < 1000000; i++) {
            graph.addVertex(i);
        }
        ASSERT_EQ(1000000, graph.numVertices());
    }

// A performance test with a large number of edges.
    TEST(DerivedGraphTest, PerformanceTestLargeNumberOfEdges) {
        DerivedGraph<int, int> graph(DAG);
        for (int i = 0; i < 1000; i++) {
            graph.addVertex(i);
        }
        for (int i = 0; i < 999; i++) {
            graph.addEdge(i, i+1, 1, true);
        }
        ASSERT_EQ(999, graph.numEdges());
    }

// A performance test with a large number of vertices and edges.
    TEST(DerivedGraphTest, PerformanceTestLargeVerticesAndEdges) {
        DerivedGraph<int, int> graph(DAG);
        auto startVertex = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 100000; i++) {
            graph.addVertex(i);
        }
        auto endVertex = std::chrono::high_resolution_clock::now();
        auto durationVertex = std::chrono::duration_cast<std::chrono::milliseconds>(endVertex - startVertex).count();
        std::cout << "Total time for adding vertices: " << durationVertex << " ms" << std::endl;
        auto startEdge = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 99999; i++) {
            graph.addEdge(i, i+1, 1, true);
            if (i % 10000 == 0) {  // Reducing the frequency of console output to every 10000 iterations
                std::cout << "Edge added " << i << "." << std::endl;
            }
        }
        auto endEdge = std::chrono::high_resolution_clock::now();
        auto durationEdge = std::chrono::duration_cast<std::chrono::milliseconds>(endEdge - startEdge).count();
        std::cout << "Total time for adding edges: " << durationEdge << " ms" << std::endl;
        ASSERT_EQ(100000, graph.numVertices());
        ASSERT_EQ(99999, graph.numEdges());
    }


// A performance test with a large number of operations.
    TEST(DerivedGraphTest, PerformanceTestLargeNumberOfOperations) {
        DerivedGraph<int, int> graph(DAG);
        for (int i = 0; i < 50000; i++) {
            graph.addVertex(i);
        }
        for (int i = 0; i < 49999; i++) {
            graph.addEdge(i, i+1, 1, true);
        }
        for (int i = 0; i < 25000; i++) {
            graph.removeVertex(i);
        }
        ASSERT_EQ(25000, graph.numVertices());
        ASSERT_EQ(24999, graph.numEdges());
    }
}