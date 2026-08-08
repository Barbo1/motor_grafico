#include "../../../../headers/concepts/collision.hpp"

Dir2 collision_point (Square& sq, NEdge& pol) {
  return collision_point_nedge_square (sq.position, sq.dims, pol.placed_segments, pol.size);
}
