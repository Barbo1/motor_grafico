#include "../../../../headers/concepts/collision.hpp"

Dir2 collision_direction (Circle& cir, NEdge& pol) {
  Dir2 cir_pos = cir.position;

  uint32_t filtered;
  Dir2 d = get_direction_nedge_circle(
    cir.radio, cir_pos, pol.placed_segments, pol.size, filtered
  );
  pol.reposition_segments();

  return d.normalize();
}
