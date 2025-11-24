#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"

void resolve_collition (Circle& cir, Square& sq) {
  float mass_1 = sq.get_mass(), mass_2 = cir.get_mass();
  AngDir2 diff = sq.position - cir.position;
  AngDir2 b = diff.bound(AngDir2 {sq.width, sq.height, 0.f}) - diff;
  AngDir2 n = b.normalize(); 
  float p = n * (sq._velocity - cir._velocity) * 2.f / (mass_1 + mass_2);

  sq._velocity -= n * (p * mass_2 * sq._movible);
  cir._velocity += n * (p * mass_1 * cir._movible);

  cir.position += n.msub(cir.radio, b);

  sq._acc_f_k = sq._f_k * cir._f_k;
  cir._acc_f_k = sq._acc_f_k;

  cir._collition_normal = n;
  sq._collition_normal = -n;

  sq._normal_presence = true;
  cir._normal_presence = true;
}
