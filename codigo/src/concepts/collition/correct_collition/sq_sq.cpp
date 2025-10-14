#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/primitives/operations.hpp"

void correct_collition (Square& sq1, Square& sq2) {
  Dir2 diff = sq1.position - sq2.position;
  Dir2 size = Dir2 {sq1.width + sq2.width, sq2.height + sq1.height};
  int pos = gti (absv(diff.y) + size.x, absv(diff.x) + size.y);
  float res = *(&diff.x + pos);
  float sign = sgn (res);
  *(&sq1.position.x + pos) += *(&size.x + pos) * sign - res;

  sq1._collition_normal = AngDir2 {0.f, 0.f, 0.f};
  *(&sq1._collition_normal.x + pos) = sign;
  sq2._collition_normal = -sq1._collition_normal;

  sq1._acc_f_k = sq1._f_k * sq2._f_k;
  sq2._acc_f_k = sq1._acc_f_k;

  sq1._normal_presence = true;
  sq2._normal_presence = true;
}
