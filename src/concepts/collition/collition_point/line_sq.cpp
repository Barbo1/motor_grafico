#include "../../../../headers/pr_objects/line.hpp"
#include "../../../../headers/pr_objects/square.hpp"

#include <cmath>

Dir2 collition_point (const Line& line, const Square& sq) {
  float h = sq.height * std::copysignf(1.f, line.get_slope()), 
        a = (std::fmaf(line.get_slope(), sq.position.x, line.get_deviation()) - sq.position.y) / 
            std::fmaf(line.get_slope(), sq.width, h);
  AngDir2 t = AngDir2 {line.get_slope(), -1.f, 0}.normalize();
  return sq.position + t * (t * (AngDir2 {sq.width, -h, 0} * (std::copysignf(1.f,a) - a)));
}
