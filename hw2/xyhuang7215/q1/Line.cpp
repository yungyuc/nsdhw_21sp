#include <vector>
#include <iostream>

class Line
{
public:
    Line();
    Line(Line const & );
    Line(Line       &&);
    Line & operator=(Line const & );
    Line & operator=(Line       &&);
    Line(size_t size);
    ~Line();
    size_t size() const;
    float const & x(size_t it) const;
    float & x(size_t it);
    float const & y(size_t it) const;
    float & y(size_t it);
private:
    std::vector<float> xs;
    std::vector<float> ys;
}; /* end class Line */

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

Line::Line(){
    xs.resize(0);
    ys.resize(0);
};

Line::Line(Line const & L){
    xs.assign(L.xs.begin(), L.xs.end());
    ys.assign(L.ys.begin(), L.ys.end());
}

Line::Line(Line       &&L){
    xs.assign(L.xs.begin(), L.xs.end());
    ys.assign(L.ys.begin(), L.ys.end());
};

Line & Line::operator=(Line const & L){
    xs.assign(L.xs.begin(), L.xs.end());
    ys.assign(L.ys.begin(), L.ys.end());
    return *this;
};

Line & Line::operator=(Line       &&L){
    xs.assign(L.xs.begin(), L.xs.end());
    ys.assign(L.ys.begin(), L.ys.end());
    return *this;
};

Line::Line(size_t size){
    xs.resize(size);
    ys.resize(size);
};

Line::~Line(){
    xs.clear();
    ys.clear();
};

size_t Line::size() const{
    return xs.size();
};

float const & Line::x(size_t it) const{
    return xs[it];
};

float & Line::x(size_t it){
    return xs[it];
};

float const & Line::y(size_t it) const{
    return ys[it];
};

float & Line::y(size_t it){
    return ys[it];
};