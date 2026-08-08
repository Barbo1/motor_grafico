#include "../../../../headers/concepts/collision.hpp"

void correct_collision (Circle& cir, NEdge& pol, bool move_first) {
  uint32_t filtered;
  Dir2 cir_pos = cir.position;
  Dir2 d = get_direction_nedge_circle(
    cir.radio, 
    cir_pos, 
    pol.placed_segments,
    pol.size,
    filtered
  );

  if (d.modulo2() == 0.f)
    return;
  Dir2 reposition = calculate_reposition_nedge_circle(
    d, cir.radio, cir_pos, pol.placed_segments, filtered
  );

  // calculate the reposition distance based on the direction d.
  if (move_first) {
    cir.position.store(cir_pos - reposition);
    pol.reposition_segments();
  } else {
    pol.set_position(AngDir2(reposition, pol.ang_pos));
  }
}
