#include "../../../../headers/concepts/collision.hpp"

void correct_collision (Line& line, NEdge& pol) {
  Dir2 pol_pos = Dir2(pol.position), dn;
  float dist;
  
  Dir2 collision_direction = calculate_reposition_line_nedge (
    line.p, line.v, pol_pos, pol.placed_segments, pol.size, dn, dist
  );

  pol.set_position(AngDir2(collision_direction + pol_pos, pol.ang_pos));
}
