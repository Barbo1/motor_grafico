#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/primitives/operations.hpp"

void resolve_collition (Square& sq1, Square& sq2) {
  AngDir2 diff = sq1.position - sq2.position;
  Dir2 size = Dir2 {sq1.width + sq2.width, sq2.height + sq1.height};

  int pos = gti (absv(diff.y) + size.x, absv(diff.x) + size.y);
  float * vel_elem1 = &sq1._velocity.x + pos;
  float * vel_elem2 = &sq2._velocity.x + pos;

  float mass_1 = sq1.get_mass(), mass_2 = sq2.get_mass();
  float denom = 1.f / (mass_1 + mass_2);
  float coef_3 = (mass_1 - mass_2);

  float new_elem = (coef_3 * *vel_elem1 + 2.f * mass_2 * *vel_elem2) * denom;
  *vel_elem2 = (2.f * mass_1 * *vel_elem1 - coef_3 * *vel_elem2) * denom * sq2._movible;
  *vel_elem1 = new_elem * sq1._movible;

  float res = *(&diff.x + pos);
  float sign = sgn (res);
  *(&sq1.position.x + pos) += *(&size.x + pos) * sign - res;

  sq1._collition_normal = AngDir2 {0.f, 0.f, 0.f};
  *(&sq1._collition_normal.x + pos) = sign;
  sq2._collition_normal = -sq1._collition_normal;

  sq1._acc_f_k = sq1._f_k * sq2._f_k;
  sq2._acc_f_k = sq1._acc_f_k;

  sq1._normal_presence = true;
  sq2._normal_presence = true;
}
