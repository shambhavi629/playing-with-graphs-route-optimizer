#include "Algorithms.h"
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <limits>
#include <chrono>

using Clock = std::chrono::high_resolution_clock;

static double weightOf(const Edge& e, const std::string& weightType) {
    if (weightType == "distance") return e.distance;
    if (weightType == "cost") return e.cost;
    return e.time; // "time"
}

static double weightOf(const Graph::SimpleEdge& e, const std::string& weightType) {
    if (weightType == "distance") return e.distance;
    if (weightType == "cost") return e.cost;
    return e.time;
}

PathResult dijkstra(const Graph& graph, const std::string& start,
                     const std::string& end, const std::string& weightType) {
    auto t0 = Clock::now();
    PathResult result;

    std::unordered_map<std::string, double> dist;
    std::unordered_map<std::string, std::string> prev;
    for (const auto& c : graph.cities) dist[c] = std::numeric_limits<double>::infinity();
    dist[start] = 0.0;

    // min-heap of (distance, city) -- priority_queue is max-heap by default,
    // so we use greater<> to flip it into a min-heap, same role as Python's heapq.
    using PQItem = std::pair<double, std::string>;
    std::priority_queue<PQItem, std::vector<PQItem>, std::greater<PQItem>> pq;
    pq.push({0.0, start});

    std::unordered_set<std::string> visited;
    int nodesVisited = 0;

    while (!pq.empty()) {
        auto top = pq.top();
        double d = top.first;
        std::string u = top.second;
        pq.pop();
        if (visited.count(u)) continue;
        visited.insert(u);
        nodesVisited++;

        if (u == end) break;

        for (const auto& e : graph.neighbors(u)) {
            if (visited.count(e.to)) continue;
            double w = weightOf(e, weightType);
            double newDist = d + w;
            if (newDist < dist[e.to]) {
                dist[e.to] = newDist;
                prev[e.to] = u;
                pq.push({newDist, e.to});
            }
        }
    }

    auto t1 = Clock::now();
    result.stats.executionTimeMs = std::chrono::duration<double, std::milli>(t1 - t0).count();
    result.stats.nodesVisited = nodesVisited;

    if (dist[end] == std::numeric_limits<double>::infinity()) {
        result.found = false;
        return result;
    }

    // Reconstruct path by walking backwards through `prev`
    std::vector<std::string> path;
    std::string node = end;
    while (true) {
        path.push_back(node);
        if (node == start) break;
        auto it = prev.find(node);
        if (it == prev.end()) break; // safety guard
        node = it->second;
    }
    std::reverse(path.begin(), path.end());

    result.path = path;
    result.weight = dist[end];
    result.found = true;
    return result;
}

PathResult fewestStops(const Graph& graph, const std::string& start,
                        const std::string& end) {
    auto t0 = Clock::now();
    PathResult result;

    std::unordered_set<std::string> visited{start};
    std::unordered_map<std::string, std::string> prev;
    std::queue<std::string> q;
    q.push(start);
    int nodesVisited = 0;

    while (!q.empty()) {
        std::string u = q.front();
        q.pop();
        nodesVisited++;
        if (u == end) break;
        for (const auto& e : graph.neighbors(u)) {
            if (!visited.count(e.to)) {
                visited.insert(e.to);
                prev[e.to] = u;
                q.push(e.to);
            }
        }
    }

    auto t1 = Clock::now();
    result.stats.executionTimeMs = std::chrono::duration<double, std::milli>(t1 - t0).count();
    result.stats.nodesVisited = nodesVisited;

    if (!visited.count(end)) {
        result.found = false;
        return result;
    }

    std::vector<std::string> path;
    std::string node = end;
    while (true) {
        path.push_back(node);
        if (node == start) break;
        node = prev[node];
    }
    std::reverse(path.begin(), path.end());

    result.path = path;
    result.weight = static_cast<double>(path.size() - 1); // number of hops
    result.found = true;
    return result;
}

// Union-Find (Disjoint Set Union) -- tracks which cities are already
// connected, so Kruskal's algorithm can detect and skip cycles.
class UnionFind {
public:
    std::unordered_map<std::string, std::string> parent;

    explicit UnionFind(const std::unordered_set<std::string>& items) {
        for (const auto& item : items) parent[item] = item;
    }

    std::string find(std::string item) {
        while (parent[item] != item) item = parent[item];
        return item;
    }

    bool unite(const std::string& a, const std::string& b) {
        std::string ra = find(a), rb = find(b);
        if (ra == rb) return false; // already connected -> would form a cycle
        parent[ra] = rb;
        return true;
    }
};

MSTResult kruskalMST(const Graph& graph, const std::string& weightType) {
    auto t0 = Clock::now();
    MSTResult result;

    auto edges = graph.allEdges();
    std::sort(edges.begin(), edges.end(),
              [&](const Graph::SimpleEdge& a, const Graph::SimpleEdge& b) {
                  return weightOf(a, weightType) < weightOf(b, weightType);
              });

    UnionFind uf(graph.cities);
    int nodesVisited = 0;
    double totalWeight = 0.0;

    for (const auto& e : edges) {
        nodesVisited++;
        if (uf.unite(e.u, e.v)) {
            result.edges.push_back(e);
            totalWeight += weightOf(e, weightType);
        }
    }

    auto t1 = Clock::now();
    result.stats.executionTimeMs = std::chrono::duration<double, std::milli>(t1 - t0).count();
    result.stats.nodesVisited = nodesVisited;
    result.totalWeight = totalWeight;
    return result;
}
