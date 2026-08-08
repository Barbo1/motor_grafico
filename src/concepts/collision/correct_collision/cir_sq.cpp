#include "../../../../headers/primitives/types_definition.hpp"
#include "../../../../headers/concepts/collision.hpp"

void correct_collision (Circle& cir, Square& sq, bool move_first) {
  Dir2 sq_pos = sq.position;
  Dir2 cir_pos = cir.position;
  Dir2 diff = sq_pos - cir_pos;
  Dir2 b = diff.bound(Dir2(sq.dims)) - diff;
  Dir2 n = b.normalize();

  if (move_first)
    cir.position.store(n.nmadd(cir.radio, cir_pos - b));
  else
    sq.position.store(n.msub(cir.radio, b + sq_pos));

  cir.collision_normal.store(n);
  sq.collision_normal.store(-n);

  cir.acc_f_k = cir.f_k * sq.f_k;
  sq.acc_f_k = cir.acc_f_k;
 
  cir.config |= PCO_IS_NORMAL;
  sq.config |= PCO_IS_NORMAL;
}
