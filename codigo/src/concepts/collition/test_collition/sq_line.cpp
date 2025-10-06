#include "../../../../headers/pr_objects/line.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/concepts/operations.hpp"

bool test_collition (Square& sq, Line& line) {
  float w = static_cast<float>(sq.width), h = static_cast<float>(sq.height) * sgn(line.slope);
  float coef = (line.slope * sq.position.x + line.deviation - sq.position.y);
  coef /= (line.slope * w + h);
  absv (&coef);
  return sq._colidalble && coef < 1;
}
