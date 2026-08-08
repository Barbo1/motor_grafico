#include "../../../../headers/concepts/collision.hpp"
#include <cmath>

Dir2 collision_direction (Line& line, Circle& cir) {
  Dir2 dL = Dir2(line.v).percan();
  float sgn = std::copysign(1.f, dL * (Dir2(cir.position) - Dir2(line.p)));
  return dL.normalize() * sgn;
}
