#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"
#include "../../../../headers/primitives/operations.hpp"

void correct_collition (Square& sq1, Square& sq2) {
  Dir2 diff = sq1.position - sq2.position;
  Dir2 size = Dir2 {(float)sq1.width + sq2.width, (float)sq2.height + sq1.height};
  int pos = gti (absv(diff.y) + size.x, absv(diff.x) + size.y);
  float res = *(&diff.x + pos);
  *(&sq1.position.x + pos) += *(&size.x + pos) * sgn (res) - res;
}
