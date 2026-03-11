#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/line.hpp"

#include <cmath>

void correct_collition (Circle& cir, Line& line) {
  Dir2 v_L = line.v.percan().normalize();
  Dir2 n = v_L * (v_L * (cir.position - line.p));
  float n_norm = n.modulo();
  n = n.normalize();

  cir.position = n.madd (cir.radio - n_norm, cir.position);
  cir._collition_normal = n;
  cir._acc_f_k = 1.f;
  cir._normal_presence = true;
}
