#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/primitives/types_definition.hpp"

void resolve_collision (Circle& cir, Square& sq) {
  Dir2 sq_pos = sq.position;
  Dir2 cir_pos = cir.position;
  Dir2 sq_vel= sq.velocity;
  Dir2 cir_vel = cir.velocity;
  float mass_1 = sq.get_mass(), mass_2 = cir.get_mass();
  Dir2 diff = sq_pos - cir_pos;
  Dir2 b = diff.bound(Dir2 (sq.dims)) - diff;
  Dir2 n = b.normalize(); 
  float p = n * (sq_vel - cir_vel) * 2.f * ENERGY_DISIPATION / (mass_1 + mass_2);

  float coef_1 = sq.config & PCO_MOVIBLE ? p * mass_2 : 0.f;
  float coef_2 = cir.config & PCO_MOVIBLE ? p * mass_1 : 0.f;
  sq.velocity.store(n.nmadd (coef_1, sq_vel));
  cir.velocity.store(n.madd (coef_2, cir_vel));
  cir.position.store(n.msub(cir.radio, b - cir_pos));

  sq.acc_f_k = sq.f_k * cir.f_k;
  cir.acc_f_k = sq.acc_f_k;

  cir.collision_normal.store(n);
  sq.collision_normal.store(-n);

  sq.config |= PCO_IS_NORMAL;
  cir.config |= PCO_IS_NORMAL;
}
