#pragma once

#include <vector>
#include <map>
#include <stdexcept>

// Template Graph Class
template <typename T>
class Graph {
private:
    std::map<T, std::vector<T>> adjacencyList;

public:
    void addNode(const T& node) {
        adjacencyList[node];
    }

    void addEdge(const T& node1, const T& node2) {
        adjacencyList[node1].push_back(node2);
        adjacencyList[node2].push_back(node1);
    }

    const std::vector<T>&getNeighbors(const T& node) const {
        auto it = adjacencyList.find(node);
        if (it == adjacencyList.end()) {
            throw std::invalid_argument("Node not found in the graph");
        }
        return it->second;
    }

    bool hasNode(const T& node) const {
        return adjacencyList.find(node) != adjacencyList.end();
    }
};
