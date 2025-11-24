#ifndef  _TRI_H
#define _TRI_H
#include "Geometry.h"
#include <cmath>

// A simple structure representing a triangle defined by three vertices.
struct Triangle
{
    Vertex a, b, c;

    Triangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
        : a(v0), b(v1), c(v2) {}

	// Compute the (unsigned) area of the triangle.
    double area() const
    {
		return Geometry::triangleArea(a, b, c);
    }
};
#endif // ! _TRI_H

