#include "../../../../headers/pr_objects/circle.hpp"

void resolve_collition (Circle& cir1, Circle& cir2) {
  float mass_1 = cir1.get_mass(), mass_2 = cir2.get_mass();
  AngDir2 n = (cir1.position - cir2.position).normalize();
  float p = (cir1._velocity - cir2._velocity) * n * 2.f / (mass_1 + mass_2);

  cir1._velocity -= n * (p * mass_2 * cir1._movible);
  cir2._velocity += n * (p * mass_1 * cir2._movible);

  cir1.position = n.madd(cir1.radio + cir2.radio, cir2.position);

  cir1._collition_normal = n;
  cir2._collition_normal = -n;
  
  cir1._acc_f_k = cir1._f_k * cir2._f_k;
  cir2._acc_f_k = cir1._acc_f_k;
  
  cir1._normal_presence = true;
  cir2._normal_presence = true;
}
