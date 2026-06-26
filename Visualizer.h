#ifndef VISUALIZER_H
#define VISUALIZER_H

#include "Graph.h"
#include <string>
#include <vector>

// Prints a readable adjacency-list view of the graph, with the option
// to highlight a computed path or MST edge set using arrows/markers
// (since C++ has no built-in plotting library like Python's matplotlib).
void drawGraph(const Graph& graph, const std::string& weightType,
               const std::vector<std::string>& highlightPath = {},
               const std::vector<std::pair<std::string, std::string>>& highlightEdges = {},
               const std::string& title = "City Network");

#endif
