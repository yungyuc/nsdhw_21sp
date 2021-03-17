#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <stdexcept>
#include <cmath>

namespace py = pybind11;

float cartesian_angle(std::array<float, 2> vec1, std::array<float, 2> vec2)
{
    float s_len1 = vec1[0] * vec1[0] + vec1[1] * vec1[1];
    float s_len2 = vec2[0] * vec2[0] + vec2[1] * vec2[1];
    if (s_len1 == 0 || s_len2 == 0) 
        throw std::invalid_argument("Vector length must be grater then zero!");

    float dot_product = vec1[0] * vec2[0] + vec1[1] * vec2[1];
    float a = dot_product / sqrt(s_len1 * s_len2);

    if (a >= 1.0)
        return 0.0;
    else if (a <= -1.0)
        return M_PI;
    else
        return acos(a);
}

PYBIND11_MODULE(package, m) {
    m.def("cartesian_angle", &cartesian_angle, "Calculates the angle (in radians) between two vectors in the 2-dimensional Cartesian coordinate system.");
} 