/*
 * main.cpp
 * --------
 * "Playing with Graphs" -- a menu-driven route optimization app (C++ version).
 *
 * Demonstrates:
 *   - Dijkstra's algorithm (shortest distance / minimum cost / least time)
 *   - BFS (fewest stops)
 *   - Kruskal's algorithm (Minimum Spanning Tree)
 *   - Dynamic graph updates (add/remove routes, change weights)
 *   - Text-based graph visualization with highlighted paths
 *   - Basic algorithm performance comparison (nodes visited, execution time)
 *
 * Build:  g++ -std=c++14 -O2 main.cpp Graph.cpp Algorithms.cpp Visualizer.cpp -o graph_app
 * Run:    ./graph_app
 */

#include "Graph.h"
#include "Algorithms.h"
#include "Visualizer.h"
#include <iostream>
#include <limits>

void printMenu() {
    std::cout << "\n==================================================\n"
               << "        PLAYING WITH GRAPHS - ROUTE OPTIMIZER\n"
               << "==================================================\n"
               << " 1. Add City\n"
               << " 2. Add Route\n"
               << " 3. Remove Route\n"
               << " 4. Update Route Weight\n"
               << " 5. Find Shortest Distance Path   (Dijkstra)\n"
               << " 6. Find Minimum Cost Path        (Dijkstra)\n"
               << " 7. Find Least Time Path          (Dijkstra)\n"
               << " 8. Find Fewest Stops Path        (BFS)\n"
               << " 9. Generate Minimum Spanning Tree (Kruskal)\n"
               << "10. Compare All Path Algorithms on Same Route\n"
               << "11. Display Graph (text view)\n"
               << "12. Load Sample Data (demo)\n"
               << " 0. Exit\n"
               << "==================================================\n";
}

void loadSampleData(Graph& graph) {
    for (const auto& c : {"Delhi", "Lucknow", "Varanasi", "Patna", "Kolkata", "Kanpur"}) {
        graph.addCity(c);
    }
    struct Route { std::string a, b; double d, c, t; };
    std::vector<Route> routes = {
        {"Delhi", "Lucknow", 500, 600, 8},
        {"Delhi", "Kanpur", 470, 550, 7},
        {"Kanpur", "Lucknow", 80, 120, 1.5},
        {"Lucknow", "Varanasi", 320, 400, 5},
        {"Kanpur", "Varanasi", 330, 420, 5.5},
        {"Varanasi", "Patna", 250, 300, 4},
        {"Patna", "Kolkata", 580, 650, 9},
        {"Varanasi", "Kolkata", 680, 750, 10.5},
    };
    for (const auto& r : routes) graph.addRoute(r.a, r.b, r.d, r.c, r.t);
    std::cout << "\n  Sample data loaded: 6 cities, 8 routes.\n";
}

bool promptTwoCities(const Graph& graph, std::string& a, std::string& b) {
    std::cout << "  Enter starting city: ";
    std::getline(std::cin, a);
    std::cout << "  Enter destination city: ";
    std::getline(std::cin, b);
    if (!graph.cities.count(a) || !graph.cities.count(b)) {
        std::cout << "  One or both cities not found in the graph.\n";
        return false;
    }
    return true;
}

void showPathResult(const std::string& label, const PathResult& r, const std::string& unit) {
    if (!r.found) {
        std::cout << "  No path found.\n";
        return;
    }
    std::cout << "\n  [" << label << "]\n  Route: ";
    for (size_t i = 0; i < r.path.size(); i++) {
        std::cout << r.path[i];
        if (i + 1 < r.path.size()) std::cout << " -> ";
    }
    std::cout << "\n  Total " << unit << ": " << r.weight << "\n";
    std::cout << "  Nodes visited: " << r.stats.nodesVisited
               << " | Execution time: " << r.stats.executionTimeMs << " ms\n";
}

int main() {
    Graph graph;
    printMenu();
    std::cout << "Tip: choose option 12 first to load demo data and explore quickly.\n\n";

    std::string choice;
    while (true) {
        std::cout << "Enter your choice: ";
        if (!std::getline(std::cin, choice)) break;

        if (choice == "1") {
            std::cout << "  City name: ";
            std::string name; std::getline(std::cin, name);
            graph.addCity(name);

        } else if (choice == "2") {
            std::string a, b; double d, c, t;
            std::cout << "  City A: "; std::getline(std::cin, a);
            std::cout << "  City B: "; std::getline(std::cin, b);
            std::cout << "  Distance (km): "; std::cin >> d;
            std::cout << "  Cost (Rs): "; std::cin >> c;
            std::cout << "  Time (hr): "; std::cin >> t;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            graph.addRoute(a, b, d, c, t);

        } else if (choice == "3") {
            std::string a, b;
            std::cout << "  City A: "; std::getline(std::cin, a);
            std::cout << "  City B: "; std::getline(std::cin, b);
            graph.removeRoute(a, b);

        } else if (choice == "4") {
            std::string a, b, attr; double val;
            std::cout << "  City A: "; std::getline(std::cin, a);
            std::cout << "  City B: "; std::getline(std::cin, b);
            std::cout << "  Attribute to update (distance/cost/time): "; std::getline(std::cin, attr);
            std::cout << "  New value: "; std::cin >> val;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            graph.updateWeight(a, b, attr, val);

        } else if (choice == "5") {
            std::string a, b;
            if (promptTwoCities(graph, a, b)) {
                auto r = dijkstra(graph, a, b, "distance");
                showPathResult("Shortest Distance (Dijkstra)", r, "distance (km)");
            }

        } else if (choice == "6") {
            std::string a, b;
            if (promptTwoCities(graph, a, b)) {
                auto r = dijkstra(graph, a, b, "cost");
                showPathResult("Minimum Cost (Dijkstra)", r, "cost (Rs)");
            }

        } else if (choice == "7") {
            std::string a, b;
            if (promptTwoCities(graph, a, b)) {
                auto r = dijkstra(graph, a, b, "time");
                showPathResult("Least Time (Dijkstra)", r, "time (hr)");
            }

        } else if (choice == "8") {
            std::string a, b;
            if (promptTwoCities(graph, a, b)) {
                auto r = fewestStops(graph, a, b);
                showPathResult("Fewest Stops (BFS)", r, "stops");
            }

        } else if (choice == "9") {
            std::cout << "  Optimize MST for (distance/cost/time): ";
            std::string attr; std::getline(std::cin, attr);
            if (attr.empty()) attr = "cost";
            auto mst = kruskalMST(graph, attr);
            if (mst.edges.empty()) {
                std::cout << "  Graph is empty or has no edges.\n";
            } else {
                std::cout << "\n  [Minimum Spanning Tree by " << attr << "]\n";
                for (const auto& e : mst.edges) {
                    double w = (attr == "distance") ? e.distance : (attr == "cost") ? e.cost : e.time;
                    std::cout << "    " << e.u << " -- " << e.v << "  (" << attr << "=" << w << ")\n";
                }
                std::cout << "  Total " << attr << ": " << mst.totalWeight << "\n";
                std::cout << "  Edges examined: " << mst.stats.nodesVisited
                           << " | Execution time: " << mst.stats.executionTimeMs << " ms\n";

                std::cout << "  Show this MST on the graph? (y/n): ";
                std::string show; std::getline(std::cin, show);
                if (show == "y" || show == "Y") {
                    std::vector<std::pair<std::string, std::string>> edgeList;
                    for (const auto& e : mst.edges) edgeList.push_back({e.u, e.v});
                    drawGraph(graph, attr, {}, edgeList, "Minimum Spanning Tree (by " + attr + ")");
                }
            }

        } else if (choice == "10") {
            std::string a, b;
            if (promptTwoCities(graph, a, b)) {
                std::cout << "\n  Comparing algorithms for the SAME source/destination:\n";
                showPathResult("Shortest Distance", dijkstra(graph, a, b, "distance"), "km");
                showPathResult("Minimum Cost", dijkstra(graph, a, b, "cost"), "Rs");
                showPathResult("Least Time", dijkstra(graph, a, b, "time"), "hr");
                showPathResult("Fewest Stops", fewestStops(graph, a, b), "stops");
                std::cout << "\n  Note: Dijkstra is appropriate here because all weights "
                           << "(distance/cost/time) are non-negative. If a graph had "
                           << "negative weights, Dijkstra could give wrong answers and "
                           << "Bellman-Ford would be needed instead.\n";
            }

        } else if (choice == "11") {
            std::cout << "  Show graph weighted by (distance/cost/time) [default distance]: ";
            std::string wt; std::getline(std::cin, wt);
            if (wt.empty()) wt = "distance";
            drawGraph(graph, wt, {}, {}, "City Network (weights = " + wt + ")");

        } else if (choice == "12") {
            loadSampleData(graph);

        } else if (choice == "0") {
            std::cout << "  Exiting. Goodbye!\n";
            break;

        } else {
            std::cout << "  Invalid choice, try again.\n";
        }

        printMenu();
    }

    return 0;
}
