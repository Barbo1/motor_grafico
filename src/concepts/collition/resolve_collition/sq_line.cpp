#include "../../../../headers/pr_objects/line.hpp"
#include "../../../../headers/pr_objects/square.hpp"

#include <cmath>

void resolve_collition (Square& sq, Line& line) {
  float h = sq.height * std::copysignf(1.f, line.slope), 
        a = (std::fmaf(line.slope, sq.position.x, line.deviation) - sq.position.y) / 
            (std::fmaf(line.slope, sq.width, h));
  sq._collition_normal = AngDir2 {line.slope, -1.f, 0}.normalize();

  sq.position = sq._collition_normal.madd (
    sq._collition_normal * AngDir2 {sq.width, -h, 0} * (std::copysignf(1.f, a) - a), 
    sq.position
  );

  sq._velocity = sq._collition_normal.nmadd (sq._collition_normal * sq._velocity * 2.f, sq._velocity);

  sq._acc_f_k = 1.f;
  sq._normal_presence = true;
}
