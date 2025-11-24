# Simple Makefile for PolygonTriangulator
# Target environment: Ubuntu 22.04+ with g++ and make installed.

CXX      := g++
CXXFLAGS := -std=c++14 -Wall -Wextra -pedantic -O2
LDFLAGS  :=

# Project structure
SRCDIR   := PolygonTriangulator
INCLUDES := -I$(SRCDIR)

BINDIR   := bin
TARGET   := $(BINDIR)/PolygonTriangulator

# Source files
SOURCES  := \
    $(SRCDIR)/PolygonTriangulator.cpp \
    $(SRCDIR)/Polygon.cpp \
    $(SRCDIR)/Vertex.cpp \
    $(SRCDIR)/Triangle.cpp \
    $(SRCDIR)/Tests.cpp
OBJECTS  := $(SOURCES:.cpp=.o)

.PHONY: all clean test run

# Default build
all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

# Compile each .cpp into a .o
$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Convenience target: run tests
test: $(TARGET)
	./$(TARGET) --test

# Convenience target: run with default input
run: $(TARGET)
	./$(TARGET)

# Clean build artifacts
clean:
	rm -f $(SRCDIR)/*.o
	rm -f $(TARGET)
