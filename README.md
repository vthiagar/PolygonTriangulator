# PolygonTriangulator

A clean C++14 implementation of ear-clipping triangulation for simple (including concave) polygons.  
The program:

- Loads polygon vertices from a CSV file.
- Triangulates using an optimized O(n²) ear-clipping implementation.
- Computes the cumulative area of triangulated polygons.
- Optionally visualizes triangulation using Python + Matplotlib.
- Includes a lightweight built-in unit test suite (no external dependencies).
- Builds on both **Windows (Visual Studio)** and **Linux (Ubuntu 22.04+)**.

---

## Directory Layout

```
PolygonTriangulator/
├── PolygonTriangulator.sln          # Visual Studio Solution
├── Makefile                         # Linux/macOS build script
├── README.md
└── PolygonTriangulator/
    ├── PolygonTriangulator.cpp      # main() + CLI
    ├── Polygon.cpp / Polygon.h      # Polygon class with optimized ear clipping
    ├── Vertex.cpp  / Vertex.h       # Vertex representation
    ├── Triangle.cpp / Triangle.h    # Triangle representation + area()
    ├── Geometry.h                   # Header-only geometric utilities
    ├── Tests.cpp                    # Minimal built-in unit tests
    ├── visualize_triangulation.py   # Optional Python visualization
    └── Input/
        ├── simple_concave_poly.csv
        └── concave_poly.csv
```

---

## Example Output

Here is an example triangulation for a complex concave polygon (64 triangles):

![Triangulation Example](PolygonTriangulator/images/complex_triangulation.png)


## Build Instructions

### **Linux / Ubuntu 22.04+**

Prerequisites:

```bash
sudo apt update
sudo apt install g++ make python3 python3-pip
pip3 install matplotlib
```

Build using:

```bash
cd PolygonTriangulator
make
```

Executable will be generated at:

```
bin/PolygonTriangulator
```

Run:

```bash
./bin/PolygonTriangulator
```

---

### **Windows (Visual Studio)**

1. Open:

   PolygonTriangulator.sln

2. Build using Visual Studio 2022 or later.

3. After building, the executable will appear in:

      x64/Debug/PolygonTriangulator.exe  
      x64/Release/PolygonTriangulator.exe

   (Visual Studio may additionally create a copy under PolygonTriangulator/bin/,
    but the official output locations are the x64 build folders.)
---

## Usage

### **Run with default built-in example**

```bash
./PolygonTriangulator
```

### **Run with a specific CSV input**

```bash
./PolygonTriangulator PolygonTriangulator/Input/simple_concave_poly.csv
```

### **Run with visualization (requires matplotlib)**

```bash
./PolygonTriangulator PolygonTriangulator/Input/simple_concave_poly.csv plot
```

This will:

1. Generate `triangles.csv`
2. Automatically run:  
   `python3 visualize_triangulation.py <input> triangles.csv`
3. Launch a Matplotlib window showing:
   - Polygon outline (red)
   - Triangulation (blue)
   - Filled triangles (blue transparent)

---
