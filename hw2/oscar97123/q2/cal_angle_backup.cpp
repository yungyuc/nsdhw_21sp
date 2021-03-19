#include <pybind11/pybind11.h>
#include <cmath>

namespace py = pybind11;

float angle(float x1, float x2, float y1, float y2){
	float dot = x1*x2 + y1*y2;      // dot product between [x1, y1] and [x2, y2]
	float det = x1*y2 - y1*x2;      // determinant
	float angle = atan2(det, dot);  // atan2(y, x) or atan2(sin, cos)

	return angle;
}

PYBIND11_MODULE(angle, m) {
    m.def("Cartesian angle", &angle, "the angle (in radians) between two vectors in the 2-dimensional Cartesian coordinate system",
		py::arg("x1"), py::arg("x2"), py::arg("y1"), py::arg("y2"));
}
