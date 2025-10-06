#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/concepts/operations.hpp"

bool test_collition (Circle& cir, Square& sq) {
  Dir2 diff = (cir.position - sq.position).abs();
  diff.x = max0 (diff.x - sq.width);
  diff.y = max0 (diff.y - sq.height);
  return cir._colidalble && sq._colidalble && diff.modulo2() < cir.radio * cir.radio;
}
