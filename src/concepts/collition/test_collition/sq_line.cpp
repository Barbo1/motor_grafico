#include "../../../../headers/pr_objects/line.hpp"
#include "../../../../headers/pr_objects/square.hpp"

#include <cmath>

bool test_collition (Square& sq, Line& line) {
  Dir2 u = Dir2 (-sq.width, std::copysign(1.f, line.v.x * line.v.y) * sq.height) * -2.f;
  return std::abs(line.v.pLd(line.p - sq.position, u)) < 0.5f;
}
