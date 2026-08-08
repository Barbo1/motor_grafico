#include "../../../../headers/primitives/types_definition.hpp"
#include "../../../../headers/concepts/collision.hpp"
#include "../../../../headers/primitives/math.hpp"

bool test_collision (Projectile& proj1, Projectile& proj2) {
  float mult_dist = coef_collision_projectile_circle (
    Dir2(proj1.position), Dir2(proj1.direction_to_new_position), proj1.radio, 
    Dir2(proj2.position), Dir2(proj2.direction_to_new_position), proj2.radio
  );
  if (mult_dist < proj2.multiplicative_distance && mult_dist < proj1.multiplicative_distance) {
    proj2.multiplicative_distance = mult_dist;
    proj2.config &= 0xFFFFFFE3;
    proj2.config |= (PRObjType::PR_PROJECTILE) << Projectile::recuperable_type;
    proj2.col_obj = static_cast<void*>(&proj1);

    proj1.multiplicative_distance = mult_dist;
    proj1.config &= 0xFFFFFFE3;
    proj1.config |= (PRObjType::PR_PROJECTILE) << Projectile::recuperable_type;
    proj1.col_obj = static_cast<void*>(&proj2);
    return true;
  }
  return false;
}
