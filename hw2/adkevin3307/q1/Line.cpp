#include "Line.h"

using namespace std;

Line::Line()
{
    this->m_points.resize(0);
}

Line::Line(size_t size)
{
    this->m_points.resize(size);
}

Line::Line(Line const& rhs)
{
    this->m_points = rhs.m_points;
}

Line::Line(Line&& rhs)
{
    this->m_points = rhs.m_points;
}

Line& Line::operator=(Line const& rhs)
{
    this->m_points = rhs.m_points;

    return (*this);
}

Line& Line::operator=(Line&& rhs)
{
    this->m_points = rhs.m_points;

    return (*this);
}

Line::~Line()
{
    this->m_points.clear();
    this->m_points.shrink_to_fit();
}

size_t Line::size() const
{
    return this->m_points.size();
}

float const& Line::x(size_t it) const
{
    return this->m_points[it].first;
}

float& Line::x(size_t it)
{
    return this->m_points[it].first;
}

float const& Line::y(size_t it) const
{
    return this->m_points[it].second;
}

float& Line::y(size_t it)
{
    return this->m_points[it].second;
}
