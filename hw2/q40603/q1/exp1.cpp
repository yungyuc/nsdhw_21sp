#include<iostream>
using namespace std;

class PointClass
{
    float m_x, m_y; // by default private.
public:
    // Accessors: get/set style.
    float getX() const { return m_x; }
    float getY() const { return m_y; }
    void setX(float v) { m_x = v; }
    void setY(float v) { m_y = v; }
    // Accessors of alternate style: single method name.
    float const & x() const { return m_x; } // getter
    float       & x()       { return m_x; } // setter
    float const & y() const { return m_y; } // getter
    float       & y()       { return m_y; } // setter
}; /* end class PointClass */

int main(){
    PointClass p;
    p.setX(1.4);
    p.setY(1.6);
    cout<<p.x()<<endl;

    return 0;
}