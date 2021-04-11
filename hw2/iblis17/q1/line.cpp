#include <iostream>
#include <vector>
#include <array>

class Line
{
public:
    Line(size_t size);
    Line(Line const &);

    ~Line();
    size_t size() const;

    float & x(size_t i) const;
    float & x(size_t i);
    float & y(size_t i) const;
    float & y(size_t i);

private:
    std::vector<std::array<float,2>>* ps;
};

Line::Line(size_t size)
{
    this->ps = new std::vector<std::array<float,2>>;
    this->ps->resize(size, std::array<float,2>());
}

Line::Line(Line const & l)
{
    this->ps = new std::vector<std::array<float,2>>(*(l.ps));
}

size_t Line::size() const { return this->ps->size(); }

Line::~Line() { delete this->ps; }

float & Line::x(size_t i) const { return this->ps->at(i).at(0); }
float & Line::x(size_t i)       { return this->ps->at(i).at(0); }
float & Line::y(size_t i) const { return this->ps->at(i).at(1); }
float & Line::y(size_t i)       { return this->ps->at(i).at(1); }

int main(int, char **)
{

    Line line(3);
    line.x(0) = 0; line.y(0) = 1;
    line.x(1) = 1; line.y(1) = 3;
    line.x(2) = 2; line.y(2) = 5;

    Line line2(line);
    line2.x(0) = 9;

    std::cout << "line: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line.x(it)
                  << " y = " << line.y(it) << std::endl;
    }

    std::cout << "line2: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line2.x(it)
                  << " y = " << line2.y(it) << std::endl;
    }

    return 0;
}
