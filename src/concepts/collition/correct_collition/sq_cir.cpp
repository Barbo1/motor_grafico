#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"

void correct_collition (Square& sq, Circle& cir) {
  AngDir2 diff = sq.position - cir.position;
  AngDir2 b = diff.bound(AngDir2 {sq.width, sq.height, 0.f}) - diff;
  AngDir2 n = b.normalize();

  sq.position += n.nmadd(cir.radio, b);

  sq._acc_f_k = sq._f_k * cir._f_k;
  cir._acc_f_k = sq._acc_f_k;

  cir._collition_normal = n;
  sq._collition_normal = -n;

  sq._normal_presence = true;
  cir._normal_presence = true;
}
