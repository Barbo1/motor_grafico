#include "../../../../headers/concepts/collision.hpp"

Dir2 collision_direction (Square& sq, NEdge& pol) {
  Dir2 dn = Dir2();
  resposition_direction_square_nedge(
    Dir2(sq.position), 
    Dir2(sq.dims),
    pol.placed_segments, 
    pol.placed_triangles,
    pol.size,
    dn
  );
  pol.reposition_segments();
  return dn;
}
