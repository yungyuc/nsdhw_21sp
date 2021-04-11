#include "line.hpp"
using namespace std;

int main(int, char **)
{
    Line line(3);
    line.x(0) = 0; line.y(0) = 1;
    line.x(1) = 1; line.y(1) = 3;
    line.x(2) = 2; line.y(2) = 5;

    Line line2(line);
    line2.x(0) = 9;

    cout << "line: number of points = " << line.size() << endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        cout << "point " << it << ":"
             << " x = " << line.x(it)
             << " y = " << line.y(it) << endl;
    }

    cout << "line2: number of points = " << line.size() << endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        cout << "point " << it << ":"
             << " x = " << line2.x(it)
             << " y = " << line2.y(it) << endl;
    }

    return 0;
}