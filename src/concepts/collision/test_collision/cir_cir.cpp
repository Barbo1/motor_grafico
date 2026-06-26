#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/concepts/collision.hpp"

bool test_collision (const Circle& cir1, const Circle& cir2) {
  Dir2 cir1_pos = cir1.position;
  Dir2 cir2_pos = cir2.position;
  AngDir2 diff = cir1_pos - cir2_pos;
  uint32_t radio = cir1.radio + cir2.radio;
  return diff.modulo2() < (radio * radio);
}
