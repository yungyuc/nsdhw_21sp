#include <iostream>
#include <vector>

class Line {
private:
	struct Coord {
		float x, y;
		Coord(): x(0.0), y(0.0) {}
		Coord(float x, float y): x(x), y(y) {}
	};
	std::vector<Coord> pts;
	
public:
	Line() {}

	Line(Line const &line) {
		pts = line.pts;
	}
	
	Line(Line &&line) {
		pts = std::move(line.pts);
	}

	Line(size_t size) {
		pts.resize(size);
	}
	
	size_t size() const {
		return pts.size();
	}

	float& x(size_t idx) {
		return pts[idx].x;
	}

	float const & x(size_t idx) const {
		return pts[idx].x;
	}

	float& y(size_t idx) {
		return pts[idx].y;
	}

	float const & y(size_t idx) const {
		return pts[idx].y;
	}
};

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
