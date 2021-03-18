#include <cstdlib>
#include <vector>

class Line {
   public:
	Line() = default;
	Line(Line const &);
	Line(Line &&);
	Line &operator=(Line const &);
	Line &operator=(Line &&);
	Line(size_t size);
	~Line() = default;
	size_t size() const;
	float x(size_t it) const;
	float &x(size_t it);
	float y(size_t it) const;
	float &y(size_t it);

   private:
	std::vector<std::pair<float, float>> points;
}; /* end class Line */
