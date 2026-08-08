#include "../../../../headers/concepts/collision.hpp"
#include "../../../../headers/primitives/types_definition.hpp"

void resolve_collision (Line& line, NEdge& pol) {
  Dir2 collision_point = collision_point_line_nedge (
    line.p, line.v, 
    pol.placed_segments, pol.size
  );

  if (collision_point.modulo2() == 0.f)
    return;

  Dir2 pol_pos = pol.position, dn;
  float distance = 0.f;
  Dir2 collision_direction = calculate_reposition_line_nedge (
    line.p, line.v, pol_pos, pol.placed_segments, pol.size, dn, distance
  );

  if (distance == 0.f)
    return;

  // calculate resolution.
  float inv_pol_mass = 1.f / pol.get_mass();
  float inv_pol_inertia = 1.f / pol.inertia;

  Dir2 pol_v = pol.velocity;
  Dir2 pol_r = collision_point - pol_pos;
  Dir2 pol_v_total = pol_r.percan().madd(pol.ang_vel, pol_v);

  float pol_coef = dn.pL(pol_r);
  float j = -2.f * (dn * pol_v_total) / (
    inv_pol_mass + 
    pol_coef * pol_coef * inv_pol_inertia 
  );
  Dir2 J = dn * j;

  pol.velocity.store(J.madd(inv_pol_mass, pol_v));
  pol.ang_vel = std::fmaf(J.pL(pol_r), inv_pol_inertia, pol.ang_vel);

  pol.set_position(AngDir2(pol_pos + collision_direction, pol.ang_pos));
  
  pol.collision_normal.store(-dn);
  pol.acc_f_k = 1.f;
  pol.config |= PCO_IS_NORMAL;
}
