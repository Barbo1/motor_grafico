#include "../../../../headers/pr_objects/line.hpp"
#include "../../../../headers/pr_objects/square.hpp"

#include <cmath>

void resolve_collition (Square& sq, Line& line) {
  Dir2 r = Dir2 (-sq.width, std::copysign(1.f, line.v.x * line.v.y) * sq.height);
  float coef = line.v.pLd(line.p - sq.position, r);
  Dir2 n = line.v.percan().normalize();

  sq.position = r.madd(coef - std::copysign(1.f, coef), sq.position);
  sq._velocity = n.nmadd (n * sq._velocity * 2.f, sq._velocity);
  sq._collition_normal = n;
  sq._acc_f_k = 1.f;
  sq._normal_presence = true;
}
