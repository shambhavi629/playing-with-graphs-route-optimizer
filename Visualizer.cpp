#include "Visualizer.h"
#include <iostream>
#include <set>
#include <map>
#include <algorithm>

static double weightOf(const Graph::SimpleEdge& e, const std::string& weightType) {
    if (weightType == "distance") return e.distance;
    if (weightType == "cost") return e.cost;
    return e.time;
}

void drawGraph(const Graph& graph, const std::string& weightType,
               const std::vector<std::string>& highlightPath,
               const std::vector<std::pair<std::string, std::string>>& highlightEdges,
               const std::string& title) {
    if (graph.isEmpty()) {
        std::cout << "  Graph is empty -- add some cities first.\n";
        return;
    }

    // Build a quick-lookup set of edges to highlight, from either
    // a path (consecutive city pairs) or an explicit edge list (e.g. MST)
    std::set<std::pair<std::string, std::string>> highlightSet;
    auto addPair = [&](const std::string& a, const std::string& b) {
        highlightSet.insert({std::min(a, b), std::max(a, b)});
    };
    for (size_t i = 0; i + 1 < highlightPath.size(); i++) {
        addPair(highlightPath[i], highlightPath[i + 1]);
    }
    for (const auto& edgePair : highlightEdges) addPair(edgePair.first, edgePair.second);

    std::set<std::string> pathCities(highlightPath.begin(), highlightPath.end());

    std::cout << "\n  ==================== " << title << " ====================\n";
    std::cout << "  (weights shown = " << weightType << ")\n\n";

    // Group edges by city for a clean adjacency-style printout
    std::map<std::string, std::vector<Graph::SimpleEdge>> byCity;
    for (const auto& e : graph.allEdges()) {
        byCity[e.u].push_back(e);
        Graph::SimpleEdge rev{e.v, e.u, e.distance, e.cost, e.time};
        byCity[e.v].push_back(rev);
    }

    // Print cities in sorted order for stable, readable output
    std::vector<std::string> sortedCities(graph.cities.begin(), graph.cities.end());
    std::sort(sortedCities.begin(), sortedCities.end());

    for (const auto& city : sortedCities) {
        std::string marker = pathCities.count(city) ? "[*]" : "[ ]";
        std::cout << "  " << marker << " " << city << "\n";

        auto it = byCity.find(city);
        if (it == byCity.end() || it->second.empty()) {
            std::cout << "        (no routes)\n";
            continue;
        }
        for (const auto& e : it->second) {
            bool isHighlighted = highlightSet.count({std::min(city, e.v), std::max(city, e.v)});
            std::string arrow = isHighlighted ? "===>" : "---->";
            std::cout << "        " << arrow << " " << e.v
                       << "  (" << weightType << "=" << weightOf(e, weightType) << ")"
                       << (isHighlighted ? "   <-- on chosen route" : "") << "\n";
        }
    }

    if (!highlightPath.empty()) {
        std::cout << "\n  Highlighted route: ";
        for (size_t i = 0; i < highlightPath.size(); i++) {
            std::cout << highlightPath[i];
            if (i + 1 < highlightPath.size()) std::cout << " -> ";
        }
        std::cout << "\n";
    }
    std::cout << "  ===========================================================\n\n";
}
