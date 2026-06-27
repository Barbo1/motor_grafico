#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/primitives/types_definition.hpp"

#include <cmath>

void resolve_collision (Square& sq1, Square& sq2) {
  Dir2 sq1_pos = sq1.position;
  Dir2 sq1_dims = sq1.dims;
  Dir2 sq1_vel = sq1.velocity;

  Dir2 sq2_pos = sq2.position;
  Dir2 sq2_dims = sq2.dims;
  Dir2 sq2_vel = sq2.velocity;

  float mass_1 = sq1.get_mass(), mass_2 = sq2.get_mass();

  Dir2 diff = sq1_pos - sq2_pos;
  Dir2 size = sq1_dims + sq2_dims;
  
  float q = size.pL(diff.abs()) < 0.f ? 1.f : 0.f;
  Dir2 n(1.f - q, q);
  float res = diff * n;
  Dir2 ns = n * std::copysign(1.f, res);
  float p = ns * (sq1_vel - sq2_vel) * 2.f * ENERGY_DISIPATION / (mass_1 + mass_2);

  float coef_1 = sq1.config & PCO_MOVIBLE ? p * mass_2 : 0.f;
  float coef_2 = sq1.config & PCO_MOVIBLE ? p * mass_1 : 0.f;
  sq1.velocity.store(ns.nmadd (coef_1, sq1_vel));
  sq2.velocity.store(ns.madd (coef_2, sq2_vel));

  sq1.position.store(n.madd (size * ns - res, sq1_pos));

  sq1.collision_normal.store(ns);
  sq2.collision_normal.store(-ns);

  sq1.acc_f_k = sq1.f_k * sq2.f_k;
  sq2.acc_f_k = sq1.acc_f_k;

  sq1.config |= PCO_IS_NORMAL;
  sq2.config |= PCO_IS_NORMAL;
}
