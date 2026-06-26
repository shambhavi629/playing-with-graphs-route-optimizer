#include "Graph.h"
#include <iostream>
#include <algorithm>

static const std::vector<Edge> EMPTY_EDGES;

bool Graph::addCity(const std::string& name) {
    if (cities.count(name)) {
        std::cout << "  City '" << name << "' already exists.\n";
        return false;
    }
    cities.insert(name);
    adj[name]; // ensure key exists even with no edges yet
    std::cout << "  City '" << name << "' added.\n";
    return true;
}

bool Graph::addRoute(const std::string& a, const std::string& b,
                      double distance, double cost, double time,
                      bool bidirectional) {
    if (!cities.count(a) || !cities.count(b)) {
        std::cout << "  Both cities must exist before adding a route. Add them first.\n";
        return false;
    }
    adj[a].push_back({b, distance, cost, time});
    if (bidirectional) {
        adj[b].push_back({a, distance, cost, time});
    }
    std::cout << "  Route added: " << a << " <-> " << b
               << " (distance=" << distance << ", cost=" << cost
               << ", time=" << time << ")\n";
    return true;
}

bool Graph::removeRoute(const std::string& a, const std::string& b) {
    size_t before = adj[a].size();
    auto removeFrom = [&](const std::string& from, const std::string& target) {
        auto& vec = adj[from];
        vec.erase(std::remove_if(vec.begin(), vec.end(),
                  [&](const Edge& e) { return e.to == target; }), vec.end());
    };
    removeFrom(a, b);
    removeFrom(b, a);
    size_t after = adj[a].size();
    if (before == after) {
        std::cout << "  No route found between " << a << " and " << b << ".\n";
        return false;
    }
    std::cout << "  Route removed: " << a << " <-> " << b << "\n";
    return true;
}

bool Graph::updateWeight(const std::string& a, const std::string& b,
                          const std::string& attr, double newValue) {
    bool updated = false;
    auto apply = [&](const std::string& from, const std::string& to) {
        for (auto& e : adj[from]) {
            if (e.to == to) {
                if (attr == "distance") e.distance = newValue;
                else if (attr == "cost") e.cost = newValue;
                else if (attr == "time") e.time = newValue;
                updated = true;
            }
        }
    };
    if (attr != "distance" && attr != "cost" && attr != "time") {
        std::cout << "  Invalid attribute. Use 'distance', 'cost', or 'time'.\n";
        return false;
    }
    apply(a, b);
    apply(b, a);
    if (updated) {
        std::cout << "  Updated " << attr << " between " << a << " and " << b
                   << " to " << newValue << ".\n";
    } else {
        std::cout << "  No route found between " << a << " and " << b << ".\n";
    }
    return updated;
}

const std::vector<Edge>& Graph::neighbors(const std::string& city) const {
    auto it = adj.find(city);
    if (it == adj.end()) return EMPTY_EDGES;
    return it->second;
}

std::vector<Graph::SimpleEdge> Graph::allEdges() const {
    std::unordered_set<std::string> seen;
    std::vector<SimpleEdge> result;
    for (const auto& entry : adj) {
        const std::string& u = entry.first;
        const std::vector<Edge>& edges = entry.second;
        for (const auto& e : edges) {
            // Build a canonical key so each undirected edge is only counted once
            std::string key = (u < e.to) ? (u + "|" + e.to) : (e.to + "|" + u);
            if (seen.count(key)) continue;
            seen.insert(key);
            result.push_back({u, e.to, e.distance, e.cost, e.time});
        }
    }
    return result;
}
