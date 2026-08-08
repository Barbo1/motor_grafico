#include "../../../../headers/concepts/collision.hpp"
#include "../../../../headers/primitives/types_definition.hpp"

void resolve_collision (Circle& cir, NEdge& pol, bool move_first) {
  uint32_t filtered;
  Dir2 d = get_direction_nedge_circle(
    cir.radio, cir.position, pol.placed_segments, pol.size, filtered
  );

  if (d.modulo2() == 0.f)
    return;

  Dir2 reposition = calculate_reposition_nedge_circle(
    d, cir.radio, cir.position, pol.placed_segments, filtered
  );

  // calculate the reposition distance based on the direction d.
  Dir2 collision_point = collision_point_circle_nedge (
    d, cir.position, pol.placed_segments, pol.size
  );

  pol.reposition_segments();

  // calculate resolution.
  const Dir2 dn = d.normalize();

  float inv_cir_mass = 1.f / cir.get_mass();
  float inv_pol_mass = 1.f / pol.get_mass();
  float inv_pol_inertia = 1.f / pol.inertia;

  Dir2 cir_v = cir.velocity;
  Dir2 pol_v = pol.velocity;
  Dir2 pol_r = collision_point - Dir2(pol.position);
  Dir2 pol_v_total = pol_r.percan().madd(pol.ang_vel, pol_v);

  float pol_coef = dn.pL(pol_r);
  Dir2 v_diff = pol_v_total - cir_v;
  Dir2 J = dn * (-2.f * (dn * v_diff) / (
    inv_cir_mass + 
    inv_pol_mass + 
    pol_coef * pol_coef * inv_pol_inertia
  ));

  cir.velocity.store(J.nmadd(inv_cir_mass, cir_v));
  pol.velocity.store(J.madd(inv_pol_mass, pol_v));
  pol.ang_vel = std::fmaf(J.pL(pol_r), inv_pol_inertia, pol.ang_vel);

  if (move_first) {
    cir.position.store(Dir2(cir.position) - reposition);
  } else {
    pol.set_position(AngDir2(Dir2(pol.position) + reposition, pol.ang_pos));
  }
  
  cir.collision_normal.store(dn);
  pol.collision_normal.store(-dn);
  
  cir.acc_f_k = cir.f_k * pol.f_k;
  pol.acc_f_k = cir.acc_f_k;

  cir.config |= PCO_IS_NORMAL;
  pol.config |= PCO_IS_NORMAL;
}
