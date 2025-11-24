#ifndef _POLYGON_H
#define _POLYGON_H
#include <vector>
#include <array>
#include <deque>
#include <string>
#include <fstream>
#include <cassert>
#include <algorithm>
#include "Triangle.h"

class Polygon
{
public:
    // Construct from a CCW-ordered list of vertices.
    explicit Polygon(std::vector<Vertex> vertices_ccw);

    // Load from a csv file
    static Polygon fromCSV(const std::string& csv_file_name);

    // O(n^2) ear clipping.
    std::vector<Triangle> triangulate();

    // Access to original vertices (for tests / visualization).
    const std::vector<Vertex>& vertices() const { return verts_; }

private:
    std::vector<Vertex> verts_;   // vertices of the polygon in counter-clockwise order

	// Data for optimized triangulation
    std::vector<std::size_t> prev_, next_; // previous and next vertex indices for optimized triangulation
    std::vector<bool> is_ear_;
    std::vector<bool> is_removed_;
    std::vector<bool> is_reflex_;

	// Helper functions for the ear-clipping algorithm
	bool isEar(std::size_t i) const;
	bool isReflex(std::size_t i) const;
    void updateVertex(std::size_t i, std::deque<std::size_t>& ear_tip_indices);
};

#endif // !_POLYGON_H

