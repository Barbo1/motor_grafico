#include "../../../../headers/pr_objects/projectile.hpp"
#include "../../../../headers/pr_objects/circle.hpp"
#include "../../../../headers/primitives/math.hpp"
#include "../../../../headers/primitives/types_definition.hpp"

bool test_collision (Projectile& proj, Circle& cir) {
  float mult_dist = coef_collision_projectile_circle (
    Dir2(proj.position), Dir2(proj.direction_to_new_position), proj.radio, 
    Dir2(cir.position), Dir2(), cir.radio
  );

  if (mult_dist < proj.multiplicative_distance) {
    proj.multiplicative_distance = mult_dist;
    proj.config &= 0xFFFFFFE3;
    proj.config |= (PRObjType::PR_CIRCLE) << Projectile::recuperable_type;
    proj.col_obj = static_cast<void*>(&cir);
    return true;
  }
  return false;
}
