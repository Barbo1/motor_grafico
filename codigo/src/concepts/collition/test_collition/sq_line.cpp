#include "../../../../headers/pr_objects/line.hpp"
#include "../../../../headers/pr_objects/square.hpp"

#include <cmath>

bool test_collition (Square& sq, Line& line) {
  Dir2 aux = Dir2 {std::fmaf(line.slope, sq.position.x, line.deviation) - sq.position.y, line.slope}.abs();
  return sq._colidalble && aux.x < std::fmaf(aux.y, sq.width, sq.height);
}
