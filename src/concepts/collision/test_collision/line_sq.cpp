#include "../../../../headers/pr_objects/line.hpp"
#include "../../../../headers/pr_objects/square.hpp"

#include <cmath>

bool test_collision (const Line& line, const Square& sq) {
  Dir2 u = Dir2 (-1.f, std::copysign(1.f, line.v.x * line.v.y)).dir_mul(Dir2(sq.dims));
  return std::abs(Dir2(line.v).pLd(Dir2(line.p) - Dir2(sq.position), u)) < 1.f;
}
