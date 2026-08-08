#include "../../../../headers/concepts/collision.hpp"

Dir2 collision_point (Circle& cir1, Circle& cir2) {
  Dir2 cir2pos = Dir2(cir2.position);
  Dir2 n = (Dir2(cir1.position) - cir2pos).normalize();
  return n.madd(cir2.radio, cir2pos);
}
