#include "../../../../headers/concepts/collision.hpp"

bool test_collision (const Circle& cir1, const Circle& cir2) {
  Dir2 cir1_pos = cir1.position;
  Dir2 cir2_pos = cir2.position;
  uint32_t radio = cir1.radio + cir2.radio;
  return (cir1_pos - cir2_pos).modulo2() < (radio * radio);
}
