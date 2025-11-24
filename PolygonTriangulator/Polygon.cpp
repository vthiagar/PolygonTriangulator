#include "Polygon.h"


Polygon::Polygon(std::vector<Vertex> vertices_ccw) : verts_(std::move(vertices_ccw)), 
													 prev_(verts_.size()), next_(verts_.size()), 
	                                                 is_ear_(verts_.size(), false), 
	                                                 is_removed_(verts_.size(), false), 
	                                                 is_reflex_(verts_.size(), false)
{
	// NOTE: Ear clipping requires a simple polygon with >= 3 vertices.
    // Degenerate polygons are not supported.
	assert(verts_.size() >= 3);

	// Ensure CCW orientation
	if (Geometry::signedArea(verts_) < 0.0) {
		std::reverse(verts_.begin(), verts_.end());
	}
}

// Load from a csv file	
Polygon Polygon::fromCSV(const std::string &csv_file_name)
{
	std::vector<Vertex> verts;
	std::string line;
	int id = 0;

	std::ifstream in(csv_file_name);
	if (!in) {
		throw std::runtime_error("Could not open file: " + csv_file_name);
	}

	while (std::getline(in, line))
	{
		if (line.empty()) continue;   // skip blank lines

		size_t commaPos = line.find(',');
		if (commaPos != std::string::npos)
		{
			double x = std::stod(line.substr(0, commaPos));
			double y = std::stod(line.substr(commaPos + 1));
			verts.emplace_back(x, y, id++);
		}
	}

	auto eq = [](double a, double b) { return std::fabs(a - b) < 1e-12; };

	if (!verts.empty()) {
		const Vertex& first = verts.front();
		const Vertex& last = verts.back();

		if (eq(first.x, last.x) && eq(first.y, last.y)) {
			verts.pop_back();
		}
	}

	return Polygon(std::move(verts));
}

// O(n^2) ear clipping.
std::vector<Triangle> Polygon::triangulate()
{
	const auto n = verts_.size();


	// Output triangle list (indices into verts_)
	std::vector<Triangle> triangles;
	triangles.reserve(n - 2);
	
	// Queue of ear tip indices
	std::deque<std::size_t> ear_tip_indices;

	// initialize circular list
	for (std::size_t i = 0; i < n; ++i)
	{
		prev_[i] = (i == 0) ? (n - 1) : (i - 1);
		next_[i] = (i + 1) % n;
		is_removed_[i] = false;
	}

	auto remaining = n;

	// Initialize reflex status
	for (std::size_t i = 0; i < n; ++i) {
		is_reflex_[i] = isReflex(i);
	}

	// Initialize ear status
	for (std::size_t i = 0; i < n; ++i) {
		if (!is_reflex_[i] && isEar(i)) {
			is_ear_[i] = true;
			ear_tip_indices.push_back(i);
		}
		else {
			is_ear_[i] = false;
		}
	}

	// Main loop
	while (remaining > 3) {

		int ear = -1;

		// Find an ear
		while (ear_tip_indices.size() > 0) {
			auto candidate = ear_tip_indices.front();
			ear_tip_indices.pop_front();
			if (is_ear_[candidate] && !is_removed_[candidate]) {
				ear = static_cast<int>(candidate);
				break;
			}
		}	

		if (ear == -1) {
			throw std::runtime_error("No ear found. Polygon may be malformed.");
		}

		// Clip ear
		auto a = prev_[ear];
		auto b = ear;
		auto c = next_[ear];
		// Add triangle
		triangles.emplace_back(verts_[a], verts_[b], verts_[c]);
		// Mark b as removed
		is_removed_[b] = true;
		// Update links
		next_[a] = c;
		prev_[c] = a;
		remaining--;
		// Update a and c
		updateVertex(a, ear_tip_indices);
		updateVertex(c, ear_tip_indices);
	}

	// Add final triangle
	int i0 = -1;
	for (std::size_t i = 0; i < n; ++i) {
		if (!is_removed_[i]) { i0 = i; break; }
	}
	auto i1 = next_[i0];
	auto i2 = next_[i1];

	triangles.emplace_back(verts_[i0], verts_[i1], verts_[i2]);
		
	return triangles;
}

bool Polygon::isEar(std::size_t i) const
{
	if (is_removed_[i]) {
		return false;
	}
	if (is_reflex_[i]) {
		return false;
	}	
	auto &a = verts_[prev_[i]];
	auto &b = verts_[i];
	auto &c = verts_[next_[i]];

	for (std::size_t j = 0; j < verts_.size(); ++j) {
	
		if (is_removed_[j] || // Skip removed vertices
			!is_reflex_[j] || // Skip convex vertices
			j == prev_[i] || j == i || j == next_[i]) { // Skip vertices of the current ear 
			continue;
		}

		if (Geometry::pointInTriangle(verts_[j], a, b, c)) {
			return false;
		}
	}

	return true;
}

bool Polygon::isReflex(std::size_t i) const
{
	const auto &a = verts_[prev_[i]];
	const auto &b = verts_[i];
	const auto &c = verts_[next_[i]];

	if (Geometry::cross(a, b, c) <= 0.0) { // Included angle >= pi => reflex vertex
		return true;
	}

	return false; // Included angle < pi => convex vertex
}

void Polygon::updateVertex(std::size_t i, std::deque<std::size_t> &ear_tip_indices) {
	
	if (is_removed_[i]) {
		return;
	}

	bool newReflex = isReflex(i);
	is_reflex_[i] = newReflex;

	bool newIsEar = false;
	if (!newReflex) {
		newIsEar = isEar(i);
	}

	if (newIsEar && !is_ear_[i]) {
		is_ear_[i] = true;
		ear_tip_indices.push_back(i);
	}
	else if (!newIsEar && is_ear_[i]) {
		is_ear_[i] = false;
	}
}