#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "Graph.h"
#include <string>
#include <vector>

struct Stats {
    int nodesVisited = 0;
    double executionTimeMs = 0.0;
};

struct PathResult {
    std::vector<std::string> path; // empty if no path found
    double weight = 0.0;
    Stats stats;
    bool found = false;
};

struct MSTResult {
    std::vector<Graph::SimpleEdge> edges;
    double totalWeight = 0.0;
    Stats stats;
};

// Generalized Dijkstra: weightType is "distance", "cost", or "time".
// Same algorithm in all three cases -- only which number is minimized changes.
PathResult dijkstra(const Graph& graph, const std::string& start,
                     const std::string& end, const std::string& weightType);

// BFS: finds path with fewest hops (edges), ignoring all weights.
// Genuinely different algorithm from Dijkstra -- correct only because
// we're minimizing hop COUNT, not a weighted quantity.
PathResult fewestStops(const Graph& graph, const std::string& start,
                        const std::string& end);

// Kruskal's algorithm: builds Minimum Spanning Tree, optimizable by
// distance, cost, or time.
MSTResult kruskalMST(const Graph& graph, const std::string& weightType);

#endif
