#include<bits/stdc++.h>
using namespace std;


class Line
{
public:
    Line() {};
    Line(Line const & l1): m_size(l1.m_size), m_x(l1.m_x), m_y(l1.m_y){};
    Line(Line       && l1): m_size(l1.m_size), m_x(std::move(l1.m_x)), m_y(std::move(l1.m_y)){};
    Line & operator=(Line const & l1){
        m_size = l1.m_size;
        m_x.assign(l1.m_x.begin(), l1.m_x.end());
        m_y.assign(l1.m_y.begin(), l1.m_y.end());
    };
    Line & operator=(Line && l1) {
        if (this == &l1) { return *this; }
        m_size = l1.m_size;
        m_x = std::move(l1.m_x);
        m_y = std::move(l1.m_y);
        return *this;
    };
    Line(size_t size): m_size(size),m_x(m_size,0),m_y(m_size,0){};
    ~Line() {};
    size_t size() const {return m_size;};
    float const & x(size_t it) const{return m_x[it];};
    float & x(size_t it) {return m_x[it];};
    float const & y(size_t it) const{return m_y[it];};
    float & y(size_t it) {return m_y[it];};
private:
    size_t m_size;
    vector<float> m_x;
    vector<float> m_y;
    // Member data.
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