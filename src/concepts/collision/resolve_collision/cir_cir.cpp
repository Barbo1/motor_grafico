#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/primitives/types_definition.hpp"

void resolve_collision (Circle& cir1, Circle& cir2) {
  Dir2 cir1_pos = cir1.position;
  Dir2 cir1_vel = cir1.velocity;

  Dir2 cir2_pos = cir2.position;
  Dir2 cir2_vel = cir2.velocity;

  float mass_1 = cir1.get_mass(), mass_2 = cir2.get_mass();

  Dir2 n = (cir1_pos - cir2_pos).normalize();
  float p = (cir1_vel - cir2_vel) * n * 2.f * ENERGY_DISIPATION / (mass_1 + mass_2);

  float coef_1 = cir1.config & PCO_MOVIBLE ? p * mass_2 : 0.f;
  float coef_2 = cir2.config & PCO_MOVIBLE ? p * mass_1 : 0.f;
  cir1.velocity.store(n.nmadd (coef_1, cir1_vel));
  cir2.velocity.store(n.madd (coef_2, cir2_vel));

  cir1.position.store(n.madd (cir1.radio + cir2.radio, cir2_pos));
  cir1.collision_normal.store(n);
  cir2.collision_normal.store(-n);
  
  cir1.acc_f_k = cir1.f_k * cir2.f_k;
  cir2.acc_f_k = cir1.acc_f_k;
  
  cir1.config |= PCO_IS_NORMAL;
  cir2.config |= PCO_IS_NORMAL;
}
