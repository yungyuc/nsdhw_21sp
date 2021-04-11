#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <math.h>

double getAngle(std::vector<double> a, std::vector<double> b) {
    return acos(a[0] * b[0] + a[1] * b[1]);
}

PYBIND11_MODULE(_vector, m) {
    m.def("getAngle", &getAngle, "A function which calculate radians between two vector");
}