#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <cmath>
#include "Vertex.h"

// A utility class for basic geometric computations.
class Geometry {
public:

	// Compute the cross product of vectors AB and AC.
    static inline double cross(const Vertex& a, const Vertex& b, const Vertex& c) {
        return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    }

	//  Compute the (unsigned) area of the triangle formed by a, b, c.
    static inline double triangleArea(const Vertex& a, const Vertex& b, const Vertex& c) {
        return std::fabs(cross(a, b, c)) * 0.5;
    }

	// Compute the signed area of the triangle formed by a, b, c.
    static inline double triangleAreaSigned(const Vertex& a, const Vertex& b, const Vertex& c) {
        return 0.5 * cross(a, b, c);
    }

	// Check if point p is inside the triangle formed by a, b, c.
    static inline bool pointInTriangle(const Vertex& p, Vertex a, Vertex b, Vertex c) {
        if (cross(a, b, c) < 0.0) {
            std::swap(b, c);
        }

        double c1 = cross(a, b, p);
        double c2 = cross(b, c, p);
        double c3 = cross(c, a, p);
        const double eps = -1e-12;
        return (c1 >= eps && c2 >= eps && c3 >= eps);
    }

	// Compute the signed area of a polygon given its vertices in order.
    static inline double signedArea(const std::vector<Vertex>& v) {
        double A = 0.0;
        const std::size_t n = v.size();
        for (std::size_t i = 0; i < n; ++i) {
            const auto& p = v[i];
            const auto& q = v[(i + 1) % n];
            A += p.x * q.y - q.x * p.y;
        }
        return 0.5 * A;
    }
};

#endif