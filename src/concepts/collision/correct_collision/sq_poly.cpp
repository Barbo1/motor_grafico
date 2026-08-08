#include "../../../../headers/concepts/collision.hpp"

void correct_collision (Square& sq, NEdge& pol, bool move_first) {
  Dir2 sq_dims = Dir2(sq.dims);
  Dir2 sq_pos = Dir2(sq.position);

  Dir2 dn = Dir2();
  Dir2 collision_direction = resposition_direction_square_nedge(
    sq_pos, sq_dims,
    pol.placed_segments, 
    pol.placed_triangles,
    pol.size,
    dn
  );

  if (dn.modulo2() == 0.f)
    return;
  
  if (move_first) {
    sq.position.store(sq_pos + collision_direction);
    pol.reposition_segments();
  } else {
    pol.set_position(AngDir2(Dir2(pol.position) - collision_direction, pol.ang_pos));
  }
}
