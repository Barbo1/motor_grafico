#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"

bool test_collition (Circle& cir, Square& sq) {
  AngDir2 diff = ((cir.position - sq.position).abs() - AngDir2 {sq.width, sq.height, 0.f}).max0();
  return cir._colidalble && sq._colidalble && diff.modulo2() < cir.radio * cir.radio;
}
