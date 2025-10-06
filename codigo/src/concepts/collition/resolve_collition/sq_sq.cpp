#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/concepts/operations.hpp"

void resolve_collition (Square& sq1, Square& sq2) {
  Dir2 diff = sq1.position - sq2.position;
  Dir2 diffa = diff.abs();
  float ws = sq1.width + sq2.width;
  float hs = sq2.height + sq1.height;

  int pos = diffa.x + hs < diffa.y + ws;
  float * vel_elem1 = &sq1._velocity.x + pos;
  float * vel_elem2 = &sq2._velocity.x + pos;

  float mass_1 = sq1.get_mass(), mass_2 = sq2.get_mass();
  float denom = 1.f / (mass_1 + mass_2);
  float coef_3 = (mass_1 - mass_2);

  float new_elem = (coef_3 * *vel_elem1 + 2.f * mass_2 * *vel_elem2) * denom;
  *vel_elem2 = (2.f * mass_1 * *vel_elem1 - coef_3 * *vel_elem2) * denom * sq2._movible;
  *vel_elem1 = new_elem * sq1._movible;

  float res = *(&diff.x + pos);
  *(&sq1.position.x + pos) += ((pos ? hs : ws) - *(&diffa.x + pos)) * sgn (res);
}
