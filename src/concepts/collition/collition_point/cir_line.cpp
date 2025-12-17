#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/line.hpp"

#include <cmath>

Dir2 collition_point (Circle& cir, Line& line) {
  AngDir2 u = AngDir2 {line.get_slope(), -1.f, cir.get_position().a};
  float num = cir.get_position().y - std::fmaf(line.get_slope(), cir.get_position().x, line.get_deviation());
  float denom = std::fmaf(line.get_slope(), line.get_slope(), 1.f);
  return u.madd(num / denom, cir.get_position());
}
