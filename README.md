# Playing with Graphs — Route Optimization App (C++)

A menu-driven C++ application that models a city network as a graph
and finds optimal routes using classic graph algorithms.

This is the C++ version of the project. (A Python version with full
visual graph rendering also exists, using the same algorithms.)

## Features

- **Dijkstra's Algorithm** — generalized to optimize for:
  - Shortest distance
  - Minimum cost
  - Least time
  (Same algorithm, three different edge weights — see `Algorithms.cpp`)
- **BFS ("Fewest Stops")** — finds the path with the fewest hops, ignoring weights.
  This is intentionally a *different* algorithm from Dijkstra, since BFS only
  gives correct shortest-hop-count results on unweighted problems.
- **Kruskal's Algorithm** — builds a Minimum Spanning Tree (cheapest network
  that connects every city with no cycles), using Union-Find to detect cycles.
  Optimizable by distance, cost, or time.
- **Dynamic graph updates** — add/remove routes and change weights at runtime,
  then re-run any algorithm on the updated graph.
- **Text-based graph visualization** — prints the city network to the console
  with the chosen path or MST clearly marked (`===>` for highlighted edges).
- **Algorithm comparison** — reports nodes visited and execution time
  (via `<chrono>`) for each run, and lets you compare all four routing
  strategies side-by-side on the same source/destination.

## Build

Requires a C++14-compatible compiler (g++ or clang++; even older MinGW versions work).

```bash
make
```

Or manually:
```bash
g++ -std=c++14 -O2 main.cpp Graph.cpp Algorithms.cpp Visualizer.cpp -o graph_app
```

## Run

```bash
./graph_app
```
or
```bash
make run
```

Choose option **12** first to load demo data (6 Indian cities, 8 routes),
then explore options 5–11 to try the algorithms.

## Project Structure

```
Graph.h / Graph.cpp           - Graph data structure (adjacency list, cities, routes)
Algorithms.h / Algorithms.cpp - Dijkstra, BFS, Kruskal implementations + perf stats
Visualizer.h / Visualizer.cpp - Console-based graph rendering with path highlighting
main.cpp                      - Menu-driven CLI tying everything together
Makefile                      - Build configuration
```

## Why Dijkstra here (and not Bellman-Ford)?

All edge weights (distance, cost, time) are non-negative, which is exactly
the condition Dijkstra requires for correctness. If the graph had negative
weights (e.g., a route that *earns* money), Dijkstra could return an
incorrect "shortest" path, and Bellman-Ford would be required instead.

## Example

```
Route: Delhi -> Lucknow -> Varanasi
Total distance (km): 370
Nodes visited: 3 | Execution time: 0.0067 ms
```

## Tech Used

C++17, Data Structures & Algorithms, Graphs, Priority Queue
(`std::priority_queue`), BFS (`std::queue`), Dijkstra's Algorithm,
Kruskal's Algorithm, Union-Find (Disjoint Set Union), STL containers
(`unordered_map`, `unordered_set`, `vector`)
