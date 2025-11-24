# PolygonTriangulator

A clean C++14 implementation of ear-clipping triangulation for simple (including concave) polygons.  

**Note:** This project has not been tested on Linux yet.

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
    ├── Images/                      # Example output images
    └── Input/
        ├── simple_concave_poly.csv
        └── concave_poly.csv
```

---

## Example Output

Here is an example triangulation for a complex concave polygon (64 triangles):

![Triangulation Example](https://raw.githubusercontent.com/vthiagar/PolygonTriangulator/main/PolygonTriangulator/Images/concave_poly_64.png)

---

## Build Instructions

### **Linux / Ubuntu 22.04+**

Prerequisites:

```bash
sudo apt update
sudo apt install g++ make python3 python3-pip
pip3 install matplotlib
```

Build:

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
cd bin
PolygonTriangulator
```

---

### **Windows (Visual Studio)**

1. Open:

   ```
   PolygonTriangulator.sln
   ```

2. Build using Visual Studio 2022 or later.

3. After building, the executable will appear in:

```
x64/Debug/PolygonTriangulator.exe
x64/Release/PolygonTriangulator.exe
```

4. Additionally, this project is configured to copy the executable into:
```
PolygonTriangulator/bin/PolygonTriangulator.exe
```
This bin/ path is intentionally defined in the project file and will always be created on every build.
You may run the program from either location.

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

### **Run with visualization (requires Python + matplotlib)**

```bash
./PolygonTriangulator PolygonTriangulator/Input/simple_concave_poly.csv plot
```

This will:

1. Generate `triangles.csv`
2. Automatically run:

   ```bash
   python3 visualize_triangulation.py <input> triangles.csv
   ```

3. Launch a Matplotlib window showing:
   - Polygon outline (red)
   - Triangulation edges (blue)
   - Filled triangles (transparent blue)

---

## Running Unit Tests

Run:

```bash
./PolygonTriangulator --test
```

This executes internal validation (triangle area consistency, convex/concave vertex identification, ear clipping checks).

---

## Command Line Help

```bash
./PolygonTriangulator --help
```

Displays:

```
Usage:
  PolygonTriangulator [input_csv] [plot]
  PolygonTriangulator --test

Options:
  input_csv   Path to a CSV file containing polygon vertices (x,y pairs).
              If omitted, a default sample file will be used.

  plot        Optional. If provided as the second argument,
              the program invokes Python visualization.

  --test      Runs internal unit tests and exits.
```

---

## License

MIT License. Free to use, modify, and integrate into other geometry / simulation workflows.
