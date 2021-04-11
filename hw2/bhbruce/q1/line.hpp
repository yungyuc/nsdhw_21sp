# pragma once
#include <cstddef>
#include <vector>
using namespace std;


class Line
{
public:
    Line(); // default constructor
    Line(Line const & ); // copy con.
    Line(Line       &&); // move con.
    Line & operator=(Line const & ); // copy assignment operator
    Line & operator=(Line       &&); // move assignment operator
    Line(size_t size); // con.
    ~Line(); // destructor
    size_t size() const; // getter
    float const & x(size_t it) const; // getter
    float & x(size_t it); // setter
    float const & y(size_t it) const; // getter
    float & y(size_t it); // setter
private:
    vector<float> m_x, m_y;
    // Member data.
}; /* end class Line */
