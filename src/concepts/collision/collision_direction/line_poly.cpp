#include "../../../../headers/concepts/collision.hpp"

Dir2 collision_direction (Line& line, NEdge& pol) {
  Dir2 dn;
  float distance = 0.f;
  calculate_reposition_line_nedge (
    line.p, line.v, pol.position, pol.placed_segments, pol.size, dn, distance
  );
  return dn;
}
