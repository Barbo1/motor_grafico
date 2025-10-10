#include "../../../../headers/pr_objects/line.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/primitives/operations.hpp"

void resolve_collition (Square& sq, Line& line) {
  float h = static_cast<float>(sq.height) * sgn(line.slope), 
        w = static_cast<float>(sq.width),
        a = (line.slope * sq.position.x + line.deviation - sq.position.y) / 
            (line.slope * w + h);
  AngDir2 t = AngDir2 {line.slope, -1.f, 0}.normalize();
  t = t * (t * (AngDir2 {w, -h, 0} * (sgn(a) - a)));
  sq.position += t;
  t = t.normalize();
  sq._velocity += t * ((-2.f) * (t * sq._velocity));
}
