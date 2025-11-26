#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"

void correct_collition (Square& sq1, Square& sq2) {
  AngDir2 diff = sq1.position - sq2.position;
  AngDir2 size = Dir2 {sq1.width + sq2.width, sq2.height + sq1.height};

  float q = static_cast<float>(size.pL(diff.abs()) < 0.f);
  AngDir2 n(1.f - q, q, 0.f);
  float res = diff * n;
  sq1._collition_normal = n * std::copysignf(1.f, res);

  sq1.position += n * (size * sq1._collition_normal - res);

  sq2._collition_normal = -sq1._collition_normal;

  sq1._acc_f_k = sq1._f_k * sq2._f_k;
  sq2._acc_f_k = sq1._acc_f_k;

  sq1._normal_presence = true;
  sq2._normal_presence = true;
}
