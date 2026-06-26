CXX = g++
CXXFLAGS = -std=c++14 -Wall -O2

SOURCES = main.cpp Graph.cpp Algorithms.cpp Visualizer.cpp
TARGET = graph_app

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
