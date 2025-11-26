#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/line.hpp"

#include <cmath>

void correct_collition (Circle& cir, Line& line) {
  AngDir2 u = AngDir2 {line.slope, -1.f, cir.position.a};
  u *= (cir.position.y - std::fmaf(line.slope, cir.position.x, line.deviation)) / 
    std::fmaf(line.slope, line.slope, 1.f);
  AngDir2 n = u.normalize();

  cir.position += n.nmadd(cir.radio, u);
  cir._collition_normal = n;
  cir._acc_f_k = 1.f;
  cir._normal_presence = true;
}
