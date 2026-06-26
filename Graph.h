#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

// A single road/route between two cities, storing THREE independent
// weights so the same graph can answer different questions:
//   - shortest distance path
//   - minimum cost path
//   - least time path
struct Edge {
    std::string to;
    double distance;
    double cost;
    double time;
};

class Graph {
public:
    std::unordered_set<std::string> cities;
    std::unordered_map<std::string, std::vector<Edge>> adj;

    bool addCity(const std::string& name);
    bool addRoute(const std::string& a, const std::string& b,
                  double distance, double cost, double time,
                  bool bidirectional = true);
    bool removeRoute(const std::string& a, const std::string& b);
    bool updateWeight(const std::string& a, const std::string& b,
                       const std::string& attr, double newValue);

    const std::vector<Edge>& neighbors(const std::string& city) const;

    // Returns unique edges (u, v, distance, cost, time) -- avoids
    // double-counting bidirectional roads, used for MST and display.
    struct SimpleEdge {
        std::string u, v;
        double distance, cost, time;
    };
    std::vector<SimpleEdge> allEdges() const;

    bool isEmpty() const { return cities.empty(); }
};

#endif
