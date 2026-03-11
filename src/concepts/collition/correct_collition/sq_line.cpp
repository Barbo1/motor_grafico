#include "../../../../headers/pr_objects/line.hpp"
#include "../../../../headers/pr_objects/square.hpp"

#include <cmath>

void correct_collition (Square& sq, Line& line) {
  Dir2 r = Dir2 (-sq.width, std::copysign(1.f, line.v.x * line.v.y) * sq.height);
  float coef = line.v.pLd(line.p - sq.position, r);

  sq.position += r * (coef - std::copysign(1.f, coef));
  sq._collition_normal = line.v.percan().normalize();
  sq._acc_f_k = 1.f;
  sq._normal_presence = true;
}
