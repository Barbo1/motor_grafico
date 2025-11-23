#include "../../../../headers/pr_objects/line.hpp"
#include "../../../../headers/pr_objects/square.hpp"

bool test_collition (Square& sq, Line& line) {
  Dir2 aux = Dir2 {line.slope * sq.position.x + line.deviation - sq.position.y, line.slope}.abs();
  return sq._colidalble && aux.x < aux.y * sq.width + sq.height;
}
