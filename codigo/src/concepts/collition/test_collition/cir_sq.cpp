#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"

bool test_collition (Circle& cir, Square& sq) {
  Dir2 diff = ((cir.position - sq.position).abs() - Dir2 {sq.width, sq.height}).max0();
  return cir._colidalble && sq._colidalble && diff.modulo2() < cir.radio * cir.radio;
}
