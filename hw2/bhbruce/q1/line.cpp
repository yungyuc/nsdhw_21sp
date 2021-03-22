#include "line.hpp"


Line::Line() {
    this->m_x.resize(0);
    this->m_y.resize(0);
}

Line::Line(Line const & rhs) {
    this->m_x = rhs.m_x;
    this->m_y = rhs.m_y;
}

Line::Line(Line && rhs) {

    this->m_x = rhs.m_x;
    this->m_y = rhs.m_y;
}

Line & Line::operator=(Line const & rhs) {
    // copy assignment operator
    // this is a class pointer
    this->m_x = rhs.m_x;
    this->m_y = rhs.m_y;
    return (*this);
}

Line & Line::operator=(Line && rhs) {
    // move assignment operator
    // return this->Line(rhs);
    this->m_x = rhs.m_x;
    this->m_y = rhs.m_y;
    return (*this);
}

Line::Line(size_t size) {
    this->m_x.resize(size);
    this->m_y.resize(size);
}

Line::~Line() {
    this->m_x.clear();
    this->m_x.shrink_to_fit();
    this->m_y.clear();
    this->m_y.shrink_to_fit();
}


size_t Line::size() const{
    return this->m_x.size();
}

float & Line::x(size_t it) {
    return this->m_x.at(it);
}

float & Line::y(size_t it) {
    return this->m_y.at(it);
}

float const & Line::x(size_t it) const {
    return this->m_x.at(it);
}

float const & Line::y(size_t it) const {
    return this->m_y.at(it);
}
