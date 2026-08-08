#include "../../../../headers/pr_objects/projectile.hpp"
#include "../../../../headers/pr_objects/line.hpp"
#include "../../../../headers/primitives/math.hpp"
#include "../../../../headers/primitives/types_definition.hpp"

bool test_collision (Projectile& proj, Line& line) {
  float mult_dist = INFINITY;
  mult_dist = std::min(coef_collision_projectil_line (
    Dir2(proj.position), Dir2(proj.direction_to_new_position), proj.radio, 
    line.p, line.v
  ), mult_dist);

  if (mult_dist < proj.multiplicative_distance) {
    proj.multiplicative_distance = mult_dist;
    proj.config |= (PRObjType::PR_LINE) << Projectile::recuperable_type;
    proj.col_obj = static_cast<void*>(&line);
    return true;
  }
  return false;
}
