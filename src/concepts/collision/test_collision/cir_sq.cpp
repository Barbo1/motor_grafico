#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"

bool test_collision (const Circle& cir, const Square& sq) {
  AngDir2 diff = ((Dir2(cir.position) - Dir2(sq.position)).abs() - Dir2 (sq.dims)).max0();
  return diff.modulo2() < cir.radio * cir.radio;
}
