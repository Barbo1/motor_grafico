#include "../../../../headers/concepts/collision.hpp"

bool test_collision (const Line& line, const Circle& cir) {
  return 
    std::abs(Dir2(line.v).percan().normalize() * (Dir2(line.p) - Dir2(cir.position))) < cir.radio;
}
