#include <vector>
#include <iostream>

 class Line
 {
 public:
     Line() = default;
     Line(Line const &lines): sizeT(lines.sizeT), x_pt(lines.x_pt), y_pt(lines.y_pt){}
     Line(Line       &&lines): sizeT(lines.sizeT), x_pt(std::move(lines.x_pt)), y_pt(std::move(lines.y_pt)){}
     Line & operator=(Line const & lines){
         sizeT = lines.sizeT;
         x_pt.assign(lines.x_pt.begin(), lines.x_pt.end());
         y_pt.assign(lines.y_pt.begin(), lines.y_pt.end());
         return *this;
     };
     
     Line & operator=(Line &&lines) {
         
         if (this == &lines) {
             return *this;
         }
         
         sizeT = lines.sizeT;
         x_pt = std::move(lines.x_pt);
         y_pt = std::move(lines.y_pt);
         return *this;
     };
     Line(size_t size): sizeT(size),x_pt(sizeT,0),y_pt(sizeT,0){}
     
     ~Line() = default; //De-constructor
     
     size_t size() const {
         return sizeT;
         
     }
     
     float const &x(size_t it) const {
         return x_pt[it];
     }
     
     float &x(size_t it) {
         return x_pt[it];
     }
     
     float const &y(size_t it) const {
         return y_pt[it];
     }
     
     float &y(size_t it) {
         return y_pt[it];
     }
     
 private:
     size_t sizeT;
     std::vector<float> x_pt;
     std::vector<float> y_pt;
 };
