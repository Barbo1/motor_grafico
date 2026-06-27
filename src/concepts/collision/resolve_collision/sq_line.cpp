#include "../../../../headers/pr_objects/line.hpp"
#include "../../../../headers/pr_objects/square.hpp"

#include <cmath>

void resolve_collision (Square& sq, Line& line) {
  Dir2 sq_pos = sq.position;
  Dir2 sq_vel = sq.velocity;
  Dir2 v = Dir2(line.v);
  Dir2 r = Dir2(sq.dims).dir_mul(Dir2 (-1.f, std::copysign(1.f, line.v.x * line.v.y)));
  float coef = v.pLd(Dir2(line.p) - sq_pos, r);
  Dir2 n = v.percan().normalize();

  sq.position.store(r.madd(coef - std::copysign(1.f, coef), sq_pos));
  sq.velocity.store(n.nmadd ((n * sq_vel) * 2.f, sq_vel));
  sq.collision_normal.store(n);

  sq.acc_f_k = 1.f;
  sq.config |= PCO_IS_NORMAL;
}
