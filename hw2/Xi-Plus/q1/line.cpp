#include "line.hpp"

#include <cstdlib>

Line::Line(Line const& line2) {
	points.resize(line2.size());
	std::copy(line2.points.begin(), line2.points.end(), points.begin());
}

Line::Line(Line&& line2) {
	points.resize(line2.size());
	points = std::move(line2.points);
}

Line& Line::operator=(Line const& line2) {
	new (this) Line(line2);
	return *this;
}

Line& Line::operator=(Line&& line2) {
	new (this) Line(std::move(line2));
	return *this;
}

Line::Line(size_t size) {
	points.resize(size);
}

size_t Line::size() const {
	return points.size();
}

float Line::x(size_t it) const {
	return points[it].first;
};

float& Line::x(size_t it) {
	return points[it].first;
};

float Line::y(size_t it) const {
	return points[it].second;
};

float& Line::y(size_t it) {
	return points[it].second;
};
