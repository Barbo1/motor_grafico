#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/line.hpp"

#include <cmath>

bool test_collition (Circle & cir, Line& line) {
  float p1 = cir.position.y - std::fmaf(line.slope, cir.position.x, line.deviation);
  float p2 = std::fmaf(line.slope, line.slope, 1.f);
  return cir._colidalble && p1 * p1 < cir.radio * cir.radio * p2;
}
