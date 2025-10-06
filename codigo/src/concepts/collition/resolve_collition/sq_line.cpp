#include "../../../../headers/pr_objects/line.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/concepts/operations.hpp"

void resolve_collition (Square& sq, Line& line) {
  float w = static_cast<float>(sq.width), h = static_cast<float>(sq.height) * sgn(line.slope);
  float coef = (line.slope * sq.position.x + line.deviation - sq.position.y);
  coef /= (line.slope * w + h);
  coef += 1;
  sq.position += AngDir2 {-w, h, sq.position.a} * coef;

  AngDir2 n = AngDir2 {line.slope, -1.f, sq.position.a}.normalize();
  sq._velocity -= n * (2.f * (n * sq._velocity));
}
