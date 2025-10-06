#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/line.hpp"

void resolve_collition (Circle& cir, Line& line) {
  AngDir2 u = AngDir2 {line.slope, -1.f, cir.position.a};
  u *= (cir.position.y - (line.slope * cir.position.x + line.deviation)) / 
    (line.slope * line.slope + 1.f);
  AngDir2 n = u.normalize();

  cir._velocity -= n * (2.f * (n * cir._velocity));
  cir.position += u;
  cir.position -= n * cir.radio;
}
