#include "../../../../headers/pr_objects/circle.hpp"

bool test_collition (Circle & cir1, Circle & cir2) {
  Dir2 diff = cir1.position - cir2.position;
  uint32_t radio = cir1.radio + cir2.radio;
  return cir1._colidalble && cir2._colidalble && diff.modulo2() < (radio * radio);
}
