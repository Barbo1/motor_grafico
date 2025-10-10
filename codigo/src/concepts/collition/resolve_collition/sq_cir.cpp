#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/primitives/operations.hpp"

void resolve_collition (Square& sq, Circle& cir) {
  float mass_1 = sq.get_mass(), mass_2 = cir.get_mass();
  AngDir2 diff = sq.position - cir.position;
  Dir2 diffa = diff.abs();

  bool pos = diffa.x < sq.width;
  if (pos || diffa.y < sq.height) {
    float * vel_elem1 = &sq._velocity.x + pos;
    float * vel_elem2 = &cir._velocity.x + pos;

    float denom = 1.f / (mass_1 + mass_2);
    float coef_3 = (mass_1 - mass_2);

    float new_elem = (coef_3 * *vel_elem1 + 2.f * mass_2 * *vel_elem2) * denom;
    *vel_elem2 = (2.f * mass_1 * *vel_elem1 - coef_3 * *vel_elem2) * denom * cir._movible;
    *vel_elem1 = new_elem * sq._movible;

    *(&sq.position.x + pos) -= 
      (*(&diffa.x + pos) - (cir.radio + (pos ? sq.height : sq.width))) * sgn (*(&diff.x + pos));

    return;

  } else {
    AngDir2 b = AngDir2 {
      diff.x - static_cast<float>(sq.width) * sgn(diff.x),
      diff.y - static_cast<float>(sq.height) * sgn(diff.y),
      0
    };
    AngDir2 n = b.normalize();
    float p = ((cir._velocity - sq._velocity) * n) * 2.f / (mass_1 + mass_2);

    sq._velocity += n * (p * mass_2 * sq._movible);
    cir._velocity -= n * (p * mass_1 * cir._movible);

    b -= n * cir.radio;
    sq.position -= b;

    return;
  }
}
