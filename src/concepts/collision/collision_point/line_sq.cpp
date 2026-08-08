#include "../../../../headers/concepts/collision.hpp"
#include <cmath>

Dir2 collision_point (Line& line, Square& sq) {
  Dir2 sq_pos = sq.position;
  float h = sq.dims.y * std::copysignf(1.f, line.get_slope()), 
        a = (std::fmaf(line.get_slope(), sq_pos.x(), line.get_deviation()) - sq_pos.y()) / 
            std::fmaf(line.get_slope(), sq.dims.x, h);
  Dir2 t = Dir2 (line.get_slope(), -1.f).normalize();
  return t.madd(t * (Dir2 (sq.dims.x, -h) * (std::copysignf(1.f,a) - a)), sq_pos);
}
