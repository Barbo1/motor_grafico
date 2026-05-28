#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"

void correct_collition (Square& sq1, Square& sq2) {
  Dir2 diff = sq1.position - sq2.position;
  Dir2 size = Dir2 {sq1.width + sq2.width, sq2.height + sq1.height};

  float q = static_cast<float>(size.pL(diff.abs()) < 0.f);
  Dir2 n(1.f - q, q);
  float res = diff * n;
  Dir2 ns = n * std::copysignf(1.f, res);

  sq1._collition_normal = ns;
  sq2._collition_normal = -ns;

  sq1.position = n.madd(size * ns - res, sq1.position);

  sq1._acc_f_k = sq1._f_k * sq2._f_k;
  sq2._acc_f_k = sq1._acc_f_k;

  sq1._normal_presence = true;
  sq2._normal_presence = true;
}
