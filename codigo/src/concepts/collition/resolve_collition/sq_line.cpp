#include "../../../../headers/pr_objects/line.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/primitives/operations.hpp"

void resolve_collition (Square& sq, Line& line) {
  float w = static_cast<float>(sq.width), 
        h = static_cast<float>(sq.height) * sgn(line.slope),
        a = (line.slope * sq.position.x - sq.position.y + line.deviation) / 
            (line.slope * w + h);
  AngDir2 u = AngDir2 {w, -h, 0} * (sgn(a) - a);
  AngDir2 t = AngDir2 {line.slope, -1.f, 0};
  sq.position += t * (t * u);
  sq._velocity -= t * (2.f * (line.slope * sq._velocity.x - sq._velocity.y));
}
