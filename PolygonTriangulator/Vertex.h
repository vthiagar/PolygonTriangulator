#ifndef _VERTEX_H
#define _VERTEX_H

// A simple structure representing a 2D vertex with an optional identifier.
struct Vertex
{
    double x{};
    double y{};
    int    id{};

    Vertex(double x_, double y_, int id_ = -1) : x(x_), y(y_), id(id_) {}
};
#endif // !_VERTEX_H