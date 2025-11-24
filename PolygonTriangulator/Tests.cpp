#include <iostream>
#include <cmath>
#include <initializer_list>
#include <utility>
#include <vector>

#include "Polygon.h"
#include "Geometry.h"

// ---------- Helper Functions ---------- //

static bool AlmostEqual(double a, double b, double eps = 1e-9) {
    return std::fabs(a - b) <= eps;
}

// Build a Polygon from an initializer list of (x,y) pairs.
static Polygon MakePolygon(std::initializer_list<std::pair<double, double>> pts) {
    std::vector<Vertex> verts;
    verts.reserve(pts.size());
    int id = 0;
    for (const auto& p : pts) {
        verts.emplace_back(p.first, p.second, id++);
    }
    return Polygon(std::move(verts));
}

// Stats for area comparison: polygon vs triangulation
struct AreaStats {
    double polyArea;
    double triArea;
    double absErr;
    double relErr;
    bool   ok;
};

// Compute areas and errors.
static AreaStats AnalyzeArea(const Polygon& poly) {
    Polygon p = poly;  // non-const copy for triangulation
    auto tris = p.triangulate();

    double triArea = 0.0;
    for (const auto& t : tris) {
        triArea += t.area();
    }

    double polyArea = std::fabs(Geometry::signedArea(poly.vertices()));
    double absErr = std::fabs(polyArea - triArea);
    double relErr = (polyArea != 0.0) ? absErr / polyArea : absErr;
    bool ok = AlmostEqual(polyArea, triArea);

    AreaStats stats;
    stats.polyArea = polyArea;
    stats.triArea = triArea;
    stats.absErr = absErr;
    stats.relErr = relErr;
    stats.ok = ok;
    return stats;
}

// ---------- Test Runner ---------- //

void RunUnitTests() {
    int passed = 0, failed = 0;

    auto RUN = [&](const char* name, const Polygon& p) {
        std::cout << "\n=== Test: " << name << " ===\n";

        // Compute area and errors
        AreaStats stats = AnalyzeArea(p);

        // Triangle count check
        Polygon tmp = p;
        auto tris = tmp.triangulate();
        std::size_t n = p.vertices().size();
        bool countOK = (n >= 3 && tris.size() == n - 2);

        // Detailed output
        std::cout << "Vertices        : " << n << "\n";
        std::cout << "Triangles       : " << tris.size()
                  << "  (expected " << (n - 2) << ")\n";
        std::cout << "Polygon Area    : " << stats.polyArea << "\n";
        std::cout << "Triang Area     : " << stats.triArea << "\n";
        std::cout << "Abs Error       : " << stats.absErr << "\n";
        std::cout << "Rel Error       : " << stats.relErr << "\n";

        bool ok = stats.ok && countOK;

        std::cout << "Result          : " << (ok ? "PASS" : "FAIL") << "\n";
        ok ? ++passed : ++failed;
    };

    // base path you’re already using
    std::string input_file_path = "../PolygonTriangulator/Input/";

    // ---- Tests ----

    RUN("Triangle",
        MakePolygon({
            {0.0, 0.0},
            {1.0, 0.0},
            {0.5, 1.0}
            })
    );

    RUN("Square",
        Polygon({
            {0,0,0}, {1,0,1}, {1,1,2}, {0,1,3}
            })
    );

    RUN("Simple Concave Inline",
        MakePolygon({
            {0.0, 0.0},
            {0.0, 1.0},
            {0.5, 0.5},
            {1.0, 1.0},
            {0.8, 0.0},
            {1.0, 0.0}
            })
    );

    RUN("Simple Concave Inline (Reversed)",
        [&] {
            Polygon tmp = MakePolygon({
                {0.0, 0.0},
                {0.0, 1.0},
                {0.5, 0.5},
                {1.0, 1.0},
                {0.8, 0.0},
                {1.0, 0.0}
                });
            std::vector<Vertex> verts = tmp.vertices();
            std::reverse(verts.begin(), verts.end());
            return Polygon(std::move(verts));
        }()
            );

    RUN("Small Concave CSV",
        Polygon::fromCSV(input_file_path + "simple_concave_poly.csv")
    );

    RUN("Large Concave CSV",
        Polygon::fromCSV(input_file_path + "concave_poly.csv")
    );

    RUN("Almost Collinear",
        MakePolygon({
            {0.0, 0.0},
            {1.0, 0.001},
            {2.0, 0.0},
            {1.0, 1.0}
            })
    );

    RUN("Flat Edge",
        MakePolygon({
            {0,0},
            {1,0},
            {2,0},   // flat segment
            {2,1},
            {0,1}
            })
    );

    // ----- Summary -----
    std::cout << "\n---------------------------------\n";
    std::cout << "Tests Passed = " << passed
        << ", Failed = " << failed << "\n";
    std::cout << "---------------------------------\n";
}
