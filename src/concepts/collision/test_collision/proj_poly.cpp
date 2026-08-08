#include "../../../../headers/primitives/types_definition.hpp"
#include "../../../../headers/concepts/collision.hpp"
#include "../../../../headers/primitives/math.hpp"

bool test_collision (Projectile& proj, NEdge& pol) {
  float mult_dist = INFINITY;

  for (uint32_t i = 0; i < pol.size; i++) {
    const auto& segment = pol.placed_segments[i];
    mult_dist = std::min(coef_collision_projectil_segment (
      Dir2(proj.position), Dir2(proj.direction_to_new_position), proj.radio, 
      segment[1], segment[0] 
    ), mult_dist);
  }

  if (mult_dist < proj.multiplicative_distance) {
    proj.multiplicative_distance = mult_dist;
    proj.config &= 0xFFFFFFE3;
    proj.config |= (PRObjType::PR_NEDGE) << Projectile::recuperable_type;
    proj.col_obj = static_cast<void*>(&pol);
    return true;
  }
  return false;
}
