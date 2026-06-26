#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/line.hpp"

#include <cmath>

void resolve_collision (Circle& cir, Line& line) {
  Dir2 cir_pos = cir.position;
  Dir2 cir_vel = cir.velocity;
  Dir2 v_L = Dir2(line.v).percan().normalize();
  Dir2 n = v_L * (v_L * (cir_pos - Dir2(line.p)));
  float n_norm = n.modulo();
  n = n.normalize();

  cir.position.store(n.madd (cir.radio - n_norm, cir_pos));
  cir.velocity.store(n.nmadd (2.f * (n * cir_vel), cir_vel));
  cir.collision_normal.store(n);

  cir.acc_f_k = 1.f;
  cir.config |= PCO_IS_NORMAL;
}
