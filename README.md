# Playing with Graphs — Route Optimization App (C++)
A menu-driven C++ application that models a city network as a graph and finds routes between cities using classic graph algorithms. 
Built this project to understand how graph algorithms work beyond coding problems and see how they can be used in a simple route optimization system.

## Screenshots

<p align="center">
  <img src="Screenshot%202026-06-26%20205923.png" alt="Main Menu" width="850">
</p>

<p align="center">
  <img src="Screenshot%202026-06-26%20205947.png" alt="Loading Sample Data" width="850">
</p>

<p align="center">
  <img src="Screenshot%202026-06-26%20210142.png" alt="Algorithm Comparison" width="850">
</p>

## Features
- **Dijkstra's Algorithm** — used to find shortest distance, minimum cost, and least time paths (same algorithm, just swapping which weight it minimizes)
- **BFS** — finds the path with fewest stops, ignoring distance/cost/time.
- **Kruskal's Algorithm** — builds a Minimum Spanning Tree using Union-Find, optimizable by distance, cost, or time.
- Supports adding, removing, and updating routes at runtime without restarting the application.
- **Text-based graph visualization** — prints the network to the console with the selected path or MST highlighted.
- Displays execution time and nodes visited for each algorithm to help compare different routing strategies.


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

Choose option **12** first to load demo data (6 Indian cities, 8 routes), then explore options 5–11 to try the algorithms.

## Project Structure

```text
Graph.h / Graph.cpp
    Graph representation using adjacency lists

Algorithms.h / Algorithms.cpp
    Dijkstra, BFS, Kruskal and Union-Find implementations

Visualizer.h / Visualizer.cpp
    Console visualization and path highlighting

main.cpp
    Menu-driven application

Makefile
    Build configuration
```

## Example

```text
Route: Delhi -> Lucknow -> Varanasi
Total distance (km): 370
Nodes visited: 3 | Execution time: 0.0067 ms
```


## Tech Used

C++14, STL, Data Structures & Algorithms, Graphs, Priority Queue, BFS, Dijkstra's Algorithm, Kruskal's Algorithm, Union-Find
