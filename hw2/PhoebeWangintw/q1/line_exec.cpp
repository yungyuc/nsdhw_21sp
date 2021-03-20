#include <iostream>
#include <vector>
#include <cmath>

class Line {
private:
	struct Coord {
		float x, y;
		Coord(): x(0.0), y(0.0) {}
		Coord(float x, float y): x(x), y(y) {}
	};
	std::vector<Coord> pts;
	
	float dot_product(size_t idx1, size_t idx2) {
		return (pts[idx1].x * pts[idx2].x) + 
			   (pts[idx1].y * pts[idx2].y);
	}

	float cal_length(size_t idx) {
		const auto x_square = std::pow(pts[idx].x, 2);
		const auto y_square = std::pow(pts[idx].y, 2);

		return std::sqrt(x_square + y_square);
	}

	float cal_angle(size_t idx1, size_t idx2) {
		auto res_product = dot_product(idx1, idx2);
		auto len1 = cal_length(idx1);
		auto len2 = cal_length(idx2);
		
		return res_product / (len1 * len2);
	}

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
