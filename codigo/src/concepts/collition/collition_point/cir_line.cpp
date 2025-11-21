#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/line.hpp"

Dir2 collition_point (Circle& cir, Line& line) {
  AngDir2 u = AngDir2 {line.get_slope(), -1.f, cir.get_position().a};
  u *= (cir.get_position().y - (line.get_slope() * cir.get_position().x + line.get_deviation())) / 
    (line.get_slope() * line.get_slope() + 1.f);
  return cir.get_position() + u;
}
