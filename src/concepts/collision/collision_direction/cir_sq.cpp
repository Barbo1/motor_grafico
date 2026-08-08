#include "../../../../headers/concepts/collision.hpp"

Dir2 collision_direction (Circle& cir, Square& sq) {
  Dir2 cir_pos = cir.position;
  Dir2 sq_pos = sq.position;
  Dir2 diff = cir_pos - sq_pos;
  return (diff.bound(Dir2 (sq.dims)) - diff).normalize();
}
