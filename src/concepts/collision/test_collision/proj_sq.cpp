#include "../../../../headers/primitives/types_definition.hpp"
#include "../../../../headers/concepts/collision.hpp"
#include "../../../../headers/primitives/math.hpp"

bool test_collision (Projectile& proj, Square& sq) {
  Dir2 sq_dims = Dir2(sq.dims);
  Dir2 down = sq_dims.dir_mul(Dir2(0.f, -2.f));
  Dir2 right = sq_dims.dir_mul(Dir2(2.f, 0.f));

  Dir2 P = Dir2(sq.position);

  Dir2 D = P + sq_dims;
  Dir2 I = P - sq_dims;
  Dir2 P1 = P + sq_dims.dir_mul(Dir2(-1.f, 1.f));

  Dir2 proj_pos = Dir2(proj.position);
  Dir2 proj_new_pos = Dir2(proj.direction_to_new_position);

  float mult_dist_1 = std::min(
    coef_collision_projectil_segment (proj_pos, proj_new_pos, proj.radio, D, down),
    coef_collision_projectil_segment (proj_pos, proj_new_pos, proj.radio, P1, down)
  );
  float mult_dist_2 = std::min(
    coef_collision_projectil_segment (proj_pos, proj_new_pos, proj.radio, I, right),
    coef_collision_projectil_segment (proj_pos, proj_new_pos, proj.radio, P1, right)
  );
  float mult_dist = std::min(mult_dist_1, mult_dist_2);

  if (mult_dist < proj.multiplicative_distance) {
    proj.multiplicative_distance = mult_dist;
    proj.config &= 0xFFFFFFE3;
    proj.config |= (PRObjType::PR_SQUARE) << Projectile::recuperable_type;
    proj.col_obj = static_cast<void*>(&sq);
    return true;
  }
  return false;
}
