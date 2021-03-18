// Ref: https://pybind11.readthedocs.io/en/stable/basics.html#creating-bindings-for-a-simple-function

#include <pybind11/pybind11.h>

inline float vec_len(std::pair<float, float> v) {
	return sqrt(pow(v.first, 2) + pow(v.second, 2));
}

float vector_angle(std::pair<float, float> v1, std::pair<float, float> v2) {
	if (v1.first == 0 && v1.second == 0) {
		throw pybind11::value_error("The length of first vector is 0");
	}
	if (v2.first == 0 && v2.second == 0) {
		throw pybind11::value_error("The length of second vector is 0");
	}

	float dot = (v1.first * v2.first + v1.second * v2.second) / vec_len(v1) / vec_len(v2);
	if (dot < -1) {
		dot = -1;
	} else if (dot > 1) {
		dot = 1;
	}

	return acos(dot);
}

// Ref: https://github.com/pybind/pybind11/issues/1408#issuecomment-391121386
PYBIND11_MODULE(_vector, m) {
	m.doc() = "_vector";
	m.def("vector_angle", &vector_angle, "Calculates the angle between two vectors");
}
