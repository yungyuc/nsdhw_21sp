#include <iostream>
#include <cmath>
#include <cfenv>
#include <utility>
#include <limits>
#include <stdexcept>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

using namespace std;

float angle(const pair<float, float> v1, const pair<float, float> v2)
{
    float v1_norm = sqrt(v1.first * v1.first + v1.second * v1.second);
    float v2_norm = sqrt(v2.first * v2.first + v2.second * v2.second);

    if (v1_norm < numeric_limits<float>::epsilon() || v2_norm < numeric_limits<float>::epsilon()) {
        throw runtime_error("zero-length vector");
    }

    if (fabs(v1.first - v2.first) < numeric_limits<float>::epsilon() && fabs(v1.second - v2.second) < numeric_limits<float>::epsilon()) {
        return 0.0f;
    }

    float dot_value = v1.first * v2.first + v1.second * v2.second;

    float result = acos(dot_value / (v1_norm * v2_norm));

    return result;
}

PYBIND11_MODULE(_vector, m)
{
    m.def("angle", &angle, "Calcuate angle between two vector in Cartesian coordinate system.");
}
