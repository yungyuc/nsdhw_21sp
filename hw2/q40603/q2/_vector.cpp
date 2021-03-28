#include <pybind11/pybind11.h>
#include <math.h>

namespace py = pybind11;

bool cmpf(float A, float B, float epsilon = 0.005f)
{
    return (fabs(A - B) < epsilon);
}


float _angle(std::pair<float, float> v1, std::pair<float, float> v2){

    if (cmpf(v1.first, 0.0f) && cmpf(v1.second, 0.0f))
        throw std::invalid_argument("1st vector length : 0");
    if (cmpf(v2.first, 0.0f) && cmpf(v2.second, 0.0f))
        throw std::invalid_argument("2nd vector length : 0");
    
    float l_v1 = sqrt( pow(v1.first, 2) + pow(v1.second, 2) );
    float l_v2 = sqrt( pow(v2.first, 2) + pow(v2.second, 2) );
    float cos_theta = (v1.first * v2.first + v1.second * v2.second) / (l_v1 * l_v2);
    
    return acos(cos_theta);
}


PYBIND11_MODULE(_vector, m) {
    m.doc() = "pybind11 cal theta between two vectors"; // optional module docstring
    m.def("_angle", &_angle, "Calculates the angle (in radians) between two vectors in the 2-dimensional Cartesian coordinate system.");
}