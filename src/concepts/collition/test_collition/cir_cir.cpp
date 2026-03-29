#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/concepts/collition.hpp"

bool test_collition (const Circle& cir1, const Circle& cir2) {
  AngDir2 diff = cir1.position - cir2.position;
  uint32_t radio = cir1.radio + cir2.radio;
  return cir1._colidalble && cir2._colidalble && diff.modulo2() < (radio * radio);
}
