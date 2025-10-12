#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/line.hpp"

void correct_collition (Circle& cir, Line& line) {
  AngDir2 u = AngDir2 {line.slope, -1.f, cir.position.a};
  u *= (cir.position.y - (line.slope * cir.position.x + line.deviation)) / 
    (line.slope * line.slope + 1.f);
  cir.position += u;
  cir.position -= u.normalize() * cir.radio;
}
