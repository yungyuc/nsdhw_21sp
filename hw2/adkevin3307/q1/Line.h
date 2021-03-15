#pragma once

#include <cstddef>
#include <vector>
#include <iomanip>

using namespace std;

class Line {
private:
    int m_size;
    vector<pair<float, float>> m_points;

public:
    Line();
    Line(size_t size);
    Line(Line const& rhs);
    Line(Line&& rhs);
    Line& operator=(Line const& rhs);
    Line& operator=(Line&& rhs);
    ~Line();

    size_t size() const;
    float const& x(size_t it) const;
    float& x(size_t it);
    float const& y(size_t it) const;
    float& y(size_t it);
};
