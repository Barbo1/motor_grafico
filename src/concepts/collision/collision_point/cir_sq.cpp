#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/pr_objects/square.hpp"

Dir2 collision_point (const Circle& cir, const Square& sq) {
  Dir2 sq_pos = sq.position;
  return sq_pos - (sq_pos - Dir2(cir.position)).bound (Dir2(sq.dims));
}
