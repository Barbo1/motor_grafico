#include "../../../../headers/pr_objects/line.hpp"
#include "../../../../headers/pr_objects/square.hpp"

#include <cmath>

void correct_collition (Square& sq, Line& line) {
  float h = sq.height * std::copysignf(1.f, line.slope), 
        a = (std::fmaf(line.slope, sq.position.x, line.deviation) - sq.position.y) / 
            std::fmaf(line.slope, sq.width, h);
  AngDir2 t = AngDir2 {line.slope, -1.f, 0}.normalize();
  t *= t * (AngDir2 {sq.width, -h, 0} * (std::copysignf(1.f, a) - a));

  sq.position += t;

  sq._collition_normal = t.normalize();
  sq._acc_f_k = 1.f;
  sq._normal_presence = true;
}
