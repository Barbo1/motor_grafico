#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/primitives/operations.hpp"

void correct_collition (Circle& cir, Square& sq) {
  AngDir2 diff = sq.position - cir.position;
  AngDir2 b = AngDir2 {
    bound (diff.x, sq.width) - diff.x,
    bound (diff.y, sq.height) - diff.y,
    0
  };
  AngDir2 n = b.normalize(); 

  cir.position -= b;
  cir.position += n * cir.radio;

  sq._acc_f_k = sq._f_k * cir._f_k;
  cir._acc_f_k = sq._acc_f_k;

  cir._collition_normal = n;
  sq._collition_normal = -n;

  sq._normal_presence = true;
  cir._normal_presence = true;
}
