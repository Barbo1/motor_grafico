#include "../../../../headers/pr_objects/line.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/primitives/operations.hpp"

bool test_collition (Square& sq, Line& line) {
  float w = static_cast<float>(sq.width) * sgn(line.slope), 
        h = static_cast<float>(sq.height);
  float coef = line.slope * sq.position.x + line.deviation - sq.position.y;
  coef /= line.slope * w + h;
  return sq._colidalble && absv (coef) < 1;
}
