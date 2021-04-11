#include<vector>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <pybind11/pybind11.h>
using namespace std;


float angle(pair<float,float> x, pair<float,float> y) {
    if((fabs(x.first) < std::numeric_limits<float>::epsilon() &&
        fabs(x.second) < std::numeric_limits<float>::epsilon()) ||
       (fabs(y.first) < std::numeric_limits<float>::epsilon() &&
        fabs(y.second) < std::numeric_limits<float>::epsilon())
      ) {
        throw std::invalid_argument("Received zero vector");
    }
    float ele_product = x.first * y.first + x.second * y.second;
    float dist_x = sqrt(x.first * x.first + x.second * x.second);
    float dist_y = sqrt(y.first * y.first + y.second * y.second);
    if (dist_x < std::numeric_limits<float>::epsilon() ||
        dist_y < std::numeric_limits<float>::epsilon())
        throw std::runtime_error("Dividing zero");
    return acos(ele_product / (dist_x * dist_y));
}


PYBIND11_MODULE(_vector, m) {
        m.def("angle", &angle, "The angle (in radians) between two vectors");
}
