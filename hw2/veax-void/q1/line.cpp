#include "line.hpp"
#include <algorithm>
#include <cstdint>
#include <utility>

Line::Line(Line const & other): coords(other.coords.size()*2, 0){
    // Copy constructor
    std::copy(other.coords.begin(), other.coords.end(), coords.begin());
}

Line::Line(Line && other): coords(other.coords.size()*2, 0){
    // Move constructor
    other.coords.swap(coords);
}

Line & Line::operator=(Line const & other){
    // Copy assignment operator
    if (this == &other) { return *this; } // don't move to self.
    // Part from the copy constructor
    coords.resize(other.coords.size()*2, 0);
    std::copy(other.coords.begin(), other.coords.end(), coords.begin());
    return *this;
}

Line & Line::operator=(Line && other){
    // Move assignment operator
    // Check if trying move to itself
    if (this == &other){ 
        return *this; 
    }
    coords.resize(other.coords.size()*2, 0);
    other.coords.swap(coords);
    return *this;
}
