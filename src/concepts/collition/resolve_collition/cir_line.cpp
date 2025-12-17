#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/line.hpp"

#include <cmath>

void resolve_collition (Circle& cir, Line& line) {
  AngDir2 u = AngDir2 {line.slope, -1.f, cir.position.a};
  float num = cir.position.y - std::fmaf(line.slope, cir.position.x, line.deviation);
  float denom = std::fmaf(line.slope, line.slope, 1.f);
  u *= num / denom;
  AngDir2 n = u.normalize();

  cir._velocity = n.nmadd(2.f * (n * cir._velocity), cir._velocity);
  cir.position += n.nmadd(cir.radio, u);

  cir._collition_normal = n;
  cir._acc_f_k = 1.f;
  cir._normal_presence = true;
}
