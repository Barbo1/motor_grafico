#include "../../../../headers/concepts/collision.hpp"
#include "../../../../headers/primitives/types_definition.hpp"

void resolve_collision (Square& sq, NEdge& pol, bool move_first) {
  Dir2 sq_dims = Dir2(sq.dims);
  Dir2 sq_pos = Dir2(sq.position);

  Dir2 dn = Dir2();
  Dir2 collision_direction = resposition_direction_square_nedge(
    sq_pos, 
    sq_dims,
    pol.placed_segments, 
    pol.placed_triangles,
    pol.size,
    dn
  );
  pol.reposition_segments();

  if (dn.modulo2() == 0.f)
    return;
  Dir2 collision_point = collision_point_nedge_square (sq_pos, sq_dims, pol.placed_segments, pol.size);

  // calculate resolution.
  float inv_sq_mass = 1.f / sq.get_mass();
  float inv_pol_mass = 1.f / pol.get_mass();

  Dir2 pol_pos = Dir2(pol.position);
  Dir2 sq_v = sq.velocity;
  Dir2 pol_v = pol.velocity;
  Dir2 pol_r = collision_point - pol_pos;
  Dir2 pol_v_total = pol_r.percan().madd(pol.ang_vel, pol_v);

  float pol_coef = dn.pL(pol_r);
  Dir2 v_diff = pol_v_total - sq_v;
  float j = -2.f * (dn * v_diff) / (
    inv_sq_mass + 
    inv_pol_mass + 
    pol_coef * pol_coef / pol.inertia 
  );
  Dir2 J = dn * j;

  sq.velocity.store(J.nmadd(inv_sq_mass, sq_v));
  pol.velocity.store(J.madd(inv_pol_mass, pol_v));
  pol.ang_vel = pol.ang_vel + J.pL(pol_r) / pol.inertia;

  if (move_first) {
    sq.position.store(sq_pos + collision_direction);
  } else {
    pol.set_position(AngDir2(pol_pos - collision_direction, pol.ang_pos));
  }
  
  sq.collision_normal.store(dn);
  pol.collision_normal.store(-dn);
  
  sq.acc_f_k = sq.f_k * pol.f_k;
  pol.acc_f_k = sq.acc_f_k;

  sq.config |= PCO_IS_NORMAL;
  pol.config |= PCO_IS_NORMAL;
}
