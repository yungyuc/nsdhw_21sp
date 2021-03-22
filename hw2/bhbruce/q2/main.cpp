#include <iostream>
#include "_vector.cpp"
using namespace std;
int main() {
    pair<float,float> x(3,0), y(-2,0);
    cout<< angle(x,y) <<endl;
    pair<float,float> x2(0,4), y2(3,0);
    cout<< angle(x2,y2) <<endl;
    pair<float,float> x3(3,4), y3(3,4);
    cout<< angle(x3,y3) <<endl;
}
