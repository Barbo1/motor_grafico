#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/primitives/types_definition.hpp"

void correct_collision (Circle& cir1, Circle& cir2) {
  Dir2 cir1_pos = cir1.position;
  Dir2 cir2_pos = cir2.position;
  Dir2 normal = (cir1_pos - cir2_pos).normalize();

  cir1.position.store(normal.madd(cir1.radio + cir2.radio, cir2_pos));
  cir1.collision_normal.store(normal);
  cir2.collision_normal.store(-normal);
  
  cir1.acc_f_k = cir1.f_k * cir2.f_k;
  cir2.acc_f_k = cir1.acc_f_k;
 
  cir1.config |= PCO_IS_NORMAL;
  cir2.config |= PCO_IS_NORMAL;
}
