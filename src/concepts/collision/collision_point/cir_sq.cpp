#include "../../../../headers/concepts/collision.hpp"

Dir2 collision_point (Circle& cir, Square& sq) {
  Dir2 sq_pos = sq.position;
  return sq_pos - (sq_pos - Dir2(cir.position)).bound (Dir2(sq.dims));
}
