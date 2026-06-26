#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/primitives/types_definition.hpp"

void correct_collision (Circle& cir, Square& sq) {
  Dir2 sq_pos = sq.position;
  Dir2 cir_pos = cir.position;
  Dir2 diff = sq_pos - cir_pos;
  Dir2 b = diff.bound(Dir2(sq.dims)) - diff;
  Dir2 n = b.normalize(); 

  cir.position.store(n.nmadd(cir.radio, cir_pos - b));
  cir.collision_normal.store(n);
  sq.collision_normal.store(-n);

  cir.acc_f_k = cir.f_k * sq.f_k;
  sq.acc_f_k = cir.acc_f_k;
 
  cir.config |= PCO_IS_NORMAL;
  sq.config |= PCO_IS_NORMAL;
}
