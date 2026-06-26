#include "../../../../headers/pr_objects/line.hpp"
#include "../../../../headers/pr_objects/square.hpp"

#include <cmath>

void correct_collision (Square& sq, Line& line) {
  Dir2 sq_pos = Dir2(sq.position);
  Dir2 v = Dir2(line.v);
  Dir2 r = Dir2 (-sq.dims.x, std::copysign(1.f, line.v.x * line.v.y) * sq.dims.y);
  float coef = v.pLd(Dir2(line.p) - sq_pos, r);

  sq.position.store(r.madd(coef - std::copysign(1.f, coef), sq_pos));
  sq.collision_normal.store(v.percan().normalize());
  sq.acc_f_k = 1.f;
  sq.config |= PCO_IS_NORMAL;
}
