#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

float cal_dot(std::vector<float>& v1,
			  std::vector<float>& v2) {
	return v1[0]*v2[0] + v1[1]*v2[1];
}

float cal_len(std::vector<float>& v) {
	return std::sqrt(v[0]*v[0] + v[1]*v[1]);
}

bool is_same(const float a, const float b) {
	return std::fabs(a-b) < 0.0001;
}

float cal_angle(std::vector<float>& v1, std::vector<float>& v2) {
	auto len1 = cal_len(v1);
	auto len2 = cal_len(v2);
	if (is_same(len1, 0.0) || is_same(len2, 0.0)) {
		throw std::length_error("Length cannot be zero.");
	}

	auto dot = cal_dot(v1, v2);
	auto res = dot / (len1 * len2);
	if (is_same(res, 1.0)) {
		res = 1;
	}

	return std::acos(res);
}

PYBIND11_MODULE(_vector, m) {
    m.doc() = "hw2 q2"; // optional module docstring

    m.def("cal_angle", &cal_angle, "Calculate the angle between two vectors.");
}
