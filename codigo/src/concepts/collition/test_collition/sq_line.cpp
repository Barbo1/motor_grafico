#include "../../../../headers/pr_objects/line.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/primitives/operations.hpp"

bool test_collition (Square& sq, Line& line) {
  float coef_n = line.slope * sq.position.x + line.deviation - sq.position.y;
  absv (&coef_n);
  return sq._colidalble && coef_n < absv(line.slope) * sq.width + sq.height;
}
