#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/line.hpp"

#include <cmath>

Dir2 collition_point (const Line& line, const Circle& cir) {
  Dir2 u = Dir2 (line.get_slope(), -1.f);
  float num = cir.position.y - std::fmaf(line.get_slope(), cir.position.x, line.get_deviation());
  float denom = std::fmaf(line.get_slope(), line.get_slope(), 1.f);
  return u.madd(num / denom, cir.position);
}
