#include "../../../../headers/concepts/collision.hpp"

Dir2 collision_point (Line& line, NEdge& pol) {
  return collision_point_line_nedge (
    line.p, line.v, 
    pol.placed_segments,
    pol.size
  );
}
