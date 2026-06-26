#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/primitives/types_definition.hpp"

void correct_collision (Square& sq1, Square& sq2) {
  Dir2 sq1_pos = sq1.position;
  Dir2 sq1_dims = sq1.dims;
  Dir2 sq2_pos = sq2.position;
  Dir2 sq2_dims = sq2.dims;

  Dir2 diff = sq1_pos - sq2_pos;
  Dir2 size = sq1_dims + sq2_dims;

  float q = static_cast<float>(size.pL(diff.abs()) < 0.f);
  Dir2 n(1.f - q, q);
  float res = diff * n;
  Dir2 ns = n * std::copysignf(1.f, res);

  sq1.collision_normal.store(ns);
  sq2.collision_normal.store(-ns);

  sq1.position.store(n.madd(size * ns - res, Dir2(sq1.position)));

  sq1.acc_f_k = sq1.f_k * sq2.f_k;
  sq2.acc_f_k = sq1.acc_f_k;

  sq1.config |= PCO_IS_NORMAL;
  sq2.config |= PCO_IS_NORMAL;
}
