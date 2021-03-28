#include <pybind11/pybind11.h>
#include <cmath>

namespace py = pybind11;

double angle(double x1, double y1, double x2, double y2) {
    if (x1 == 0 && y1 == 0)
        throw std::invalid_argument("invalid input: zero length vector");
    if (x2 == 0 && y2 == 0)
        throw std::invalid_argument("invalid input: zero length vector");

    return abs(atan2(x1*y2 - y1*x2, x1*x2 + y1*y2));
}

PYBIND11_MODULE(_vector, m) {
    m.def("angle", &angle, "...");
}
