#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/line.hpp"
#include "../../../../headers/primitives/operations.hpp"

bool test_collition (Circle & cir, Line& line) {
  float p1 = absv (cir.position.y - line.slope * cir.position.x - line.deviation);
  float p2 = line.slope * line.slope + 1;
  return cir._colidalble && p1 * p1 * p2 < cir.radio * cir.radio;
}
