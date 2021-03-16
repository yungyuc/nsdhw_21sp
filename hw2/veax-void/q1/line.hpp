#pragma once

#include <iostream>
#include <vector>
using namespace std;

class Line{
public:
    // Constructors
    Line() = default;
    Line(Line const &);
    Line(Line &&);
    Line &operator=(Line const&);
    Line &operator=(Line &&);
    Line(size_t size): coords(size*2, 0) {}
    // Destructor
    ~Line() = default;
    //Fuctions
    size_t size() const {return coords.size()/2;}
    float x(size_t it) const {return coords.at(it*2);}
    float y(size_t it) const {return coords.at(it*2+1);}
    float & x(size_t it) {return coords.at(it*2 );}
    float & y(size_t it) {return coords.at(it*2+1);}

private:
    vector<float> coords;
};
