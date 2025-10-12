#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/primitives/operations.hpp"

void correct_collition (Circle& cir, Square& sq) {
  float mass_1 = sq.get_mass(), mass_2 = cir.get_mass();
  AngDir2 diff = sq.position - cir.position;
  AngDir2 b = AngDir2 {
    sgn(diff.x) * min(sq.width, max0(absv(diff.x))) - diff.x,
    sgn(diff.y) * min(sq.height, max0(absv(diff.y))) - diff.y,
    0
  };
  cir.position -= b;
  cir.position += b.normalize() * cir.radio;
}
